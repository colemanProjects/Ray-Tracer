#pragma once
#include <vector>
#include "Ray.h"
#include "vectorMath.h"
#include <math.h>
#include <string>
#include <sys/timeb.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

#define DEBUG 0
using namespace std;

enum material{
 DIFFUSE,		/*light is averaged over all incoming*/
 SPECULAR,		/* light reflects all of what comes in*/
 TRANSMITTED,	/*light reflects some of what comes in, and transmits some*/
 FLUORESCENT	/*light is emitted from the surface*/
}; 

class Shape
{
protected:
	vector<float> startPosition;
	material reflectiveProperty; // can be 0, 1, 2, or 3. diffuse, specular, reflective, flourescent respectively
	string name;
	float xLeftBound, xRightBound; //bounds on movement, the shape will oscillate between these bounds 
	float yLeftBound, yRightBound;
	float zLeftBound, zRightBound;


public:
	bool mobile; //true if shape can move, false otherwise
	float albedo;
	vector<int> color; // color stored as <red, green, blue>; white =  <255,255,255>
	vector<int> colorInShade;
	Shape(void);
	Shape(int red, int green, int blue,material reflectionProperty,vector<float> startPoint);
	~Shape(void);
	
	enum material getMaterialProperty(); 
	
	void setStartPosition(float x, float y, float z);
	void setColor(int red, int green, int blue);
	void setName(string newName); 
	void setMaterialProperty(material myMaterial); 
	
	void setXBounds(float leftBound, float rightBound);
	void setYBounds(float leftBound, float rightBound);
	void setZBounds(float leftBound, float rightBound);
	void setBounds(float x_LeftBound, float x_RightBound, float y_LeftBound, float y_RightBound, float z_LeftBound, float z_RightBound);
	
	virtual string identifySelf();
	
	vector<float> getPosition(clock_t startTime); 
	vector<int> getColor();
	string getName(); 


	virtual vector<float> getNormal(const vector<float> &point, clock_t startTime);
	virtual bool closestIntersection(const Ray& myRay, float* closestTvalue, float restraintTvalue, clock_t startTime);
	virtual void move(clock_t sceneStartTime); 

};

