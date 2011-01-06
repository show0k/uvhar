#ifndef SP_
#define SP_

#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>


/**
 * @class SerialPort
 *
 * @author Nate Roney
 *
 * Enables simple serial port access in Linux
 *
 * Example usage:
 *
	SerialPort lsc("/dev/ttyUSB1", B38400);
	lsc.readData(buffer, 200);
	lsc.closePort();
 *
 * --OR--
 *
	SerialPort lsc();
	lsc.setPort("/dev/ttyUSB1");
	lsc.setBaudRate(B38400);
	lsc.openPort();
	lsc.readData(buffer, 200);
	lsc.closePort();
 *
 */
class SerialPort
{
public:
	
	/**
	 * Default Constructor
	 *
	 * If using this constructor, you must use setPort, setBaudRate and openPort to initiate
	 *  serial port communications.
	 */
	SerialPort();
	
	
	/**
	 * Using this constructor will open the specified port at the specified baud rate
	 * 
	 * Baud rates MUST be entered with a leading 'B' for internal conversion to speed_t
	 *  e.g. B9600 or B38400
	 *
	 * @param port : the port to open
	 * @param baudRate : the baud rate of the port
	 */
	SerialPort(char *port, int baudRate);
	
	
	/**
	 * Destructor
	 *  performs a clean disconnect
	 */
	~SerialPort();


	/**
	 * Reads data from the serial port
	 *
	 * @param readBuffer : A character array to store the data
	 * @param bytesToRead : The number of bytes to read. This must be >= the size of readBuffer
	 *
	 * @return bytesRead : -1 on error, >= 0 indicating number of bytes read on success
	 */
	int readData(char *&readBuffer, int bytesToRead);
	
	
	/**
	 * Writes data to the serial port
	 *
	 * @param writeBuffer : A character array storing data to be written
	 * @param bytesTowrite : The number of bytes to write. This must be >= the size of readBuffer
	 *
	 * @return bytesRead : -1 on error, >= 0 indicating number of bytes written on success
	 */
	int writeData(char *&writeBuffer, int bytesToWrite);
	
	
	/**
	 * Opens the specified port 
	 *
	 * @return boolean
	 */
	bool openPort();
	
	
	/**
	 * Closes the currently specified port
	 */
	void closePort();
	
	
	/**
	 * Sets the baud rate. 
	 *
	 * @param baudRate : the speed at which to open the port
	 * 		Baud rates MUST be entered with a leading 'B' for internal conversion to speed_t
	 *  		e.g. B9600 or B38400
	 */
	 void setBaudRate(int baudRate);
	 
	 /**
	  * Sets the port. 
	  *
	  * @param port : the port 
	  */
	  void setPort(char *port);
	  
	  /**
	   * Queries the port status; open or closed
	   */
	  bool isOpen();

protected:

	/**
	 * Internal flag indicating port status
	 */
	bool portIsOpen;
	
	
	/**
	 * File descriptor representing the opened serial port
	 */
	int fileDescriptor;
	
	
	/**
	 * The name of the serial port
	 */
	char *portName;
	
	
	/**
	 * The speed at which to open the port
	 *
	 * Baud rates MUST be entered with a leading 'B' for internal conversion to speed_t
	 * e.g. B9600 or B38400
	 */
	int portBaudRate;
	
	
	/**
	 * Configures the port. 
	 *
	 * @param fd : a file descriptor
	 * @param baudRate : the speed at which to open the port
	 * 		Baud rates MUST be entered with a leading 'B' for internal conversion to speed_t
	 *  		e.g. B9600 or B38400
	 */
	int configurePort(int fd, int baudRate);

};


#endif
