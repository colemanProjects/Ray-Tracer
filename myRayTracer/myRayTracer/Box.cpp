#include "Box.h"
#include "Ray.h"
#include <iostream>
using namespace std;
Box::Box(void)
{
	length = width = height = 1.0;/* default box is 1x1x1 */
}

Box::Box(int red, int green, int blue, material reflectionProperty, direction directionToFace, vector<float> startPoint):Shape(red,green,blue, reflectionProperty, startPoint)
{
	facing = directionToFace; 
}

Box::~Box(void)
{
}

vector<float> Box::getDimensions()
{
	vector<float> myDimensions;
	myDimensions.push_back(width);
	myDimensions.push_back(length); 
	myDimensions.push_back(height);
	return myDimensions;
}

orientation Box::getOrientation()
{

	return boxOrientation;
}

void Box::setOrientation(orientation newOrientation)
{
	boxOrientation = newOrientation;
}

void Box::setDimensions(float newWidth, float newLength, float newHeight, direction directionToFace)
{
	//error checking: don't allow negative lengths
	width = abs(newWidth);
	length = abs(newLength);
	height = abs(newHeight);

	facing = directionToFace; //I'm leaving it up to the user to choose the correct direction to face

	//set orientation of the box or plane 
	if(width <= 0.0)		this->setOrientation(SIDEWAYS);
	else if(length <= 0.0)	this->setOrientation(UPRIGHT);
	else if(height <= 0.0)	this->setOrientation(FLAT);
	else					this->setOrientation(BOX);
}

vector<float> Box::getFrontLowerLeftCorner()
{
	return startPosition;
}

void Box::setFrontLowerLeftCorner(float x, float y, float z)
{
	setStartPosition(x,y,z);
}

vector<float> Box::calculateNormal(vector<float> point)
{
	//calculate the normal of a plane
	//here we only use planes that are int the xy, xz, or zy plane
	vector<float> normal; 
	float x = getDimensions()[0];
	float y = getDimensions()[1];
	float z = getDimensions()[2];
	//if the plane is in the xy plane ( i.e z = 0; flat on the ground)
	if(z == 0)
	{
		if(facing == UP)
		{
			normal.push_back(0);
			normal.push_back(0);
			normal.push_back(1);
		}
		else //DOWN
		{
			normal.push_back(0);
			normal.push_back(0);
			normal.push_back(-1);
		}
	}
	else if( y == 0) // it's standing upright in the xz plane
	{
		if(facing == FORWARD)
		{
			normal.push_back(0);
			normal.push_back(1);
			normal.push_back(0);
		}
		else //BACKWARD
		{
			normal.push_back(0);
			normal.push_back(-1);
			normal.push_back(0);
		}
	}
	else			// it's on the side in the zy plane
	{
		if(facing == RIGHT)
		{
			normal.push_back(1);
			normal.push_back(0);
			normal.push_back(0);
		}
		else //LEFT
		{
			normal.push_back(-1);
			normal.push_back(0);
			normal.push_back(0);
		}
	}
	return normal;
}

