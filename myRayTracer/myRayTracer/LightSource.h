#pragma once
#include <vector>
#include "Box.h"

using namespace std;
class LightSource
{
private:
	vector<float> location;
public:
	float intensity;
	Box* area; 
	LightSource(void);
	LightSource(float x, float y, float z);
	~LightSource(void);

	void changeLocation(float x, float y, float z);
	vector<float> getlocation() const;
	void setArea(float width, float length, float height);
	vector<float> getArea();
	const vector<vector<float>> getRandomSample(int rows, int columns);
	
};

