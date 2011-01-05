/**
 * @author Nate Roney
 * copyright 2010
 * gps_node
 *
 * Interfaces with a serial based GPS receiver. Attempts to parse until the 
 *  GPRMC sentence is found. Populates a ROS message with that data and publishes
 *  it on the topic "gps_msg"
 *
 * Default behavior uses a GPS device attached to /dev/ttyUSB0. This can be changed
 *  with parameter "gps_port"
 *
     This file is part of gps_node.

    gps_node is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gps_node is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gps_node.  If not, see <http://www.gnu.org/licenses/>.

 */
#include "serial_port_node/SerialPort.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "gps_node/GPSPoint.h"
#include "GPS_Parser.h"

using namespace std;


int main(int argc, char** argv)
{
  	ros::init(argc, argv, "gps_node");
  	ros::NodeHandle n;
  	ros::NodeHandle nh("~");
  	
	string port = "/dev/ttyUSB0";
	ros::Rate loop_rate(1);

	nh.param("gps_port", port, string("/dev/ttyUSB0"));
	nh.deleteParam("gps_port");
	
	ROS_INFO("Opening GPS device at: %s", port.c_str());
	SerialPort gpsDevice((char*)port.c_str(), B4800);
	
	if(!gpsDevice.isOpen())
	{
		ROS_ERROR("Unable to open GPS device at: %s, verify port and try again", port.c_str());
		return 1;
	}


	char *data; 
	int status = -1;
  	string lon, lat;
  	GPS_Parser pr;
  	gps_node::GPSPoint currentCoordinates;
 	ros::Publisher gpsPub = n.advertise<gps_node::GPSPoint>("gps_msg", 1);
  	
  	while (ros::ok())
  	{
		status = -1;	
  		data = new char[200];
  		status = gpsDevice.readData(data, 200);

		if(status > 50)
		{
			//Parse here, publish if valid
			if( pr.parse(data) )
			{
				currentCoordinates.lon = pr.getLongitude();
				currentCoordinates.lat = pr.getLatitude();
				currentCoordinates.speed = pr.getSpeed();
				currentCoordinates.bearing = pr.getBearing();
				currentCoordinates.status = pr.getStatus();
				gpsPub.publish(currentCoordinates);
			}
		}

		usleep(500);					
		delete data;
	
  		ros::spinOnce();
		loop_rate.sleep();
  	}

  
  	return 0;
}

