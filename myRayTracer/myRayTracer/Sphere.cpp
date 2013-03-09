#include "Sphere.h"
#include "vectorMath.h"

Sphere::Sphere(void)
{
	radius = 1; /*unit sphere*/
}

Sphere::Sphere(int red, int green, int blue,material reflectionProperty, vector<float> startPoint, float newRadius):Shape(red,green,blue,reflectionProperty,startPoint)
{
	radius = newRadius;
}

Sphere::~Sphere(void)
{
}

float Sphere::getRadius()
{
	return radius;
}

vector<float> Sphere::getCenter(clock_t startTime)
{
	return getPosition(startTime);
}

void Sphere::setCenter(float x, float y, float z)
{
	setStartPosition(x,y,z);
}
	
void Sphere::setRadius(float r)
{
	radius = r;
}

/**
* returns the closest Intersection point between the ray and the sphere
* the Ray myRay is the ray from a start point in a certain direction
* closestTvalue will be set to the smallest distance along the ray that intersects with this shape
* restraintTvalue sets a bound on how far along the ray we consider a valid tvalue/intersection point
**/
bool Sphere::closestIntersection(const Ray& myRay, float* closestTvalue, float restraintTvalue, clock_t startTime)
{
	//equation for a circle: (x - c)^T (x - c) = r^2
	//here x is the vector we want to intersect with the circle
	//x = p + tv 
	//where p is the staring point
	//t is the distance along the line
	//v is the unit vector that denotes direction
	// we want to solve for t and get the one closest to the starting point
	// we just solve (p - c + tv)^T (p - c + tv) = r^2
	if(DEBUG) cout<<" restraintTvalue = "<<restraintTvalue<<"  ;"; 
	vector<float> p = myRay.startPoint;
	vector<float> v = myRay.direction;
	vector<float> c = getCenter(startTime);
	int r = getRadius();

	using namespace vectorMath; 
    float A = dot(v,v);
    float B = dot(	scalarMultiply(	(vectorMath::minus(p,c)) , 2), v);		// 2* (p - c) (v)
    float C = dot( vectorMath::minus(p,c)  ,  vectorMath::minus(p,c)  )	- (r * r);	//(p -c) (p - c) - r^2

	//if the descriminant is zero then we know that there aren't any real solutions
	float descriminant = B * B - 4 * A * C;
	float sqrtDescriminant =  sqrt(descriminant);//Square root of  B^2 - 4 * A * C
	
	if(descriminant <= 0)
	{
		if(DEBUG) cout<<"missed "<<name<<endl; 
		return false;
	}

	//otherwise we know that there are two solutions t0 and t1
	//for numeric precision as stated in http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
	//we calculate q first
	float q; 
	if(B < 0)
		q =  (-B + sqrtDescriminant) / 2;
	else
		q = (-B - sqrtDescriminant)/2;
	
	float t0 = q / A;
	float t1 = C / q; 

	// make sure t0 is smaller than t1, this next if statement is from the website
    if (t0 > t1)
    {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
	//TODO: only set the variable "closestTvalue" if the tvalue is actually valid. i.e if it is too close(tvalue < minTvalue) then disregard that tvalue and go on with the calculation
	float minTvalue = 0.1;

	//both values being negative means we're behind the object
	if( t0 < minTvalue && t1 < minTvalue)
	{
		if(DEBUG) cout<<"ray started behind the object, doesn't intersect"<<endl;
		return false; 
	}
	//t0 is closest to the start point of the ray, but it could be too close
	else if( t0 < minTvalue)
	{
		//then we know that t1 is the closest non negative intersection.
		*closestTvalue = t1;
	}
	else
	{
		*closestTvalue = t0;
	}
	if(DEBUG) cout<<";   myTvalue = "<<*closestTvalue;
	
	//take the distance restraint into account
	//restraintTvalue is the largest Tvalue allowed
	//minTvalue is the smallest allowed Tvalue
	if(*closestTvalue > restraintTvalue || *closestTvalue < minTvalue)
	{ 
		if(DEBUG) cout<<" "<<name<<"intersection is pass restraint value"<<endl;
		*closestTvalue = -1; 
		return false; 
	}
	if(DEBUG) cout<<";   intersects "<< name<<endl; 
	return true; 
}

vector<float> Sphere::getNormal(const vector<float> &point, clock_t startTime)
{
	//returns the unit normal to a point on the surface
	vector<float> normal; 
	int x = 0; 
	int y = 1; 
	int z = 2; 
	normal.push_back(point[x] - this->getCenter(startTime)[x]); 
	normal.push_back(point[y] - this->getCenter(startTime)[y]); 
	normal.push_back(point[z] - this->getCenter(startTime)[z]);

	return vectorMath::scalarMultiply(normal, 1.0/((float) vectorMath::magnitude(normal))); //(normal) * 1/||normal|| 
}

void Sphere::move(clock_t sceneStartTime)
{
	//only move in x direction for now
	//we basically want to scale the movement to oscillate between xLeftBound and xRightBound
	//change position to xLeftBound + .5(rightBound - leftBound) + (.5(rightBound - leftBound))*sin(theta)
	//sin should be 0 at 0 and 180degrees, this means that it should start in the middle and go from left to right
	//theta then can be found by CurrentTime % 360. in degrees and then multiply by 2pi/360 to convert to radians

	float range = xRightBound - xLeftBound; 
	float xStartPosition = xLeftBound + .5 * (range);

	clock_t endTime(clock());
	int currTime = (endTime - sceneStartTime)/150; // in the of miliseconds
	int thetaInDegrees  = (currTime) % 360;
	float thetaInRadians = thetaInDegrees * (PI/180);

	float finalXPosition = xStartPosition + sin(thetaInRadians)*(range * .5);
	setCenter(finalXPosition, startPosition[1], startPosition[2]);
}

string Sphere::identifySelf()
{
	return "Sphere"; 
}