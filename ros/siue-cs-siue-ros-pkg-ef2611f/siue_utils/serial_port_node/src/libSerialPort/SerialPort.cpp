#include "serial_port_node/SerialPort.h"
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>


SerialPort::SerialPort()
{
	portName = NULL;
	portBaudRate = B0;
	portIsOpen = false;
}

SerialPort::SerialPort(char *port, int baudRate)
{
	portName = port;
	portBaudRate = baudRate;
	portIsOpen = openPort();	
}

SerialPort::~SerialPort()
{
	close(fileDescriptor);
}

bool SerialPort::openPort()
{	
	if(!portIsOpen && (portName != NULL) )
	{	
		fileDescriptor = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);	
	}

	if(fileDescriptor == -1)
	{
		perror("openPort(): Unable to open serial port");
		return false;
	}
	else
	{
		fcntl(fileDescriptor, F_SETFL, 0);
		portIsOpen = true;
		configurePort(fileDescriptor, portBaudRate);
		return true;
	}
		
}

void SerialPort::closePort()
{
	if(portIsOpen) 
	{
		close(fileDescriptor);
		portName = NULL;
		fileDescriptor = -1;
	}
}

int SerialPort::readData(char *&readBuffer, int bytesToRead)
{
	int bytesRead = -1;

	if(portIsOpen)
	{
		bytesRead = read(fileDescriptor, readBuffer, bytesToRead);
	}

	return bytesRead;
}

int SerialPort::writeData(char *&writeBuffer, int bytesToWrite)
{
	int bytesWritten = -1;

	if(portIsOpen)
	{
		bytesWritten = write(fileDescriptor, writeBuffer, bytesToWrite);
	}
	
	return bytesWritten;
}
void SerialPort::setBaudRate(int baudRate)
{
	portBaudRate = baudRate;
}

void SerialPort::setPort(char *port)
{
	portName = port;
}

int SerialPort::configurePort(int fd, int baudRate)      // configure the port
{
	struct termios port_settings;      // structure to store the port settings in

	cfsetispeed(&port_settings, baudRate);    // set baud rates
	cfsetospeed(&port_settings, baudRate);

	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cflag |= CS8;

	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
	return(fd);

}	

bool SerialPort::isOpen()
{
	return portIsOpen;
}


