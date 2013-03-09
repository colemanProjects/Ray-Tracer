#include "Shape.h"
#include <iostream>


Shape::Shape(void)
{
	// this gets called before any of the inherited constructors also
	
	/* default color is red */
	int red = 25;
	int green = 0;
	int blue = 0;
	color.push_back(red);
	color.push_back(green);
	color.push_back(blue);
	
	colorInShade.push_back(red/2); 
	colorInShade.push_back(green/2);
	colorInShade.push_back(blue/2);

	albedo = 0.9;
	mobile = false; 

	// bounds
	xLeftBound = imax;	xRightBound = imax;
	yLeftBound = imax;	yRightBound = imax;
	zLeftBound = imax;	zRightBound = imax;

	//default reflectiveness is 0 for diffuse surfaces
	reflectiveProperty = DIFFUSE;

	//default start location is at (x,y,z) = (left to right, front to back, bottom to top) = (0,0,0)
	startPosition.push_back(0);
	startPosition.push_back(0);
	startPosition.push_back(0);

	name = "generic Shape"; 
}

Shape::Shape(int red, int green, int blue, material reflectionProperty,vector<float> startPoint)
{
	albedo = 0.9;
	color.push_back(red);
	color.push_back(green);
	color.push_back(blue);

	colorInShade.push_back(red/2); 
	colorInShade.push_back(green/2);
	colorInShade.push_back(blue/2);

	reflectiveProperty = reflectionProperty;
	startPosition.push_back(startPoint[0]);
	startPosition.push_back(startPoint[1]);
	startPosition.push_back(startPoint[2]);

	// bounds
	xLeftBound = imax;	xRightBound = imax;
	yLeftBound = imax;	yRightBound = imax;
	zLeftBound = imax;	zRightBound = imax;

	mobile = false;	
}

Shape::~Shape(void)
{
}


void Shape::setStartPosition(float x, float y, float z)
{
	startPosition[0] = x;
	startPosition[1] = y;
	startPosition[2] = z;
}

vector<int> Shape::getColor()
{
	return color;
}

void Shape::setColor(int red, int green, int blue)
{
	color.clear();
	color.push_back(red);
	color.push_back(green);
	color.push_back(blue);

	colorInShade.clear(); 
	colorInShade.push_back(red/2); 
	colorInShade.push_back(green/2); 
	colorInShade.push_back(blue/2); 
}

void Shape::setMaterialProperty(material myMaterial)
{
	reflectiveProperty = myMaterial; 
}

bool Shape::closestIntersection(const Ray& myRay, float* closestTvalue, float restraintTvalue, clock_t startTime)
{
	return false;
}
enum material Shape::getMaterialProperty()
{
	return reflectiveProperty; 
}

vector<float> Shape::getNormal(const vector<float> &point, clock_t startTime)
{
	return point; 
}

void Shape::setName(string newName)
{
	name = newName; 
}

void Shape::setXBounds(float leftBound, float rightBound)
{
	xLeftBound = leftBound;
	xRightBound = rightBound;
}
void Shape::setYBounds(float leftBound, float rightBound)
{
	yLeftBound = leftBound;
	yRightBound = rightBound;
}
void Shape::setZBounds(float leftBound, float rightBound)
{
	zLeftBound = leftBound;
	zRightBound = rightBound;
}
void Shape::setBounds(float x_LeftBound, float x_RightBound, float y_LeftBound, float y_RightBound, float z_LeftBound, float z_RightBound)
{
	xLeftBound = x_LeftBound;
	xRightBound = x_RightBound;

	yLeftBound = y_LeftBound;
	yRightBound = y_RightBound;

	zLeftBound = z_LeftBound;
	zRightBound = z_RightBound;
}

void Shape::move(clock_t startTime)
{
}

vector<float> Shape::getPosition(clock_t startTime)
{
	if(mobile) move(startTime);

	return startPosition;
}

string Shape::identifySelf()
{
	return "Generic Shape";
}

string Shape::getName()
{
	return name; 
}
