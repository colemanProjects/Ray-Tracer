#include "Ray.h"

Ray::Ray(float startingPointX, float startingPointY, float startingPointZ, float directionX, float directionY, float directionZ)
{
	//directionX, directionY, and directionZ are all passed in component-wise as (startpoint - endpoint)
	//initalize 
	startPoint.push_back(startingPointX);
	startPoint.push_back(startingPointY);
	startPoint.push_back(startingPointZ);

	albedo = 1.0;

	//make it a unit direction vector
	magnitude = sqrt((directionX * directionX) + (directionY * directionY) + (directionZ * directionZ));
	direction.push_back(directionX/magnitude);
	direction.push_back(directionY/magnitude);
	direction.push_back(directionZ/magnitude);
}


Ray::~Ray(void)
{
}
