#pragma once
#include <vector>
#include "Ray.h"
#define PI (3.141592653589793)

namespace vectorMath{

	vector<float> cross(vector<float> vect1, vector<float> vect2);
	vector<float> calculateNormal(vector<float> point);
	
	vector<float> plus(vector<float> vect1, vector<float> vect2);
	vector<int>	  plus(vector<int> vect1, vector<int> vect2);
	vector<float> scalarMultiply(vector<float> vect1, float scalar);
	vector<int> scalarMultiply(vector<int> vect1, float scalar);

	vector<float> minus(vector<float> vect1, vector<float> vect2);
	float dot(vector<float> vect1, vector<float> vect2); 
	float magnitude( vector<float> myVect);
}

