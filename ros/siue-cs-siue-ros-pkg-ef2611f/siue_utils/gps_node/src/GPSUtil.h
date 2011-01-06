/** @author Andy Zeisler
 *  @author Nate Roney
 * copyright 2010
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
 *
 * A collection of GPS utilities useful for navigation
 */
 
#ifndef GPSUTIL_
#define GPSUTIL_

#include "NRGPSPoint.h"
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include "Defines.h"

using namespace std;


class GPSUtil
{

	private:
	
	int NUM_OFFROAD_WAYPOINTS;
	
	//Degrees to radians
	double degToRad(double val)
	{
		return val * (PI/180);
	}

	//Radians to degrees
	double radToDeg(double val)
	{
		return val * (180/PI);
	}


	public:
	
	


	//Calculates the angle (azimuth, initial bearing, whatever you prefer to call it) from 
	// 'current' to 'goal' and returns that value. 
	double angleBetween(NRGPSPoint current, NRGPSPoint goal)
	{
		double latCur = degToRad(current.latDecimalDegrees());
		double lonCur = degToRad(current.lonDecimalDegrees());
		double latGoal = degToRad(goal.latDecimalDegrees());
		double lonGoal = degToRad(goal.lonDecimalDegrees());
		
		double bearingRaw = radToDeg( fmod(atan2(sin(lonGoal-lonCur)*cos(latGoal),
		       cos(latCur)*sin(latGoal)-sin(latCur)*cos(latGoal)*cos(lonGoal-lonCur)),
		       2*PI) );
		       
		double initialBearing = fmod( (bearingRaw+360), 360);
	
		return initialBearing;
	}

	//Calculates the distance between 'current' and 'goal' and returns that value in feet.
	double distanceBetween(NRGPSPoint current, NRGPSPoint goal)
	{

		//formula adapted from : http://www.meridianworlddata.com/Distance-Calculation.asp

		double long1 = degToRad(current.lonDecimalDegrees());
		double long2 = degToRad(goal.lonDecimalDegrees());
		double lat1 = degToRad(current.latDecimalDegrees());
		double lat2 = degToRad(goal.latDecimalDegrees());

		double t1 = sin(lat1) * sin(lat2);
		double t2 = cos(lat1) * cos(lat2);
		double t3 = cos(long2 - long1);
		double t4 = t2*t3;
		double t5 = t1 + t4;
		double t6 = acos(t5);
		double d = (6378.8 * t6) * 3280;//converts from kilometers to feet
		
		d = d/5280;
	
		return d;

	}

	void readWayPoints(list<NRGPSPoint> &waypoints, string filename)
	{
	
		NUM_OFFROAD_WAYPOINTS = 0;
		NRGPSPoint tempPoint;
		int offRoadPoints = 0;
	
		ifstream fin;
		int counter = 0;

		string temp;
	
		fin.open(filename.c_str());
	
		while (fin.fail())
		{
			cout << "Error opening file:" << endl;
			cout << "Enter filename:";
			cin >> filename;

			fin.open(filename.c_str());

		}

		//file format
		//<number>
		//<type>  (P = Path, O = Offroad)
		//<lat>
		//<lon>
		//<separator>

		int id;
		char type;
		double lat;
		double lon;
		istringstream iss;
			
		while (!fin.eof() )
		{
			//<number>
			getline(fin, temp);

			//if we read this in and it's a blank line, then we're at the end of the file but there's blank lines.
			if (temp == "")
			{
				break;
			}

			iss.clear();
			iss.str(temp);
			iss >> id;
			tempPoint.setId(id);
		
			//<type>
			getline(fin, temp);
			iss.clear();
			iss.str(temp);
			iss >> type;
			tempPoint.setType(type);
			if (type == 'O' || type == 'o')
			{
				offRoadPoints++;
			}

			//<lat>
			getline(fin, temp);
			iss.clear();
			iss.str(temp);
			iss >> lat;
			tempPoint.convertLatitude(lat);

			//<lon>
			getline(fin, temp);
			iss.clear();
			iss.str(temp);
			iss >> lon;
			tempPoint.convertLongitude(lon);

			//<separator>
			getline(fin, temp);
			//empty space
			getline(fin, temp);

			waypoints.push_back(tempPoint);

			counter++;
			temp = "";


		}

		NUM_OFFROAD_WAYPOINTS = offRoadPoints;
	}

	int getNumOffroadWaypoints()
	{
		return NUM_OFFROAD_WAYPOINTS;
	}

}GPSUTIL;
#endif
