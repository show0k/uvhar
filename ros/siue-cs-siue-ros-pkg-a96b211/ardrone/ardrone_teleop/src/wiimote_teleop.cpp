/**
 *
 *  @author Nate Roney
 *
    wiimote_teleop - A ROS node for teleoperation of ARDrone_Driver
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
#include "ros/ros.h"
#include "wiimote/State.h"
#include "geometry_msgs/Twist.h"
#include "ardrone_driver/Takeoff.h"
#include "ardrone_driver/Land.h"
#include "ardrone_driver/Reset.h"

#include <cmath>
#include <ctime>
#include <string>

using namespace std;

//
geometry_msgs::Twist cmd_vel;

ros::ServiceClient takeoff_client;
ros::ServiceClient land_client;
ros::ServiceClient reset_client;


//wiimote buttons
#define WII_BTN_1     0
#define WII_BTN_2     1
#define WII_BTN_A     4
#define WII_BTN_B     5
#define WII_BTN_PLUS  2
#define WII_BTN_MINUS 3
#define WII_BTN_LEFT  8
#define WII_BTN_RIGHT 9
#define WII_BTN_UP    6
#define WII_BTN_DOWN  7
#define WII_BTN_HOME  10

#define K_P (0.1)
#define VEL_MAX 0.8

void wiimoteCallback(const wiimote::State::ConstPtr &wiiCmd)
{

    if( wiiCmd->buttons[WII_BTN_PLUS] )
	{
	    ardrone_driver::Takeoff takeoff;
	    takeoff_client.call( takeoff );
	    return;
    }
    else if( wiiCmd->buttons[WII_BTN_MINUS] )
    {
        ardrone_driver::Land land;
        land_client.call( land );
        return;
    }
    else if( wiiCmd->buttons[WII_BTN_HOME] )
    {
        ardrone_driver::Reset reset;
        reset_client.call( reset );
        return;
    }
    
    //*****************
    //Hovering commands
    //*****************
    //if 1 button is held, up and down arrows function as altitude
    //    left and right allow rotation of the drone
    if( wiiCmd->buttons[WII_BTN_1] )
    {

		//altitude
		if ( wiiCmd->buttons[WII_BTN_RIGHT] )
		{
			cmd_vel.linear.z += 1 * K_P;
			if(cmd_vel.linear.z > VEL_MAX) cmd_vel.linear.z = VEL_MAX;
		}
		
		if ( wiiCmd->buttons[WII_BTN_LEFT] )
		{
			cmd_vel.linear.z -= 1 * K_P;
			if(cmd_vel.linear.z < -(VEL_MAX)) cmd_vel.linear.z = -(VEL_MAX);
		}
		
		//left/right rotation
		if ( wiiCmd->buttons[WII_BTN_DOWN] )
		{
			cmd_vel.angular.z += 1 * K_P;
			if(cmd_vel.angular.z < -(VEL_MAX)) cmd_vel.angular.z = -(VEL_MAX);
		}
		
		if ( wiiCmd->buttons[WII_BTN_UP] )
		{
			cmd_vel.angular.z -= 1 * K_P;
			if(cmd_vel.angular.z > VEL_MAX) cmd_vel.angular.z = VEL_MAX;
		}		

        return;
    }
    
    //*****************
    //Velocity commands
    //*****************
    //if 2 button is held, the wiimote IMU moves the drone
	if( wiiCmd->buttons[WII_BTN_2] )
	{
	    cmd_vel.linear.x = cmd_vel.linear.y = 0.0f;


        cmd_vel.linear.x = wiiCmd->linear_acceleration_zeroed.z / 25;          
        cmd_vel.linear.y = wiiCmd->linear_acceleration_zeroed.y / 25;
		return;
	}
	

	//no deadman, zero all movement and zero filters
	cmd_vel.linear.x = cmd_vel.linear.y = cmd_vel.linear.z = 0.0f;
	cmd_vel.angular.x = cmd_vel.angular.y = cmd_vel.angular.z = 0.0f;
	

	return;

} 

int main(int argc, char **argv)
{
	ros::init(argc, argv, "wiimote_teleop_node");
	ros::NodeHandle n;
	ros::NodeHandle nh("~");
	
	ros::Subscriber wiimoteSubscriber;
	ros::Publisher cmdPublisher;
	
	ros::Rate loop_rate(30);
	
	wiimoteSubscriber = n.subscribe("wiimote/state", 1, wiimoteCallback);

	cmdPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

    takeoff_client = n.serviceClient<ardrone_driver::Takeoff>("ardrone/takeoff");
    land_client = n.serviceClient<ardrone_driver::Land>("ardrone/land");
    reset_client = n.serviceClient<ardrone_driver::Reset>("ardrone/reset");
	
	while ( ros::ok() )
	{	
		ros::spinOnce();		
		
	    cmdPublisher.publish ( cmd_vel );

	    
	    loop_rate.sleep();
 	}

  	return 0;
}
