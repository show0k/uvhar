/**
 *
 *  @author Nate Roney
 *
    ARDroneDriver - a ROS driver for the Parrot AR.Drone quadrotor helicopter
    Copyright (C) 2010  Nate Roney

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <ros/ros.h>
#include "geometry_msgs/Twist.h"


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>

#include "ardrone_driver/ardrone_driver.h"

#define BUF_MAX      1024
#define DATA_BUF_MAX 2048
#define VID_BUF_MAX  8192

ARDroneDriver::ARDroneDriver(ros::NodeHandle n) : nh_(n)
{
    airborne_ = false;
    isInitialized_ = false;
    
    bzero((char *) &drone_udp_addr_, sizeof(drone_udp_addr_));
    bzero((char *) &drone_addr_, sizeof(drone_addr_));

    //initialize driver->drone fd
    dronefd_ = -1;
    datafd_ = -1;
    videofd_ = -1;
    
    //intialize frame number
    frame_number_ = 1;
    
    //params
    nh_.param( "drone_ip"    , drone_IP_  , std::string("192.168.1.1") );
    nh_.param( "drone_port"  , drone_port_, int(5556) );
    nh_.param( "data_port"   , data_port_ , int(5554) );
    nh_.param( "video_port"  , video_port_, int(5555) );
    nh_.param( "altitude_max", altitude_  , double(1) );
    nh_.param( "tilt_max"    , tilt_max_  , double(.44) );
    
    //setup Twist subscriber
    cmd_vel_sub_ = nh_.subscribe( "cmd_vel", 1, &ARDroneDriver::cmdVelCallback, this);

    //setup publishers
    //state
    //IMU
    //video    
    imu_pub_   = data_handle_.advertise<sensor_msgs::Imu>( "ardrone/imu/data", 1 ); 
    //img_pub_ = nh_.advertise<sensor_msgs::Image>( "ardrone/image_raw", 1);
    state_pub_ = data_handle_.advertise<ardrone_driver::ARDroneState>( "ardrone/state", 1 );
    
    //setup services
    takeoff_service_ = nh_.advertiseService( "ardrone/takeoff", &ARDroneDriver::takeoffSrv, this );
    land_service_    = nh_.advertiseService( "ardrone/land", &ARDroneDriver::landSrv, this );
    reset_service_   = nh_.advertiseService( "ardrone/reset", &ARDroneDriver::resetSrv, this );
    zero_imu_service_= nh_.advertiseService( "ardrone/imu/calibrate", &ARDroneDriver::imuCalSrv, this);
    
    //begin the video collection thread
    video_thread_ = boost::thread::thread( boost::bind(&ARDroneDriver::videoCollectionLoop, this) );
    
}

ARDroneDriver::~ARDroneDriver()
{
    shutdown( dronefd_, 2);
    shutdown( datafd_,  2);
}

bool ARDroneDriver::Init()
{
    int status;
    
    //if the socket isn't set up, do it
    if( !isInitialized_ )
    {
        if ( !configSocket() )
        {
            return false;
        }
    }
    
    //send init commands to drone
    status = cmdDroneInit();
    
    if (status > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ARDroneDriver::run()
{
    ros::Duration keepalive_interval(0.2);
    ros::Time last_time = ros::Time::now();
    ros::Time this_time = ros::Time::now();
    
    while( ros::ok() )
    {
        this_time = ros::Time::now();
    
        //for now, set this constantly. Once the navdata is decoded
        // well enough, the other thread should update airborne_, and
        // this thread can check needs_airborne_, and only send these
        // commands until the desired state is reached.
        if( airborne_ )
        {
            setShouldFly();
        }
        else
        {
            setShouldLand();
        }
        
        //send a watchdog reset if needed.
        if( this_time - last_time > keepalive_interval )
        {
            cmdMaintainLink();
            last_time = ros::Time::now();
        }
        
        ros::spinOnce();
     
    }
}

void ARDroneDriver::stop()
{
    video_thread_.interrupt();
    data_thread_.interrupt();
    
    ros::shutdown();
    
}

void ARDroneDriver::cmdVelCallback(const geometry_msgs::TwistConstPtr &msg)
{
    if( airborne_ )
    {
        int status;
        geometry_msgs::Twist cmd_vel;
        
        cmd_vel.linear.x  = msg->linear.x;
        cmd_vel.linear.y  = msg->linear.y;
        cmd_vel.linear.z  = msg->linear.z;
        cmd_vel.angular.z = msg->angular.z;
        
        status = cmdVelocities( cmd_vel );
    }
}

bool ARDroneDriver::imuCalSrv(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
{
    if( !airborne_ )
    {
        cmdZeroIMU();
    }
    
    return true;
}

bool ARDroneDriver::takeoffSrv(ardrone_driver::Takeoff::Request &req,
                                 ardrone_driver::Takeoff::Response &res)
{

    int status;
    status = cmdTakeoff();
    
    if( airborne_ )
    {
        res.successful = true;
    }
    else
    {
        res.successful = false;
    }
    
    return res.successful;
    
}
             
bool ARDroneDriver::landSrv(ardrone_driver::Land::Request &req,
                              ardrone_driver::Land::Response &res)
{
    int status;
    status = cmdLand();
    
    if( !airborne_ )
    {
        res.successful = true;
    }
    else
    {
        res.successful = false;
    }
    
    return res.successful;
   
}

        
bool ARDroneDriver::resetSrv(ardrone_driver::Reset::Request &req,
                               ardrone_driver::Reset::Response &res)
{
    if( isInitialized_ )
    {
        int status;
        status = cmdReset();
    }
    
    return true;   
} 

void ARDroneDriver::dataCollectionLoop()
{
    //first, configure the socket to receive data
    bzero((char *) &data_udp_addr_, sizeof(data_udp_addr_));
    bzero((char *) &data_addr_, sizeof(data_addr_));

    int addr_size = sizeof(struct sockaddr);

    
	//bzero((char *) &drone_addr_, sizeof(drone_addr_));
	data_addr_.sin_family      = AF_INET;
	data_addr_.sin_addr.s_addr = INADDR_ANY;
	data_addr_.sin_port        = htons(data_port_);
    
    //create the socket
	datafd_ = socket(AF_INET, SOCK_DGRAM, 0);

    //set a timeout on the socket
	struct timeval recv_timeout;
	recv_timeout.tv_sec   = 1;
	recv_timeout.tv_usec  = 0;
    setsockopt( datafd_, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof( recv_timeout ) );
    
    //bind the socket
	if ( bind(datafd_, (struct sockaddr *)&data_addr_, sizeof(struct sockaddr)) < 0)
	{
			ROS_ERROR("Couldn't establish connection to receive drone data.");
			ros::shutdown();
	}
    

	
	ROS_INFO( "Receiving drone data from address: %s on port: %d", drone_IP_.c_str(), data_port_);

    //need to stamp each outgoing message pair for synchronization
    ros::Time pub_time;
    
    //variables for storing the data as it is reconstructed from the packet
    // this should all be refactored into a struct eventually
    // looks like the internal header starts at byte 0, or 42 if the packet header is included (it isn't, right now)
    int DATA_START = 0;

    
    //->header
    int32_t header     = 0;
    int32_t state      = 0;
    int32_t ctrl_state = 0;
    int32_t sequence   = 0;
    int32_t last_seq   = 0;
    

    uint32_t batt_percent; //mV

    float roll = 0.0, pitch = 0.0, yaw = 0.0; //gyros
    int32_t cur_altitude = 0; //sonars
    float vx = 0.0, vy = 0.0, vz = 0.0; //accels

    int32_t tmp32; // used for bit shifting
    
    //Data structures for storage of received data
    unsigned char in_buffer[DATA_BUF_MAX];

    int      bytes_received = 0;
    int32_t option_index   = 16;

    
    //data collection happens here
    for(;;)
    {

        boost::this_thread::interruption_point();
        
        bytes_received = 0;
        option_index = 16;
        
        
        for(int z = 0; z < DATA_BUF_MAX; z++)
        {
            in_buffer[z] = 0;
        }
        

        bytes_received = recvfrom (datafd_, &in_buffer[0]
                                          , DATA_BUF_MAX
                                          , 0
                                          , (struct sockaddr *)&data_udp_addr_
                                          , (socklen_t *)&addr_size);
        
        
        //TODO: this will need refactoring, once the navdata stream is properly deciphered
        if( bytes_received <= 0 )
        {
            boost::mutex::scoped_lock lock(data_mutex_);
            initStream(data_port_);
            //nothing
            //printf("got no data\n\n");
        }
        else if( bytes_received >= 1192 ) //typical size? A: yes; it seems ALL navdata_demo packets are 1192 or 292 bytes
        {
            //first, look at the message header
            // format
            // || int32 header || int32 state || int32 sequence || int32 vision_flag ||
            
            //get the header, bytes 0-3
            header = shift32( in_buffer[DATA_START], 
                              in_buffer[DATA_START + 1], 
                              in_buffer[DATA_START + 2], 
                              in_buffer[DATA_START + 3] );
                            
            //get the state, bytes 4-7
            state = shift32( in_buffer[DATA_START + 4],
                             in_buffer[DATA_START + 5],
                             in_buffer[DATA_START + 6],
                             in_buffer[DATA_START + 7] );

            //get the sequence number, bytes 8-11
            sequence = shift32( in_buffer[DATA_START +  8],
                                in_buffer[DATA_START +  9],
                                in_buffer[DATA_START + 10],
                                in_buffer[DATA_START + 11] );
            
            //ignore the vision detect flag for now. 
           
            //ignore this packet if it is too old
            if( sequence > last_seq || sequence == 1)
            {
                last_seq = sequence;
            }
            else
            {
                printf("ignoring %d due to sequence conflict\n\n", sequence);
                last_seq--;
                usleep(5000);
                continue;
            }
       
       
            //DEBUG - test to see if these headers
            //        are being reconstructed properly.
            //printf( "header  : %d\n", header ); //if this isn't 0x55667788 (1432778632) something is wrong
            //printf( "sequence: %d\n\n", sequence ); //this should increase by 1 each iteration

            
            //now look at the 'options', these are the data packets within the packet
            // need to get size of each one, then check it's type
            // We only want the 'navdata_demo' packet, whose type id is 0
            // format
            // || int16 tag || int16 size || int8[] data ||

            tmp32 = 0;
            int32_t option_tag = 0;  //identifies the current 'option'
            int32_t option_size = 0; //# bytes in option. Use to determine location of next option header
            option_index = 16;
            sensor_msgs::Imu drone_imu;
            ardrone_driver::ARDroneState drone_state;

            //zero these messages before processing the packet
            drone_imu.linear_acceleration.x = drone_imu.linear_acceleration.y = drone_imu.linear_acceleration.z = 0.0f;
            drone_imu.orientation.x = drone_imu.orientation.y = drone_imu.orientation.z = 0.0f;            
            drone_state.vels.x = drone_state.vels.y = drone_state.vels.z = 0.0f;
            
            
            while( option_index < 900 )
            {

                
                option_tag  = 0;
                option_size = 0;
                //scan through the options. If the option isn't the 'NAVDATA_DEMO'
                // type, ignore it - that's a lot of data that isn't needed right now

                //step 1 - reconstruct the tag and size values
                option_tag  = in_buffer[option_index++];
                option_tag |= (int16_t)in_buffer[option_index++] << 8;

                option_size  = in_buffer[option_index++];
                option_size |= (int16_t)in_buffer[option_index++] << 8;
              
                
                //printf("got option tag: %d with size: %d at index: %d\n\n", option_tag, option_size, option_index);
                
                if( option_tag == 0 )
                {
                    
                    //parse and fill in messages
                    ctrl_state = shift32( in_buffer[option_index],
                                          in_buffer[option_index + 1],
                                          in_buffer[option_index + 2],
                                          in_buffer[option_index + 3] );

                    //printf("ctrl_state: %d\n", ctrl_state);
                    
                    option_index += 4;
                    batt_percent = shift32( in_buffer[option_index],
                                            in_buffer[option_index + 1],
                                            in_buffer[option_index + 2],
                                            in_buffer[option_index + 3] );

                    //batt_percent /= 1000;
                    //printf("batt_percent: %d\n", batt_percent);

                    //the floats will be the interesting part. The assumption here is that 
                    //they are using the same type of cast used to send AT*PCMD.
                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );

                    roll = *(float *)&tmp32;

                    

                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );

                    pitch = *(float *)&tmp32;

     
                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );                    

                    yaw = *(float *)&tmp32;

                    
                    //printf("orientation: roll: %2.2f, pitch: %2.2f yaw: %2.2f\n", roll, pitch, yaw);                    
                            

                    //altitude is a simple int32
                    option_index += 4;
                    cur_altitude = shift32( in_buffer[option_index],
                                            in_buffer[option_index + 1],
                                            in_buffer[option_index + 2],
                                            in_buffer[option_index + 3] ); 
                                            
                    
                    //velocities are floats, same as gyros
                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );                    

                    vx = *(float *)&tmp32;

                    
                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );                    

                    vy = *(float *)&tmp32;

                    
                    option_index += 4;
                    tmp32 = shift32( in_buffer[option_index],
                                     in_buffer[option_index + 1],
                                     in_buffer[option_index + 2],
                                     in_buffer[option_index + 3] );                    

                    vz = *(float *)&tmp32;                    

                    option_index += 4;
                    
                    //printf("orientation: vx: %2.2f, vy: %2.2f, vz: %2.2f\n\n", vx, vy, vz);
                    
                    //publish the valid data packet
                    //TODO: the data from the drone is *probably* in g's. will need to convert
                    //      that to meters/sec for velocity, and rad/sec for rotation.
                    //TODO: make sure the coordinate systems match what ROS expects


                    //pub_time = ros::Time::now();

                    //populate the Imu message
                    //drone_imu.header.stamp = pub_time;
                    drone_imu.linear_acceleration_covariance[0] = -1;
                    
                    drone_imu.orientation.x = degreesToRadians( -(roll / 1000.0) );
                    drone_imu.orientation.y = degreesToRadians( -(pitch / 1000.0)   );
                    //drone_imu.orientation.z = degreesToRadians( -(yaw / 1000.0)   );
                    drone_imu.orientation_covariance[0] = -1;
                    
                    
                    //populate the ARDroneState message
                    //drone_state.header.stamp = pub_time;
                    drone_state.altitude = (float)cur_altitude/1000.0;
                    drone_state.battery_percentage = batt_percent;
                    drone_state.vels.x = (vx / 1000.0);
                    drone_state.vels.y = (vy / 1000.0);
                    drone_state.vels.z = (vz / 1000.0);                   

                    
                    option_index += (option_size + 1);
                }
                else if( option_tag == 5 )
                {
                    //raw accels & gyros?
                    
                    option_index += (option_size + 1);
                }
                else
                {
                    //printf("failed to find tag, moving on to next...\n\n");
                    option_index += (option_size + 1);
                    //option_index = 600;
                }
   
            }
            
            //finally, publish.
            {
            boost::mutex::scoped_lock lock(data_mutex_);
            
            current_vels_.x = drone_state.vels.x;
            current_vels_.y = drone_state.vels.y;
            current_vels_.z = drone_state.vels.z;

            imu_pub_.publish( drone_imu );
            state_pub_.publish( drone_state );
            } 

        }
    }
}

float ARDroneDriver::degreesToRadians(float degrees)
{
    return degrees * (M_PI / 180);
}

int32_t ARDroneDriver::shift32(int32_t lsb, int32_t mlsb, int32_t mmsb, int32_t msb)
{
    int32_t tmp = 0;

    tmp  = lsb;
    tmp |= mlsb << 8;
    tmp |= mmsb << 16;
    tmp |= msb  << 24;
	
    return tmp;
}

void ARDroneDriver::videoCollectionLoop()
{

    //first, configure the socket to receive data
    bzero((char *) &video_udp_addr_, sizeof(video_udp_addr_));
    bzero((char *) &video_addr_, sizeof(video_addr_));

    int addr_size = sizeof(struct sockaddr);


	//bzero((char *) &drone_addr_, sizeof(drone_addr_));
	video_addr_.sin_family      = AF_INET;
	video_addr_.sin_addr.s_addr = INADDR_ANY;
	video_addr_.sin_port        = htons(video_port_);
    
    //create the socket
	videofd_ = socket(AF_INET, SOCK_DGRAM, 0);

    //bind the socket
	if ( bind(videofd_, (struct sockaddr *)&video_addr_, sizeof(struct sockaddr)) < 0)
	{
			ROS_ERROR("Couldn't establish connection to receive drone video.");
			ros::shutdown();
	}

    //set a timeout on the socket
	struct timeval recv_timeout;
	recv_timeout.tv_sec   = 1;
	recv_timeout.tv_usec  = 0;
    setsockopt( videofd_, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof( recv_timeout ) );


	//ROS_INFO("Receiving drone video from address: %s on port: %d", drone_IP_.c_str(), video_port_);


    //Data structures for storage of received data
    char img_data[BUF_MAX];

    char in_buffer[DATA_BUF_MAX];

    int bytes_received = 0;
    //data collection happens here
    for(;;)
    {
        boost::this_thread::interruption_point();

        bytes_received = 0;
        
        bytes_received = recvfrom (videofd_, &in_buffer[0]
                                           , BUF_MAX
                                           , 0
                                           , (struct sockaddr *)&video_udp_addr_
                                           , (socklen_t *)&addr_size);
          
        
        if( bytes_received <= 0 )
        {
            //nothing
            continue;
        }
        else
        {
            //parse
                
            
        }
    }  
}

int ARDroneDriver::cmdTakeoff()
{
    char takeoff_cmd[BUF_MAX];
    
    int status;

	frame_number_ = 1;
	
    //zero IMU before takeoff
    cmdZeroIMU();
    
    //set command to takeoff velocity (straight up, full speed)
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x  = 0.0f;
    cmd_vel.linear.y  = 0.0f;
    cmd_vel.linear.z  = 1.0f;
    cmd_vel.angular.z = 0.0f;
    
    //send the takeoff bit
	//sprintf( takeoff_cmd, "AT*REF=%d,290718208\r", frame_number_ );
	//frame_number_++;
	
    //status = sendCommand( takeoff_cmd, strlen(takeoff_cmd) );
    
    setShouldFly();
    
    cmdMaintainLink();
    
    //need to wait a bit after setting the takeoff bit , then fire altitude+ cmd
    usleep(5000);
    
    //send an up command for takeoff assist
    for(int tk = 0; tk < 15; tk++)
    {
        cmdVelocities( cmd_vel );
    }
    
    cmd_vel.linear.z = 0;
    cmdVelocities( cmd_vel );
    
    cmdMaintainLink();
    
    airborne_ = true;
    
    return status;
}

int ARDroneDriver::cmdLand()
{
    char cmdLand[BUF_MAX];
    
    frame_number_ = 1;
    //sprintf( cmdLand, "AT*REF=%d,290717696\r", frame_number_++);

    //for(int l = 0; l < 20; l++)
    //{
	//    status = sendCommand( cmdLand, strlen(cmdLand) );
	//}
	
	setShouldLand();
	
	airborne_ = false;
	
	return 1;
}

void ARDroneDriver::setShouldFly()
{
    char fly_cmd[BUF_MAX];
    int status;
    
	sprintf( fly_cmd, "AT*REF=%d,290718208\r", frame_number_++ );
	
    status = sendCommand( fly_cmd, strlen(fly_cmd) );   
}

void ARDroneDriver::setShouldLand()
{
    char land_cmd[BUF_MAX];
    int status;
    
    sprintf( land_cmd, "AT*REF=%d,290717696\r", frame_number_++);
    
    status = sendCommand( land_cmd, strlen( land_cmd ) );
}

int ARDroneDriver::cmdEStop()
{
    char estop_cmd[BUF_MAX];
    int status;
    
    int32_t estop_bit = 0b00000000000000000000000100000000;
	
	//send the command several times; the drone seems to need this
	//for(int tk = 0; tk < 5; tk++)
	//{
	
    sprintf( estop_cmd, "AT*REF=%i,%d\r", frame_number_, estop_bit );
    frame_number_++;
    status = sendCommand( estop_cmd, strlen(estop_cmd) );
    
    //}

	#ifdef DEBUG
	    printf("sent %i bytes for ESTOP\n\n", status);
    #endif
       
    return status;
}

int ARDroneDriver::cmdReset()
{
    char estop_cmd[BUF_MAX];
    int status;
	//int32_t estop_bit = 0b00000000000000000000000100000000;
	
	frame_number_ = 1;
	
	//To reset the drone to 'normal' state, a flip-flop of bit 8
	// in AT*REF must be sent
    sprintf( estop_cmd, "AT*REF=%d,256\r", frame_number_++);
    status = sendCommand( estop_cmd, strlen(estop_cmd) );
    
    sprintf( estop_cmd, "AT*REF=%d,0\r", frame_number_++);
    status = sendCommand( estop_cmd, strlen(estop_cmd) );
    
    sprintf( estop_cmd, "AT*REF=%d,256\r", frame_number_++);
    status = sendCommand( estop_cmd, strlen(estop_cmd) );
    
    cmdMaintainLink();
    
    airborne_ = false;
    
    return status;
}
        

int ARDroneDriver::cmdVelocities(geometry_msgs::Twist cmd_vel)
{
    if( airborne_ )
    {
        char velocity_cmd[BUF_MAX];
        int movement_flag = 0;
        
        float ly = -cmd_vel.linear.y;  //drone movement - up & down
        float lx = -cmd_vel.linear.x;  //drone movement - forward & back
        float lz = cmd_vel.linear.z;  //drone movement - left & right
        float az = cmd_vel.angular.z; //drone movement - rotation
        
        //TODO: map az in radians from -1 to 1
        
        //unless issuing ly or lx, the hover bit needs to be set. If issuing
        // ly or lx, set the progressive bit
        if( ly == 0.0f && lx == 0.0f)
        {
            movement_flag = 0;
        }
        else
        {
            movement_flag = 1;
        }
        
        
        //set up the movement command.
        sprintf( velocity_cmd, "AT*PCMD=%d,%d,%d,%d,%d,%d\r", frame_number_++
                                                            , movement_flag
                                                            , *(int *)&ly
                                                            , *(int *)&lx
                                                            , *(int *)&lz
                                                            , *(int *)&az );
                                                            
        
        return sendCommand( velocity_cmd, strlen( velocity_cmd ) );
        
   
    }
    else
    {
        return -1;
    }
}

int ARDroneDriver::cmdMaintainLink()
{
    char wd_cmd[BUF_MAX];
    int status;
    
    sprintf( wd_cmd, "AT*COMWDG=%d\r", frame_number_++ );
    
    status = sendCommand( wd_cmd, strlen(wd_cmd) );
    
    return status;
}

int ARDroneDriver::cmdZeroIMU()
{
    char zero_cmd[BUF_MAX];
    int status;
    
    sprintf( zero_cmd, "AT*FTRIM=%d\r", frame_number_ );
    frame_number_++;
    
    status = sendCommand( zero_cmd, strlen(zero_cmd) );
    
    return status;
}

int ARDroneDriver::cmdDroneInit()
{
    char config_cmd[BUF_MAX];
    char ctrl_cmd[BUF_MAX];
    
    int status1, status2, status3, status4;
    
    frame_number_ = 2;
    
    
    //First, start the data stream.
	initStream( data_port_ );
    
    //these commands allow the data stream to be configured and started.
	sprintf( config_cmd, "AT*CONFIG=%d,\"general:navdata_demo\",\"FALSE\"\r", 1 ); 	
 	sprintf( ctrl_cmd, "AT*CTRL=%d,%d,%d\r", frame_number_++, 5, 0 );

	//send commands
	status1 = sendCommand( config_cmd, strlen( config_cmd ) );
	status2 = sendCommand( ctrl_cmd, strlen( ctrl_cmd ) );
	status3 = configTiltMax( float( tilt_max_ ) );
	status4 = configAltitudeMax( altitude_ );
	
	//now that the data init, config and ctrl have been sent, the data gathering thread should begin
    data_thread_ = boost::thread::thread( boost::bind(&ARDroneDriver::dataCollectionLoop, this) );
	
	if ( status1 < 0 || status2 < 0 || status3 < 0 || status4 < 0)
	{
	    ROS_ERROR("Drone initialization failed.");
	    return -1;
    }
    
    ROS_INFO("Drone is initialized");
    
    return status1;
}

int ARDroneDriver::configAltitudeMax(double meters)
{
    char alt_cmd[BUF_MAX];
    int status;
    
    meters *= 1000;
    
    sprintf( alt_cmd, "AT*CONFIG=%d,\"control:altitude_max\",\"%d\"\r", frame_number_++, int( meters ) );
    
    status = sendCommand( alt_cmd, strlen( alt_cmd ) );
    
    return status;
}

int ARDroneDriver::configTiltMax(float rads)
{
    char angle_cmd[BUF_MAX];
    sprintf( angle_cmd, "AT*CONFIG=%d,\"control:euler_angle_max\",\"%2.6f\"\r", frame_number_++, rads );
    
    int status;
    
    status = sendCommand(angle_cmd, strlen( angle_cmd ) );
    
    return status;
}

bool ARDroneDriver::configSocket()
{
    int32_t flags;

    if (!isInitialized_)
    {
    
        //setup sockaddr structs
		bzero((char *) &drone_udp_addr_, sizeof(drone_udp_addr_));
		drone_udp_addr_.sin_family = AF_INET;
		drone_udp_addr_.sin_addr.s_addr = INADDR_ANY;
		drone_udp_addr_.sin_port = htons(drone_port_ + 100);
		
		//bzero((char *) &drone_addr_, sizeof(drone_addr_));
		drone_addr_.sin_family = AF_INET;
		drone_addr_.sin_addr.s_addr = inet_addr( "192.168.1.1" );//drone_IP_.c_str() );
		drone_addr_.sin_port = htons(drone_port_);
        
        //create the socket
		dronefd_ = socket(AF_INET, SOCK_DGRAM, 0);
    
        //set non blocking on the file descriptor
		flags = fcntl(dronefd_, F_GETFL, 0);
		if (flags >=0 )
		{
				flags |= O_NONBLOCK;
				flags = fcntl(dronefd_, F_SETFL, flags);
		}

        //bind the socket
		if ( bind(dronefd_, (struct sockaddr *)&drone_udp_addr_, sizeof(struct sockaddr)) < 0)
		{
				ROS_ERROR("Couldn't connect to the drone. Check WiFi and try again.");
				//ros::shutdown();
				return false;
		}
		
		ROS_INFO("Connected to drone at address: %s on port: %d", drone_IP_.c_str(), drone_port_);
		isInitialized_ = true;
		return true;
	}
	
	//already initialized
	return false;
}



int ARDroneDriver::initStream(int stream_port)
{
    int init_status = 0;
    
    //this can probably all go in a class, DroneData
    char init_cmd[] = "1";
    int32_t init_flags;
    int initfd;
    
    struct sockaddr_in data_init_addr, data_init_udp_addr;
    data_init_addr.sin_family      = AF_INET;
    data_init_addr.sin_addr.s_addr = inet_addr( "192.168.1.1" );
    data_init_addr.sin_port        = htons(stream_port);
    
    
    data_init_udp_addr.sin_family      = AF_INET;
    data_init_udp_addr.sin_addr.s_addr = INADDR_ANY;
    data_init_udp_addr.sin_port        = htons( stream_port + 100 );
    
    initfd = socket(AF_INET, SOCK_DGRAM, 0);
   
    init_flags = fcntl(initfd, F_GETFL, 0);
    if( init_flags >= 0 )
    {
        init_flags |= O_NONBLOCK;
        init_flags = fcntl(initfd, F_SETFL, init_flags);
    }
    bind( initfd, (struct sockaddr *)&data_init_udp_addr, sizeof(struct sockaddr) );
    
    init_status = sendto(initfd, init_cmd
              , strlen(init_cmd)
              , 0
              , (struct sockaddr *)&data_init_addr
              , sizeof( data_init_addr ) );
    
    shutdown(initfd, 2);
    
    return init_status;
}

int ARDroneDriver::sendCommand(char *cmd, int len)
{
    int status;
    
    if(dronefd_ >= 0)
	{
	    status = sendto(dronefd_, cmd
	                    , strlen(cmd)
	                    , 0
	                    , (struct sockaddr *)&drone_addr_
	                    , sizeof(drone_addr_));
	                    
	    usleep(2000);
	    return status;
	}

	return -1;
}













