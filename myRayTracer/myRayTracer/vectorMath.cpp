#include "vectorMath.h"

float descriminant(float a, float b, float c, float d)
{
	/*
	|a  b|
	|c  d|    == ad - bc
	*/

	return a*d - b*c; 
}
vector<float> vectorMath::cross(vector<float> a, vector<float> b)
{
	//cross product, axb
	/*
	|a2 a3| i	-	|a1 a3| j	+	|a1 a2|k
	|b2 b3|			|b1 b3|			|b1 b2|
	*/
	float i = descriminant(a[1], a[2], b[1], b[2]); 
	float j = descriminant(a[0], a[2], b[0], b[2]);
	float k = descriminant(a[0], a[1], b[0], b[1]);
	vector<float> result; 
	result.push_back(i);
	result.push_back(-j);
	result.push_back(k);
	return result; 
}

float vectorMath::dot(vector<float> vect1, vector<float> vect2)
{
	return (vect1[0] * vect2[0]) + (vect1[1] * vect2[1]) + (vect1[2] * vect2[2]);
}

float vectorMath::magnitude(vector<float> myVect)
{
	float sum = sqrt(myVect[0]*myVect[0] + myVect[1]*myVect[1] + myVect[2]*myVect[2]);
	return sum;
}

vector<float> vectorMath::plus(vector<float> vect1, vector<float> vect2)
{
	vector<float> result;
	result.push_back(vect1[0] + vect2[0]);
	result.push_back(vect1[1] + vect2[1]);
	result.push_back(vect1[2] + vect2[2]);
	
	return result;
}

vector<int> vectorMath::plus(vector<int> vect1, vector<int> vect2)
{
	vector<int> result;
	result.push_back(vect1[0] + vect2[0]);
	result.push_back(vect1[1] + vect2[1]);
	result.push_back(vect1[2] + vect2[2]);
	
	return result;
}

vector<float> vectorMath::minus(vector<float> vect1, vector<float> vect2)
{
	vector<float> result;
	result.push_back(vect1[0] - vect2[0]);
	result.push_back(vect1[1] - vect2[1]);
	result.push_back(vect1[2] - vect2[2]);
	
	return result;
}

vector<float> vectorMath::scalarMultiply(vector<float> vect1, float scalar)
{
	vector<float> result;
	result.push_back(vect1[0]  * scalar);
	result.push_back(vect1[1]  * scalar);
	result.push_back(vect1[2]  * scalar);
	
	return result;
}

vector<int> vectorMath::scalarMultiply(vector<int> vect1, float scalar)
{

	vector<int> result;
	result.push_back((float)vect1[0]  * scalar);
	result.push_back((float)vect1[1]  * scalar);
	result.push_back((float)vect1[2]  * scalar);
	
	return result;
}