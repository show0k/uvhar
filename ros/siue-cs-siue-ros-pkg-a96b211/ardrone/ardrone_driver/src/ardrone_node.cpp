/**
 *
 *  @author Nate Roney
 *
    ardrone_node - A ROS node that implements the ARDrone_Driver
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

#include "ardrone_driver/ardrone_driver.h"


int main(int argc, char **argv)
{
		ros::init(argc, argv, "ardrone_driver");
		ros::NodeHandle nh;

		ARDroneDriver *ard = new ARDroneDriver(nh);

        
		if( !ard->Init() )
		{
		    ROS_ERROR("Init failed for ARDrone_Driver");
		    exit(1);
	    }

        ard->run();
                
        ard->stop();
            
		return 0;

}

