#pragma once
/**
* The Scene Class sets up an environment that contains various objects. 
* A Scene is composed of a camera, a vector of light sources, and a vector of objects
**/

#include "Shape.h"
#include <vector>
#include "LightSource.h"
#include "Camera.h"
#include "Ray.h"
#include <limits.h>

using namespace std;
class Scene
{
private:
	vector<int> backgroundColor;
public:
	Camera* myCamera;
	vector<Shape*> myObjects;
	vector<LightSource> myLightSources;

	Scene(void);

	void placeShape(Shape* newShape);
	void placeLightSource(const LightSource &newLight);
	void placeLightSource(int x, int y, int z);
	void setBackgroundColor(int red, int green, int blue);
	
	//functions that don't alter this object
	vector<float> getCameraPosition() const;
	vector<int> getBackgroundColor() const;
};

