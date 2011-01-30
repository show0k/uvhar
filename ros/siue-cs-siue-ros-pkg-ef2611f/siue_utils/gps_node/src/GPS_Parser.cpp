


#include "GPS_Parser.h"
#ifndef GPS_PARSER_CPP
#define GPS_PARSER_CPP
#include<string>
#include<time.h>
#include<stdlib.h>

using namespace std;


GPS_Parser::GPS_Parser()
{
	time = "";
	status = 0;
	longitude = "";
	latitude = "";
	speed = 0.0;
	bearing = 0.0;
	date = "";
	magneticVar = "";
	checksum = "";
}


bool GPS_Parser::parse(string sentence)
{

	resetValues();

	char character;
	int index = 0;
	int myLength = sentence.length();
	string parsedSentence = "";
	int newLength=0;
	int commaCount = 0;
	int j = 0;
	bool foundComplete = false;
	string ourType = "$GPRMC";
	bool found = false;
	
	for (int i = 0; i < myLength; i++)
	{
		character = sentence[i];

		//we found the beginning of a sentence
		if (character == '$')
		{
			//calculate the new length starting from the beginning of the sentence
			newLength = sentence.length() - i;
			//store the sentence starting at the new beginning to the new length
			try
			{
				parsedSentence = sentence.substr(i, newLength);
			}
			catch(...)
			{
				return false;
			}

			//the type of sentence we want is $GPRMC so we will get the first 6 chars from the
			//parsed sentence
			string foundType;
			try
			{
				foundType = parsedSentence.substr(0,6);
			}
			catch(...)
			{
				return false;
			}

			if (foundType == ourType)
			{
				found = true;
			}
			else
			{
				//we didn't find the type of sentence we want so don't process it
				found = false;
			}

			if (found)
			{

				foundComplete = false;
				j = 0;
				while(character != '*' && j != parsedSentence.length())
				{
					j++;
				character = parsedSentence[j];
				}
				if (character == '*')
				{
				//j now holds the index of the * which always indicates the checksum
				//grab just the starting point of the checksum and the rest of the sentence
					string temp = "";
					try
					{
						temp = parsedSentence.substr(j, parsedSentence.length());
					}
					catch(...)
					{
						return false;
					}

					//make sure we have the full checksum at the end, which will be 3 characters
					if (temp.length()  >= 3)
					{
						//reset the sentence from the beginnging to the end of the checksum (3 characters)
						try
						{
							parsedSentence = parsedSentence.substr(0, j+3);
						}
						catch(...)
						{
							return false;
						}

						//count the commas
						string character;

						for (int k = 0; k < (int)parsedSentence.length(); k++)
						{
							character = parsedSentence[k];
							if (character == ",")
							{
								commaCount++;
							}
						}

						//make sure we have a full string with only 11 comma's 
						//(12 if the gprmc contains magnetic Var
						if (commaCount == 11 || commaCount == 12)
						{
							foundComplete = true;
						}
						else
						{
							foundComplete = false;
						}
						
						
					}
				}
				
				//we have found a full sentence to parse
				//if (commaCount == 11 || commaCount == 12)
				if (foundComplete)
				{
					foundComplete = separateSentence(parsedSentence);
					if (!foundComplete)
					{
						resetValues();
					}
				}
		
				return foundComplete;
			}

		}

		
	}
	return foundComplete;
}

