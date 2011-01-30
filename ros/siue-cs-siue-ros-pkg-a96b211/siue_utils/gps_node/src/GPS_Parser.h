/**
* @author Nate Roney
* @author Andy Zeisler
* copyright 2010
*
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
*
*/

#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include<string>

using namespace std;


class GPS_Parser
{
public:
/**
* Class constructor: initializes all values to nothing
*/
	GPS_Parser();
	
	/**
* Parse takes a sentence of any length and looks for a complete 
* $GPRMC sentence to parse.  It does this by find the location of
* the first $ which is present at the beginning of all $GPRMC sentences.
* It then goes through the sentence and counts the commas.  A complete
* sentence will contain 11 or 12 commas depending on the presence of the
* Magnetic Variation.  It calls SeparateSentence and returns true if a complete
* sentence was found, returns false if not.
*
* @param sentence the $GPRMC sentence to be parsed
* @return true if complete sentence found, false otherwise
*/
	bool parse(string sentence);


//RETURN FUNCTIONS
/**
*	Returns the time
*/
	string getTime();

/**
*	Returns the status
*/
	int getStatus();

/**
*	Returns the longitude
*/
	string getLongitude();

/**
*	Returns the latitude
*/
	string getLatitude();

/**
*	Returns the speed
*/
	double getSpeed();

/**
*	Returns the bearing
*/
	double getBearing();

/**
*	Returns the date
*/
	string getDate();

/**
*	Returns the magneticVar
*/
	string getMagneticVar();

/**
*	Returns the checksum
*/
	string getChecksum();

private:

	string time; 
	string longitude;
	string latitude;
	string date;
	string magneticVar;
	string checksum;
	int status;
	double speed;
	double bearing;

	/**
* Separates a complete sentence into the individual parts and sets their
* corresponding variable to the values found in the sentence.  Does not
* return a value.
*
* @param sentence the complete $GPRMC sentence to be separated
* @return nothing
	*/
	bool separateSentence(string sentence);

	/**
* Helper function used to pull data out based on comma locations.  Each
* components data can be found between commas.  This function stores the 
* data from the beginning of the substring to the first comma in a variable.
* It then resets the substring by removing the previously stored data and the 
* comma.  This places the next bit of data at the 'head' of the string.  It
* then updates the location of the new first comma for future separation.
* 
* @param &commaLocation holds the string index of the first comma in the subString
* @param &subString the remaining parts of the original complete sentence after each 
*	separation
* @return returns the 'head' information going from the beginning to the first comma
*/
	string modifySubString(int &commaLocation, string &subString);

		/**
* Data verification function used to attempt to cast the data to doubles.  If they
* fail then the data must be bad so we return false, otherwise data is assumed to be
* valid.  We test the data length and also attempt to cast it to a double.  If either
* test fails then the data must be invalid.  
* 
* @param data holds the string of data to be tested
* @param maxLength holds the max length the data should have.
* @return returns TRUE if data passed all tests, false if it fails any.
*/
	bool testData(string data, int maxLength);

	//SET FUNCTIONS
/**
*	Sets the time to the time found in the complete sentence.
*
* @param newTime the new time that was pulled from the complete sentence
*/
	void setTime(string newTime);

/**
*	Sets the status to the status found in the complete sentence
*
* @param newStatus the new status that was pulled from the complete sentence
*/
	void setStatus(int newStatus);

/**
*	Sets the longitude to the longitude found in the complete sentence
*
* @param newLongitude the new longitude that was pulled from the complete sentence
*/
	void setLongitude(string newLongitude);

/**
*	Sets the lattitude to the lattitude found in the complete sentence
*
* @param newLattitude the new lattitude that was pulled from the complete sentence
*/
	void setLatitude(string newLatitude);

/**
*	Sets the speed to the speed found in the complete sentence
*
* @param newSpeed the new speed that was pulled from the complete sentence and converts it to km/h
*/
	void setSpeed(double speed);

/**
*	Sets the bearing to the bearing found in the complete sentence
*
* @param newBearing the new bearing that was pulled from the complete sentence
*/
	void setBearing(double newBearing);

/**
*	Sets the date to the date found in the complete sentence
*
* @param newDate the new date that was pulled from the complete sentence
*/
	void setDate(string newDate);

/**
*	Sets the magneticVar to the magneticVar found in the complete sentence
*
* @param newMagneticVar the new magneticVar that was pulled from the complete sentence
*/
	void setMagneticVar(string newMagneticVar);

/**
*	Sets the checksum to the checksum found in the complete sentence
*
* @param newChecksum the new checksum that was pulled from the complete sentence
*/
	void setChecksum(string newChecksum);

	/**
*	Resets all values to empty strings so to avoid querying outdated data
*
*/
	void resetValues();
	
};

#endif


