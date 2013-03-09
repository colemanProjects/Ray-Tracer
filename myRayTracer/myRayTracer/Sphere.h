#pragma once
#include "Shape.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "Ray.h"

using namespace std;

class Sphere: public Shape
{
private:
	typedef Shape super;
	float radius;

public:
	Sphere(void);
	~Sphere(void);
	Sphere(int red, int green, int blue,material reflectionProperty, vector<float> startPoint, float radius);

	float getRadius();
	vector<float> getCenter(clock_t startTime);
	
	/*
	 *  x = left to right
	 *  y = front to back
	 *  z = bottom to top
	 */
	void setCenter(float x, float y, float z);
	void setRadius(float r);
	vector<float> calculateNormal(vector<float> point);
	
	// const keyword is saying that the method won't copy the ray but also won't change it
	bool closestIntersection(const Ray& myRay, float* closestTvalue, float restraintTvalue, clock_t startTime);
	vector<float> getNormal(const vector<float> &point, clock_t startTime);
	void move(clock_t sceneStartTime);
	string identifySelf();
};

