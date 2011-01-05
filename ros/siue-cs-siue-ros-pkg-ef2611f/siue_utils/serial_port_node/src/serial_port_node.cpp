/**
 * serial_port_node
 *
 * Provides simple access to the serial port with ROS services
 *
 * Default connection parameters are
 * serial_port: /dev/ttyUSB0
 * baud_rate: 115200
 * 
 * Connection specifics: 8-N-1
 */

#include "serial_port_node/SerialPort.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "serial_port_node/SerialWrite.h"
#include "serial_port_node/SerialRead.h"

using namespace std;


SerialPort *device;

bool read(serial_port_node::SerialRead::Request  &req,
		      serial_port_node::SerialRead::Response &res)
{
	int length = req.length;
	int status = -1;
	char *read_buffer = new char[length];
	
	status = device->readData(read_buffer, length);

	if(status > -1)
	{
		res.bytesRead = status;
		res.data = read_buffer;
		res.status = true;
	}
	else
	{
		res.bytesRead = -1;
		res.status = false;
	}
	
	delete read_buffer;
	
	return res.status;
}

bool write(serial_port_node::SerialWrite::Request  &req,
		       serial_port_node::SerialWrite::Response &res)
{
	char *write_buffer = (char*)req.data.c_str();
	int length = req.length;
	int status = -1;
	
	status = device->writeData(write_buffer, length);
	
	delete write_buffer;
	
	if(status > -1)
	{
		res.bytesWritten = status;
		res.status = true;
		return true;
	}
	else
	{
		res.bytesWritten = -1;
		res.status = false;
		return false;
	}	
}

int main(int argc, char** argv)
{
  	ros::init(argc, argv, "serial_port");
  	ros::NodeHandle n;
  	ros::NodeHandle nh("~");
  	
	string port = "/dev/ttyUSB0";
	int baud_rate = 115200;

	nh.param("serial_port", port, string("/dev/ttyUSB0"));
	nh.param("serial_baud_rate", baud_rate, int(115200));
	nh.deleteParam("serial_port");
	nh.deleteParam("serial_baud_rate");
	

	switch(baud_rate)
	{
		case 4800:
			baud_rate = B4800;
			break;
		
		case 9600:
			baud_rate = B9600;
			break;
		
		case 19200:
			baud_rate = B19200;
			break;
		
		case 38400:
			baud_rate = B38400;
			break;
		
		case 57600:
			baud_rate = B57600;
			break;
		
		case 115200:
			baud_rate = B115200;
			break;
		
		default:
			ROS_ERROR("Bad baud rate. Please select a standard baud rate");
			return 1;
		
	}
	
	
	ROS_INFO("Opening serial device: %s, with baud rate %i: ", port.c_str(), baud_rate);
	device = new SerialPort((char*)port.c_str(), baud_rate);
	
	
	if(!device->isOpen())
	{
		ROS_ERROR("Unable to open serial device: %s, please verify settings", port.c_str());
		return 1;
	}
	
	
	//Advertise services if the port opens correctly.
	ros::ServiceServer serial_write = n.advertiseService("serial_port_write", write);
	ros::ServiceServer serial_read = n.advertiseService("serial_port_read", read);
	

	ros::spin();
  
  	return 0;
}

