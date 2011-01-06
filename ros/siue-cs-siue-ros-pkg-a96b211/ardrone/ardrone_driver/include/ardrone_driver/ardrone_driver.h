/**
 *
 * @author Nate Roney
 *
 *
 */
#ifndef ARD_DRV_
#define ARD_DRV_

#include <ros/ros.h>

#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"
#include "std_srvs/Empty.h"

#include "ardrone_driver/Takeoff.h"
#include "ardrone_driver/Land.h"
#include "ardrone_driver/Reset.h"
#include "ardrone_driver/ARDroneState.h"
#include "ardrone_driver/Velocity.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <string>
#include <math.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp> 

/*
typedef enum ControlStates
{
    kControlStateLanded
    kControlStateAirborne
    kControlStateEStop
}
*/

class ARDroneDriver
{
    public:
    
        ARDroneDriver(ros::NodeHandle n);
        ~ARDroneDriver();
        
        //Public init function. This must be called first.
        bool Init();
        
        //run loop
        void run();
        void stop();
        
        //public commands
        int cmdTakeoff();
        int cmdLand();
        int cmdEStop();
        int cmdReset();
        
        int cmdVelocities(geometry_msgs::Twist cmd_vel);
        int cmdMaintainLink();
        int cmdZeroIMU();
        
        //public configs
        int configAltitudeMax(double meters);
        int configTiltMax(float rads);
               
    private:

        //functions to maintain flying or landed state
        void setShouldFly();
        void setShouldLand();

	    //loop to gather data from the drone
	    // runs as a thread
	    void dataCollectionLoop();
        boost::thread data_thread_;

        //loop to gather video from the drone
        // runs as a thread
        void videoCollectionLoop();
        boost::thread video_thread_;
        
        //internal configuration
        bool configSocket();
        int sendCommand(char *cmd, int len);
        
        //init command used to start data or video streams
        int initStream(int stream_port);
        
        //Private commands
        int cmdDroneInit();
        
                
        //cmd_vel callback
        void cmdVelCallback(const geometry_msgs::TwistConstPtr &msg);

        //utility to convert incoming binary data
        int32_t shift32(int32_t lsb, int32_t mlsb, int32_t mmsb, int32_t msb);
        float degreesToRadians(float degrees);
        
        //state
        bool needs_airborne_;
        bool airborne_;
        bool isInitialized_;
        ardrone_driver::Velocity current_vels_;

        std::string drone_IP_;
        int drone_port_;
        int data_port_;
        int video_port_;
        
        double altitude_;
        double tilt_max_;

        //socket communication
        int frame_number_;
        int dronefd_;
        int datafd_;
        int videofd_;
        
        struct sockaddr_in drone_addr_;
        struct sockaddr_in drone_udp_addr_;

        struct sockaddr_in data_addr_;
        struct sockaddr data_udp_addr_;
        
        struct sockaddr_in video_addr_;
        struct sockaddr video_udp_addr_;      

        boost::mutex drone_mutex_;
        boost::mutex data_mutex_;
        boost::mutex video_mutex_;

        //ROS specific
        ros::NodeHandle nh_;
        ros::NodeHandle data_handle_;
        
        ros::Subscriber cmd_vel_sub_;

        ros::Publisher imu_pub_;
        ros::Publisher img_pub_;
        ros::Publisher state_pub_;
        
        //ARDrone Services
        ros::ServiceServer takeoff_service_;
        ros::ServiceServer land_service_;
        ros::ServiceServer reset_service_;
        ros::ServiceServer zero_imu_service_;
        
        bool imuCalSrv(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
                
        bool takeoffSrv(ardrone_driver::Takeoff::Request &req,
                        ardrone_driver::Takeoff::Response &res);
                     
        bool landSrv(ardrone_driver::Land::Request &req,
                     ardrone_driver::Land::Response &res);
        
        bool resetSrv(ardrone_driver::Reset::Request &req,
                      ardrone_driver::Reset::Response &res);
       
};

#endif
