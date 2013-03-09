#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

using namespace std;

Scene::Scene(void)
{
	myCamera = new Camera();
	
	//default background is black
	backgroundColor.push_back(0);
	backgroundColor.push_back(0);
	backgroundColor.push_back(0);
}

void Scene::placeShape(Shape* newShape)
{
	myObjects.push_back(newShape);
}

void Scene::placeLightSource(const LightSource &newLight)
{
	myLightSources.push_back(newLight);
}

void Scene::placeLightSource(int x , int y, int z)
{
	LightSource* newLight = new LightSource(x,y,z);
	
	myLightSources.push_back(*newLight);
	
}


void Scene::setBackgroundColor(int red, int green, int blue)
{
	backgroundColor[0] = red;
	backgroundColor[1] = green;
	backgroundColor[2] = blue;
}
	
vector<int> Scene::getBackgroundColor() const
{
	return backgroundColor;
}

vector<float> Scene::getCameraPosition() const
{
	return myCamera->getLocation();
}