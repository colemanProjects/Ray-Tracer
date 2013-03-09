#pragma once
#include <vector>

using namespace std;
class Ray
{
private:
	
public:
	int idNumber; 
	float albedo;
	float magnitude;
	vector<float> direction;
	vector<float> startPoint;
	Ray(float startingPointX, float startingPointY, float startingPointZ, float directionX, float directionY, float directionZ);
	~Ray(void);
};

