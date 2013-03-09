#include "LightSource.h"
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

LightSource::LightSource(void)
{
	//light source starts a little above and to the right of the default position (0,0,0)
	location.push_back(1);
	location.push_back(-1);
	location.push_back(2);

	intensity = 50.0;
	area = new Box(); //4x4 ceiling light
	area->setMaterialProperty(FLUORESCENT);
}

LightSource::LightSource(float x, float y, float z)
{
	location.push_back(x);
	location.push_back(y);
	location.push_back(z);
	intensity = 50.0;
	area = new Box(); //4x4 ceiling light
	area->setMaterialProperty(FLUORESCENT);
}

LightSource::~LightSource(void)
{
}

void LightSource::changeLocation(float x, float y, float z)
{
	float leftToRight = 0;
	float frontToBack = 1;
	float bottomToTop = 2;
	
	location[leftToRight] = x;
	location[frontToBack] = y;
	location[bottomToTop] = z; 
}

vector<float> LightSource::getlocation() const
{
	return location; 
}

void LightSource::setArea(float width, float length, float height)
{
	vector<float> pointSource = area->getFrontLowerLeftCorner(); 
	float x = pointSource[0];
	float y = pointSource[1];
	float z = pointSource[2];
	
	//fill the rectangular area around the point light source 
	area->setDimensions(width, length,height,DOWN);
	if(area->getOrientation() == FLAT)			area->setFrontLowerLeftCorner(x - width/2, y - length/2, z);
	else if(area->getOrientation() == UPRIGHT)	area->setFrontLowerLeftCorner(x - width/2, y, z - height/2);
	else if(area->getOrientation() == SIDEWAYS)	area->setFrontLowerLeftCorner(x,  y - length/2, z - height/2);
}


const vector<vector<float> > LightSource::getRandomSample(int rows, int columns)
{
	//cout<<"RAY!!!!!!!!!!!!!!!!!"<<endl;
	vector<vector<float> > points; 
	//three cases:
	//the area source can be flat horizontally, having area in the y plane only (i.e orientedd like the floor or ceiling)
	orientation lightOrientation = area->getOrientation();
	srand(time(NULL));

	vector<float> dimensions = area->getDimensions(); 
	float width = dimensions[0];
	float length = dimensions[1]; 
	float height = dimensions[2]; 

	vector<float> startPoint = area->getFrontLowerLeftCorner(); 
	float x = startPoint[0];
	float y = startPoint[1];
	float z = startPoint[2];

	if(lightOrientation == FLAT)
	{
		for(int i = 0; i < columns; i++){
			for(int j = 0; j < rows; j++)
			{
				//here rows correspond to the movement in the y direction
				//columns correspond to movement in the x direction

				//find random number within column i and row j
				float boxWidth = ((float)width)/columns;
				float boxLength = ((float)length)/rows;
				float xCoord = x + boxWidth*(i + (float)(rand()%100)/100);
				float yCoord = y + boxLength*(j + (float)(rand()%100)/100);
				float zCoord = z; 
				vector<float> coords;
				coords.push_back(xCoord);
				coords.push_back(yCoord);
				coords.push_back(zCoord);

				points.push_back(coords);
				//cout<<"coords = ("<<xCoord<<","<<yCoord<<")";
				//cout<<"    "<<"x = "<<x<<" width = "<<width<<"  boxWidth = "<<boxWidth<<endl;
				//cout<<"coords = ("<<xCoord<<","<<yCoord<<")";
				//cout<<"    "<<"y = "<<y<<" length = "<<length<<"  boxLength = "<<boxLength<<endl;
			}
		}
	}
	//it could be oriented like a left or right wall
	else if(lightOrientation == UPRIGHT)
	{
		for(int i = 0; i < columns; i++){
			for(int j = 0; j < rows; j++)
			{


			}
		}
	}
	//or it could be oriented like a back or front wall
	else if(lightOrientation == SIDEWAYS)
	{
		for(int i = 0; i < columns; i++){
			for(int j = 0; j < rows; j++)
			{


			}
		}
	}
	
	return points;
}
