/**
* This class is really just a plane class that has 2 dimension
* It has the capability of being oriented in all 3 dimenis
*
**/
#pragma once
#include <vector>
#include "Shape.h"
#include "Ray.h"
using namespace std;
enum orientation{
	FLAT,		//like a ceiling or a floor
	UPRIGHT,	//like a back wall or front wall
	SIDEWAYS,	//like a left wall or right wall
	BOX			//a box with volume  
};

enum direction{
	RIGHT,		//in the positive x direction
	LEFT,		//in the negative x direction
	UP,			//in the positive z direction
	DOWN,		//in the negative z direction
	FORWARD,	//away from the Screen. in the positive y direction
	BACK		//towards the Screen. in the negative y direction
};

class Box: public Shape
{
private: 
	float width; //grows in the x direction
	float length;// y direction
	float height;// z direction
	orientation boxOrientation;

public:
	direction facing;
	Box(void);
	Box(int red, int green, int blue,material reflectionProperty, direction face, vector<float> startPoint);
	~Box(void);
	
	vector<float> getDimensions();		//returns <width, length, height>
	void setDimensions(float width, float length, float height, direction face);
	vector<float> getFrontLowerLeftCorner();
	vector<float> calculateNormal(vector<float> point);
	orientation getOrientation();
	void setOrientation(orientation newOrientation); 

	/*
	 *  x = left to right
	 *  y = front to back
	 *  z = bottom to top
	 */
	void setFrontLowerLeftCorner(float x, float y, float z);
	bool closestIntersection(const Ray& myRay, float* closestTvalue,float restraintTvalue, clock_t startTime);
	vector<float> getNormal(const vector<float> &point, clock_t startTime);
	void move(clock_t sceneStartTime);
	string identifySelf();
};