bool Box::closestIntersection(const Ray& myRay, float* closestTvalue, float restraintTvalue, clock_t startTime)
{
	//we'll use the formula for a ray R = S +tv
	//S = start Point; t = distance along the line; v = direction of the ray;
	//formula for the plane = (p - p0) . n = 0;
	// where p0 is a point on the plane; n is the normal to the plane;
	//we wanna solve for t by substituting our ray for p
	//(S + tv -p0) . n = 0
	//t =  - ((p0 -S) . n )/ (v . n)

	if(DEBUG) cout<<" restraintTvalue = "<<restraintTvalue<<"  ;"; 
	using namespace vectorMath; 
	vector<float> S = myRay.startPoint;
	vector<float> v = myRay.direction;
	vector<float> n = calculateNormal(this->getFrontLowerLeftCorner());
	vector<float> p0 = this->getFrontLowerLeftCorner();
	float t = -1;

	float intermediate = dot(vectorMath::minus(p0,S),n);

	// the ray doesn't intersect if the direction is perpendicular to the plane. (i.e v . n = 0)
	float denominator =  dot(v,n);
	if(denominator == 0) {
		if(DEBUG) cout<<"missed "<<name<<endl; 
		return false;
	}
	else
	{
		t = (dot(vectorMath::minus(p0,S),n) / dot(v,n));
	
		vector<float> interSectPoint = vectorMath::vectorAdd(S,scalarMultiply(v,t));
		int x = 0;
		int y = 1;
		int z = 2;
		vector<float> startPostion = getFrontLowerLeftCorner();
	
		float myXLowerBound = startPosition[x];
		float myXUpperBound = myXLowerBound + this->width;
		float myYLowerBound = startPosition[y];
		float myYUpperBound = myYLowerBound + this->length;
		float myZLowerBound = startPosition[z];
		float myZUpperBound = myZLowerBound + this->height;

		if(t < 0)//we don't want it to accept tvalues that are negative. that means that it's behind the ray startPoint
		{
			
			if(DEBUG) cout<<"was behind the ray; ";
			if(DEBUG) cout<<"missed "<<name<<endl; 
			return false; 
		}

	

		//if the intersection point falls outside of the dimensions then it doesn't intersect
		if(n[x] == 1) //if it's in the yz plane; on the side
		{
			//check whether the y and z values are in bounds
			if(interSectPoint[z] < myZLowerBound || interSectPoint[z] > myZUpperBound)
			{
				if(DEBUG) cout<<"missed "<<name<<endl; 
				return false;
			}
			if(interSectPoint[y] < myYLowerBound || interSectPoint[y] > myYUpperBound)
			{
					if(DEBUG) cout<<"missed "<<name<<endl; 
					return false;
			}
		}
		else if(n[y] == 1) //it's in the xz plane; standing straight up
		{
			//check whether the x and z values are in bounds
			if(interSectPoint[x] < myXLowerBound || interSectPoint[x] > myXUpperBound)
			{
				if(DEBUG) cout<<"missed "<<name<<endl; 
				return false;
			}
			if(interSectPoint[z] < myZLowerBound || interSectPoint[z] > myZUpperBound)
			{
				if(DEBUG) cout<<"missed "<<name<<endl; 
				return false;
			}
		}
		else //it's in the xy plane; on the ground 
		{
			//check whether the x and y values are in bounds
			if(!(interSectPoint[x] > myXLowerBound && interSectPoint[x] < myXUpperBound))
			{
				if(DEBUG) cout<<"missed "<<name<<endl; 
				return false;
			}
			if(!(interSectPoint[y] > myYLowerBound && interSectPoint[y] < myYUpperBound))
			{
				if(DEBUG) cout<<"missed "<<name<<endl; 
				return false;
			}
		}

		//if the intersection is further along the ray than the restraint value
		if(DEBUG) cout<<";   myTvalue = "<<t;
		if(t > restraintTvalue || t <= 0.001)
		{
			if(DEBUG) cout<<" "<<name<<" intersection is pass restraint value"<<endl;
			*closestTvalue =  -1; 
			return false; 
		}

		//otherwise it's in bounds and we can set the t value
		if(DEBUG) cout<<";   intersects "<< name<<endl;
		*closestTvalue = t; 
		return true;
	}
}
/*returns the unit normal at the given point and time*/
vector<float> Box::getNormal(const vector<float> &point, clock_t startTime)
{
	vector<float> normal =  calculateNormal(point);
	return vectorMath::scalarMultiply(normal, 1.0/((float) vectorMath::magnitude(normal))); //(normal) * 1/||normal|| 
}

void Box::move(clock_t sceneStartTime)
{
	//only move in x direction for now
	//we basically want to scale the movement to oscillate between xLeftBound and xRightBound
	//change position to xLeftBound + .5(rightBound - leftBound) + (.5(rightBound - leftBound))*sin(theta)
	//sin should be 0 at 0 and 180degrees, this means that it should start in the middle and go from left to right
	//theta then can be found by CurrentTime % 360. in degrees and then multiply by 2pi/360 to convert to radians
	if(mobile)
	{
		float range = xRightBound - xLeftBound; 
		float xStartPosition = xLeftBound + .5 * (range);

		clock_t endTime(clock());
		int currTime = endTime - sceneStartTime; 
		int thetaInDegrees  = currTime % 360;
		float thetaInRadians = thetaInDegrees * (PI/180);

		float finalXPosition = xStartPosition + sin(thetaInRadians)*(range * .5);
		//setCenter(finalXPosition, startPosition[1], startPosition[2]);
	}
}

string Box::identifySelf()
{
	return "Box"; 
}
