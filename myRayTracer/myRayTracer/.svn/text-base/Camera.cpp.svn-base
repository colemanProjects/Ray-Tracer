#include "Camera.h"
#include <math.h>

Camera::Camera(void)
{
	focalLength = 10.0;

	//start the camera position at (0,-1,1), it has a nice view of (0,0,0)
	startLocation.push_back(0);
	startLocation.push_back(0);
	startLocation.push_back(0); 

	//default focal point is  (0,0,0)
	focalPoint.push_back(0.0);
	focalPoint.push_back(0.0);
	focalPoint.push_back(0.0);
}

vector<float> Camera::getLocation()
{
	return startLocation;
}
vector<float> Camera::getFocalPoint()
{
	return focalPoint;
}

void Camera::setLocation(float x, float y, float z)
{
	startLocation[0] = x;
	startLocation[1] = y;
	startLocation[2] = z;
}

void Camera::changeDirection(float x , float y, float z)
{
	float sum = 1;//abs(x) + abs(y) + abs(z); 
	focalPoint[0] = x/sum;
	focalPoint[1] = y/sum;
	focalPoint[2] = z/sum;
}