bool GPS_Parser::separateSentence(string sentence)
{
	try
	{
		double castValue = 0.0;
		int commaLocation;
		commaLocation = sentence.find_first_of(',');

		try
		{
			//pull off the $GPRMC
			string gps_type = sentence.substr(0,commaLocation);
		}
		catch(...)
		{
			return false;
		}

		string subString = "";

		try
		{
			//set a substring that contains all the parts
			subString = sentence.substr(commaLocation + 1, sentence.length() - commaLocation);
		}
		catch(...)
		{
			return false;
		}

		try
		{
			//find the next comma location
			commaLocation = subString.find_first_of(',');
		}
		catch(...)
		{
			return false;
		}

		//---------------------------TIME--------------------------
		string component = modifySubString(commaLocation, subString);
		//time should always be a numeric value, so we try to cast it for simple error checking
		if (!testData(component , 10))
		{
			return false;
		}

		setTime(component);

		//---------------------------STATUS------------------------
		component = modifySubString(commaLocation, subString);

		if (component == "A")
		{
			setStatus(1);
		}
		else
		{
			setStatus(0);
		}
	

		//-------------------------LATITUDE------------------------
		//*****REQUIRES 2 COMMA VALUES******
		component = modifySubString(commaLocation, subString);
		//this call will strip off the heading which is not needed for anything
		//since we will always be in the same hemisphere.
		string temp = modifySubString(commaLocation, subString);
		if (!testData(component, 10))
		{
			return false;
		}

		setLongitude(component);

		//------------------------LONGITUDE-------------------------
		//*****REQUIRES 2 COMMA VALUES******
		component = modifySubString(commaLocation, subString);
		//this call will strip off the heading which is not needed for anything
		//since we will always be in the same hemisphere.
		temp = modifySubString(commaLocation, subString);
		if (!testData(component, 10))
		{
			return false;
		}
		setLatitude(component);

		//-------------------------SPEED---------------------------
		component = modifySubString(commaLocation, subString);
		if (!testData(component, 7))
		{
			return false;
		}
		else
		{
			try
			{
 				castValue = strtod(component.c_str(), NULL);
				double temp = castValue;
				//convert knots to km/h
				castValue = temp * 1.852;
				setSpeed(castValue);
			}
			catch (...)
			{
				return false;
			}

		}

		
		//------------------------BEARING---------------------------
		component = modifySubString(commaLocation, subString);
		if (!testData(component, 7))
		{
			return false;
		}
		else
		{
			//reset castValue
			castValue = 0.0;
			try
			{
 				castValue = strtod(component.c_str(), NULL);
				setBearing(castValue);
			}
			catch (...)
			{
				return false;
			}

		}
		
		//------------------------DATE------------------------------
		component = modifySubString(commaLocation, subString);
		if (!testData(component, 6))
		{
			return false;
		}
		setDate(component);
		
		
		//---------------------------MAGNETIC VARIATION-----------------------------
		component = modifySubString(commaLocation, subString);
		try
		{
			component.append(subString.substr(0,1));
		}
		catch(...)
		{
			return false;
		}
		try
		{
			subString = subString.substr(1,subString.length());
		}
		catch(...)
		{
			return false;
		}


		//the MagneticVar was present so append the additional value to its string
		if (!testData(component, 8))
		{
			return false;
		}
		
		setMagneticVar(component);

		//-------------------------------CHECKSUM-------------------------------------
		component = modifySubString(commaLocation, subString);

		if (component.length() > 3)
		{
			return false;
		}
		setChecksum(component);
	}

	catch (string test)
	{
		return false;
	}
	return true;
	
}

bool GPS_Parser::testData(string data, int maxLength)
{
	if (data == "")
	{
		return true;
	}
	if ((int)data.length() > maxLength)
	{
		return false;
	}
	
	double castValue;
	try
		{
 			castValue = strtod(data.c_str(), NULL);
	
		}
		catch (...)
		{
			return false;
		}

		//maxLength will be 1 for magnetic Variation which will either be there or it won't.
		if (castValue <= 0 && maxLength > 1)
		{
			return false;
		}

		return true;
}
string GPS_Parser::modifySubString(int &commaLocation, string &subString)
{
	string returnString = "";

	try
	{
		returnString = subString.substr(0,commaLocation);
	}
	catch(...)
	{
		return "";
	}

	try
	{
		subString = subString.substr(commaLocation + 1, subString.length() - commaLocation);
	}
	catch(...)
	{
		return "";
	}

	try
	{
		commaLocation = subString.find_first_of(',');
	}

	catch(...)
	{
		return "";
	}

	return returnString;
}


void GPS_Parser::setTime(string newTime)
{
	time = newTime;
}

void GPS_Parser::setStatus(int newStatus)
{
	status = newStatus;
}

void GPS_Parser::setLongitude(string newLongitude)
{
	longitude = newLongitude;
}

void GPS_Parser::setLatitude(string newLatitude)
{
	latitude = newLatitude;
}

void GPS_Parser::setSpeed(double newSpeed)
{
	speed = newSpeed;
}

void GPS_Parser::setBearing(double newBearing)
{
	bearing = newBearing;
}

void GPS_Parser::setDate(string newDate)
{	
	date = newDate;
}

void GPS_Parser::setMagneticVar(string newMagneticVar)
{
	magneticVar = newMagneticVar;
}

void GPS_Parser::setChecksum(string newChecksum)
{
	checksum = newChecksum;
}

string GPS_Parser::getTime()
{
	return time;
}

int GPS_Parser::getStatus()
{
	return status;
}

string GPS_Parser::getLongitude()
{
	return longitude;
}

string GPS_Parser::getLatitude()
{
	return latitude;
}

double GPS_Parser::getSpeed()
{
	return speed;
}

double GPS_Parser::getBearing()
{
	return bearing;
}

string GPS_Parser::getDate()
{
	return date;
}

string GPS_Parser::getMagneticVar()
{
	return magneticVar;
}

string GPS_Parser::getChecksum()
{
	return checksum;
}
void GPS_Parser::resetValues()
{
	time = "";
	status = 0;
	longitude = "";
	latitude = "";
	speed = 0.0;
	bearing = 0.0;
	date = "";
	magneticVar = "";
	checksum = "";
}
#endif
