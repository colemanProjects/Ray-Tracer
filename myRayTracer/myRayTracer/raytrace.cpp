/*
 * This is the main file that renders a scene with lighting via ray tracing calculations
 */
#include "Scene.h"
#include "Box.h"
#include "Sphere.h"
#include "EasyBMP.h"
#include "EasyBMP_BMP.h"
#include "EasyBMP_DataStructures.h"
#include "EasyBMP_VariousBMPutilities.h"
#include "Ray.h"


using namespace std;
enum pruningStyle{
	DEPTH, //number of bounces
	ALBEDO //contribution of light at intersection
};
enum lightingStyle{
 POINTLIGHT,		//point light source
 AREA		//area light sources
 };

bool focal = true; 
pruningStyle pruneOption = DEPTH;
lightingStyle lightOption = POINTLIGHT;
float shutterTime = 8;
clock_t globalStartTime = clock();
time_t startTime, currTime, endTime, totalTime; 

//this variable is used to automatically scale the scene size 
//to get a better looking picture with objects taking up more pixels
float cameraCoefficient = 1000.0; //it actually seems to have no effect at the moment TODO: figure out why this is here
float _maxSceneBoundary  = 1000000; //1,000,000

//image looks better when it is larger. Smallest image size is 1x1(real time), largest meaningful size is 1,000 x 1,000(6+ hours)
int outputImageWidth = 30;
int outputImageHeight = 30;
static int rayNumber = 0;

//creates a ray from startPoint to endPoint and returns it
Ray* generateRay(const vector<float> &startPoint, const vector<float> &endPoint)
{
	vector<float> rayDirection;
	int x = 0;
	int y = 1;
	int z = 2;
	rayDirection.push_back(endPoint[x] - startPoint[x]);
	rayDirection.push_back(endPoint[y] - startPoint[y]);
	rayDirection.push_back(endPoint[z] - startPoint[z]);
	
	//create the ray
	Ray* myRay = new Ray(startPoint[x],startPoint[y],startPoint[z], rayDirection[x],rayDirection[y],rayDirection[z]);
	rayNumber++; //this variable is just used to keep track of how many rays are cast in the scene
	return myRay;

}
vector<int> findColor(const Ray &myRay, Scene &myScene, int depth);
/*
* Shadow Ray function. Produces a shadow ray from a point to a light source
* function that determines whether the intersection point on an object in the scene
* can see a specific light source.
* to determine this, we cast a ray from the point to the light source
* if there is any object in the way, then return 0 we can't see the light. otherwise return 1 + the remaining light contributions
*/
/*determines whether something is in between startPoint and LightSourcePoint, return 1 if yes 0 if no*/
int shadowRay(const Ray &myRay, Scene &myScene)
{
	//helper variables
	const int numObjects = myScene.myObjects.size();
	float* tValues  = new float[numObjects];
	Shape* closestShape = NULL;
	
	//we only want to check whether objects in the scene lie between the light and the intersection point
	//in order to do that we must find the proper tRestraint to pass into the intersection calculation
	//the function looks like: lightSourceLocation = rayStartPoint + tLightSource*rayDirection
	//solve for t: tLightSource = (lightSourceLocation - rayStartPoint) / rayDirection
	//do this component-wise. we only need one component to solve for t
	int z = 2; // x =0; //y = 1, z = 2; just shows which axis to use for the following line of code
	float tLight = ((myScene.myLightSources[0].getlocation()[z] - myRay.startPoint[z])/ myRay.direction[z]); 
	
	//look for objects in the scene that intersect the ray
	for(int item = 0; item < numObjects; item++)
	{
		tValues[item] = -1;
		(myScene.myObjects[item])->closestIntersection(myRay,&tValues[item],tLight, globalStartTime);
		//cout<<tValues[item]<<endl; 
		//if there's something in the way return false, we can't see it
		//if(tValues[item] != -1 && tValues[item] != 0)
		if(tValues[item] != -1 && tValues[item] > 0.0)
		{
			if(myScene.myObjects[item]->getMaterialProperty() == FLUORESCENT)
			{
				return 1;
			}
				if(DEBUG) cout<<"couldn't see the light"<<endl;
				return 0;
		}
	}
	if(DEBUG) cout<<" *****I can see the light"<<endl; 
	 
	//if nothing is in between return true, we can see the light
	return 1;
}

float areaShadowRay(const vector<float> startPoint, const LightSource &areaSource, Scene &myScene, Shape &closestShape)
{	
	float lightContribution = 0.0; //should be a number between 0 and 1
	int rows = 4;
	int columns = 3;
	int numBoxes = 12; //4x3
	float exitance = areaSource.intensity; 
	float albedo = closestShape.albedo; 
	float minAllowedDistanceFromLight = 0.1; 

	vector<float> nObject = closestShape.getNormal(startPoint, globalStartTime); //both unit vectors
	
	//break up the light source into several boxes
	vector<vector<float>> randPoints = ((const LightSource) areaSource).getRandomSample(rows, columns);
	
	//find random point in each box
	int points = randPoints.size();
	
	//cast shadow ray to each point
	for(int i = 0; i < points; i++)
	{
		Ray* myRay = generateRay(startPoint,randPoints[i]); 
		float visibility = shadowRay(*myRay, myScene);
		float distanceFromLight = myRay->magnitude;
		if(distanceFromLight < minAllowedDistanceFromLight) distanceFromLight = minAllowedDistanceFromLight; 

		//angle between the normal at the startPoint and the ray direction, we'll use dot product r . n = cos(theta)
		vector<float> r = myRay->direction; 
		float thetaPoint =  vectorMath::dot(r, nObject); 

		//angle between the normal of the area source and the ray direction, we'll use dot product again r . n  = cos(theta)
		vector<float> nLight = areaSource.area->getNormal(randPoints[i], globalStartTime);
		float thetaSource = vectorMath::dot(r,nLight); 
		
		//calculate power and add that to the light contribution
		float transmittedPower = (exitance/PI)*(abs(cos(thetaPoint))/(distanceFromLight* distanceFromLight)) * abs((cos(thetaSource)));
		float radiosityOfAreaSource = albedo * transmittedPower * visibility * exitance; 

		if(false )//strcmp(closestShape.getName().c_str(), "leftWall") == 0 )
		{

			cout<<"START!!!!!!!!!!!!!!!!!!!!!!"<<endl;
			cout<<"r's magnitude = "<<vectorMath::magnitude(r)<<endl;
			cout<<"nLight's magnitude = "<<vectorMath::magnitude(nLight)<<endl;
			cout<<"nObject's magnitude = "<<vectorMath::magnitude(nObject)<<endl;
			cout<<"height = "<<startPoint[2]<<endl;
			cout<<"thetaPoint = "<<thetaPoint * (180/PI)<<endl;
			cout<<"thetaLight = "<<thetaSource * (180/PI)<<endl;
			cout<<"distanceFromLight = "<<distanceFromLight<<endl;
			cout<<" transmittedPower = "<<transmittedPower<<" visibility = "<<visibility<<" exitance = "<<exitance<<"albedo = "<<albedo<<endl; 
			cout<<"radiosityOfAreaSource = "<<radiosityOfAreaSource<<endl;
			//radiosityOfAreaSource = 1.0; 
		}
		lightContribution = lightContribution + radiosityOfAreaSource;

	}
	/*cout<<"lightContribution = "<<lightContribution<<endl;
	cout<<"randPoints = "<<points<<endl;
	cout<<"lightContribution/points = "<<lightContribution/points<<endl;*/

	return lightContribution/points; 
}


//we get to a diffuse point and need to know how much of the light it sees
//we call this source lighting
float sourceLighting(const vector<float> intersectPoint, Scene &myScene, Shape &closestShape, int lightID)
{
	
	if(lightID >= myScene.myLightSources.size()) return 0;

	if(lightOption == POINTLIGHT)
	{
		const int numLights = myScene.myLightSources.size();
		float lightContribution = 0.0;
		for(int i = 0; i < numLights; i++)
		{
			//create ray to light source and follow it to find intersections
			Ray* myRay = generateRay(intersectPoint,myScene.myLightSources[i].getlocation()); 
			 lightContribution = lightContribution + shadowRay(*myRay,myScene);
		}
		 return (float)lightContribution/numLights;
	}
	else if(lightOption == AREA)
	{
		const int numLights = myScene.myLightSources.size();
		float lightContribution = 0.0;
		for(int i = 0; i < numLights; i++)
		{
			//if(intersectPoint[2] > 0) cout<<intersectPoint[2]<<endl;
			float areaShadowContribution = areaShadowRay(intersectPoint,myScene.myLightSources[i],myScene, closestShape);
			lightContribution = lightContribution + areaShadowContribution;
			//cout<<"areaShadowContribution = "<<areaShadowContribution<<"   lightContribution = "<<lightContribution<<endl;
		}
		 return (lightContribution/numLights);
	}
	else
	{
		//no light, everything is black
		return 0.0;
	}
}

vector<int> diffuseRay(const vector<float> &intersectPoint, Shape &closestShape, Scene &myScene)
{
	vector<int> finalColor = myScene.getBackgroundColor();
	float lightContributions = sourceLighting(intersectPoint, myScene, closestShape, 0); //should be a number between 0 and 1
			float newColorRed =		(float)closestShape.color[0] * lightContributions; 
			float newColorGreen =	(float)closestShape.color[1] * lightContributions; 
			float newColorBlue =	(float)closestShape.color[2] * lightContributions; 
			
			if(newColorRed > 255) newColorRed		= 255; 
			if(newColorGreen > 255) newColorGreen	= 255; 
			if(newColorBlue > 255) newColorBlue		= 255; 

			finalColor[0] = newColorRed;
			finalColor[1] = newColorGreen;
			finalColor[2] = newColorBlue;
			
			//if(lightContributions != 1.0 ) {cout<<"closestShape.color(red,green,blue) = ("<<closestShape.color[0]<<","<<closestShape.color[1]<<","<<closestShape.color[2]<<endl;
			//cout<<"lightContribution = "<<lightContributions<<endl;
		//	cout<<"finalColor = ("<<finalColor[0]<<","<<finalColor[1]<<","<<finalColor[2]<<endl;
		
	return finalColor; 
}

vector<int> specularRay(const Ray &myRay, const vector<float> normal, const vector<float> &intersectPoint, Scene &myScene,float albedo,int depth)
{
//calculate the new ray direction
	using namespace vectorMath;
			//d_s = 2(d_n . d_i) d_n - d_i
			//where:
			//d_s = direction of the outgoing ray (or specular reflected ray)
			//d_n = direction of the normal
			//d_i = direction of incoming ray direction
			int x = 0; 
			int y = 1; 
			int z = 2;
			vector<float> d_n = normal; 
			vector<float> d_i = myRay.direction; 
			vector<float> specularDirection =  scalarMultiply(vectorMath::minus(scalarMultiply(d_n,2* dot(d_n,d_i)),d_i), -1); 
			
			if(DEBUG) cout<<"normal  = <"<<d_n[x]<<","<<d_n[y]<<","<<d_n[z]<<"> "<<endl;
			if(DEBUG) cout<<"direction  = <"<<d_i[x]<<","<<d_i[y]<<","<<d_i[z]<<"> "<<endl;
			if(DEBUG) cout<<"normal  = <"<<specularDirection[x]<<","<<specularDirection[y]<<","<<specularDirection[z]<<"> "<<endl;
			Ray* specRay = new Ray(intersectPoint[x],intersectPoint[y],intersectPoint[z],specularDirection[x],specularDirection[y],specularDirection[z]); 
			
			specRay->albedo = myRay.albedo * albedo; 
			if(DEBUG) cout<<"albedo = "<<specRay->albedo<<endl;

			return findColor(*specRay, myScene,depth);
}

vector<int> transmittedRay(const Ray &myRay, const vector<float> normal, const vector<float> &intersectPoint, Scene &myScene,float albedo,int depth)
{
	using namespace vectorMath;
	//d_s = direction of the outgoing ray (or specular reflected ray)
	//d_n = direction of the normal
	//d_i = direction of incoming ray direction
	int x = 0; 
	int y = 1; 
	int z = 2;
	vector<float> d_n = normal;
	vector<float> d_n_minus = scalarMultiply(d_n, -1); 
	vector<float> d_i = myRay.direction; 
			
	float nWater = 1.333;
	float nAir = 1.000; 
			
	//find new ray direction by snells law
	vector<float> transmittedDirection; 
	float N_dot_I = dot(d_n,d_i);
	float k = 1.0 - nWater *nWater * (1.0 - N_dot_I * N_dot_I);
	if(k < 0.0){
		transmittedDirection.push_back(0.0);
		transmittedDirection.push_back(0.0);
		transmittedDirection.push_back(0.0);
	}
	else
	{
		transmittedDirection = vectorMath::minus(scalarMultiply(d_i,nWater), scalarMultiply(d_n,nWater * N_dot_I + sqrt(k)));
	}
	Ray* refractRay = new Ray(intersectPoint[x],intersectPoint[y],intersectPoint[z],transmittedDirection[x],transmittedDirection[y],transmittedDirection[z]);
	//recursively cast a new ray
	return findColor(*refractRay, myScene,depth); 
}

/*
* function determines whether the recursive ray casting should stop
* true denotes halting the recursion
* false allows for more ray casting
*/
bool shouldPrune(int depth, float albedo)
{
	if( pruneOption == DEPTH && depth >= 3) //stop the infinite recursion
	{
			if(DEBUG) {cout<<"WENT TOO DEEP"<<endl;
			system("pause");}
			return true; 
	}
	if(pruneOption == ALBEDO && albedo < 0.1)
	{
		if(DEBUG) 
			{
				cout<<"ALBEDO TOO SMALL"<<endl;
				system("pause");
		}
		return true;
	}
	return false;
}

//function that actually casts the ray recursively. 
vector<int> findColor(const Ray &myRay, Scene &myScene, int depth)
{
	int red, green, blue =  myScene.getBackgroundColor()[0]; /*default is black*/
	vector<int> finalColor = myScene.getBackgroundColor();
	vector<int> lightColor; lightColor.push_back(255);lightColor.push_back(255);lightColor.push_back(255); //white

	//stop infinite recursion
	if(shouldPrune(depth,myRay.albedo)) return finalColor;

	//loop through the scene's objects and find the closest intersection
	const int numObjects = myScene.myObjects.size();
	float* tValues  = new float[numObjects];
	Shape* closestShape = NULL;
	rayNumber++; //keep track of how many rays are cast in the scene

	//find all intersection points
	for(int item = 0; item < numObjects; item++)
	{
		tValues[item] = -1;
		(myScene.myObjects[item])->closestIntersection(myRay,&tValues[item], _maxSceneBoundary,globalStartTime);
	}

	//pick the closest one
	int closestItemIndex = 0; //I'm assuming there's at least an object in the scene
	float smallestTVal = _maxSceneBoundary;
	closestShape = myScene.myObjects[0];
	for(int item = 0; item < numObjects; item++)
	{
		if(tValues[item] > 0 && tValues[item] < smallestTVal)
		{
			closestItemIndex = item;
			closestShape = myScene.myObjects[item];
			smallestTVal = tValues[item];
		}
	}
	
	if(tValues[closestItemIndex] != -1)
	{
		//first find the actual intersection point;
		using namespace vectorMath; 
		using  vectorMath::plus; 
		float distanceAlongRay = tValues[closestItemIndex]; 
		float t = distanceAlongRay;
		vector<float> p = myRay.startPoint;
		vector<float> v = myRay.direction;
		
		//intersectPoint = startPoint + (distance along the ray) * direction; strtpt = p +tv;
		vector<float> intersectPoint = plus(p,scalarMultiply(v,t));
		
		//**take light sources into account**//
		if(closestShape->getMaterialProperty() == DIFFUSE) //diffuse surfaces
		{
			return diffuseRay(intersectPoint,*closestShape,myScene);
		}
		else if(closestShape->getMaterialProperty() == SPECULAR)//specular surfaces
		{
			depth++;
			return specularRay(myRay,closestShape->getNormal(intersectPoint, globalStartTime),intersectPoint,myScene,closestShape->albedo,depth);
		}
		else if(closestShape->getMaterialProperty() == TRANSMITTED)//refractive surfaces
		{
			depth++; 
			return transmittedRay(myRay,closestShape->getNormal(intersectPoint, globalStartTime),intersectPoint,myScene,closestShape->albedo,depth);
		}
		else //fluorescent surface 
		{
			return lightColor; 
		}
	}
	return finalColor;
}

//h and k are treated ast the center point of the circle
bool inCircle(float h, float k, float radius, float x, float y)
{
	//(x - h)^2 + (y - k)^2 - r^2 = 0
	//calculate the formula and if 
	//cout<<" (x - h)^2 = "<<(x - h) * (x - h)<<endl;
	//cout<<" (y - k)^2 = "<<(y - k) * (y - k)<<endl;
	float formula = (x - h) * (x - h) + (y - k) * (y - k) - radius*radius; 
	//cout<<"formula = "<<formula<<endl;
	if(formula < 0.0) return true; //the point lies in the circle
	else	return false; //it lies outside or on the circle 
}

//returns a point <x,y,z>
vector<float> getRandPointOnLense(const vector<float> &lenseCenter, float radius, int iteration)
{
	if(iteration > 10) return lenseCenter; //fail safe to stop infinite recursion; 


	vector<float> randPoint; 
	srand(time(NULL)); 
	int x = 0;
	int y = 1;
	int z = 2; 
	//generate a random point in a square of size radius^2
	float xCoord = (lenseCenter[x]  - radius)	+ ((float)(rand()%100)/100)	* (2*radius); //startPoint + some percentage of diameter
	float zCoord = (lenseCenter[z]	- radius)	+ ((float)(rand()%100)/100)	* (2*radius);

	//rejection sampling
	if(inCircle(lenseCenter[x],lenseCenter[z], radius, xCoord, zCoord))
	{
		randPoint.push_back(xCoord); 
		randPoint.push_back(lenseCenter[y]); 
		randPoint.push_back(zCoord); 
		return randPoint; 
	}
	else
	{
		//cout<<"wasn't in the circle and rayNumber = "<<rayNumber<<endl;
		return getRandPointOnLense(lenseCenter, radius, iteration + 1); 
	}

}

vector<int> castRaysPerPixel(int columns, int rows, int rayNumber, int numRays, Scene &myScene, int imgWidth, int imgHeight, int count)
{
	//the start location for the rays are broken up into intervals along the diagonal of the pixel in the x and z positions
	// r = ray location
	//        ------------------
	//       | r                |
	//       |   r              |
	//       |      r           |
	//      Z|        r         |    /
	//       |           r      |   / Y
	//       |               r  |  /
	//        ------------------- /
	//				   X
	float focalLength = myScene.myCamera->focalLength;
	float diagonalXComponent = ((float)rayNumber)/numRays;
	float diagonalYComponent = ((float)rayNumber)/numRays;	
	float startXlocation = (myScene.getCameraPosition()[0]  - imgWidth/2 + columns + diagonalXComponent)/cameraCoefficient;
	float startYlocation = myScene.getCameraPosition()[1];
	float startZlocation = (myScene.getCameraPosition()[2] - imgHeight/2 + rows + diagonalYComponent)/cameraCoefficient;

	vector<float> primaryRayDirection; // direction of the ray should be (startingPoint - focalPoint)/ ||(startingPoint - focalPoint)||
	primaryRayDirection.push_back(myScene.myCamera->getFocalPoint()[0] - startXlocation);
	primaryRayDirection.push_back((myScene.myCamera->getFocalPoint()[1]) - startYlocation);
	primaryRayDirection.push_back(myScene.myCamera->getFocalPoint()[2] - startZlocation);

	//set this pixel equal to the color of whatever it sees(adhering to the specular, reflective, and diffuse properties of the material course)
	vector<int> primaryRayColor =  findColor(*(new Ray(startXlocation,startYlocation,startZlocation,primaryRayDirection[0],primaryRayDirection[1],primaryRayDirection[2])),myScene, 0);
	//for motion blurr uncomment the following, you might have to adjust it for motion blurr though
	Sleep(shutterTime);
	vector<int> newColor2 = findColor(*(new Ray(startXlocation,startYlocation,startZlocation,primaryRayDirection[0],primaryRayDirection[1],primaryRayDirection[2])),myScene, 0);
	Sleep(shutterTime);
	vector<int> newColor3 = findColor(*(new Ray(startXlocation, startYlocation, startZlocation, primaryRayDirection[0],primaryRayDirection[1],primaryRayDirection[2])),myScene, 0);
	vector<int> motionBlurRayColor = vectorMath::scalarMultiply(vectorMath::plus(newColor3, vectorMath::plus(primaryRayColor, newColor2)),1.0/3.0);
	return motionBlurRayColor; 
	cout<<"hree"<<endl;
	//cast secondary rays through the lense
	//now we want to allow for lenses. Instead of casting a single ray through a pinhole, we cast multiple rays at random through a circular lense
	//we first cast our primary ray. This is simple, we did this without depth of field
	//we then calculate the focal point of the lense. This focal point can be found using the thin lense equation
	//then we pick a random starting point on the lense. cast a ray from that starting point to the focal point of the lense in the scene
	//and there it is, we have a ray. do the normal ray calculations on it and average the color over the pixel. 
	float numSecondaryRays = 3;
	vector<int> secondaryRaysColor; secondaryRaysColor.push_back(0); secondaryRaysColor.push_back(0); secondaryRaysColor.push_back(0); 
	float lenseRadius = 0.026;
	float f = -3.0 - myScene.myCamera->getFocalPoint()[1]; //focalDistance, should technically use abs around this
	if(focal){ cout<<"convergence Point = "<<f<<endl; focal = false;}
	vector<float> C; //C = point of ray convergence
	vector<float> D = primaryRayDirection; 
	vector<float> O = myScene.myCamera->getFocalPoint(); 

	//C = O + fD
	C = vectorMath::plus(O,vectorMath::scalarMultiply(D,f)); 

	//now that we have the convergence point we simply choose random points on our lense, then cast rays from those points to C
	for(int i = 0; i < numSecondaryRays; i++)
	{
		//choose random point
		vector<float> randPointOnLense = getRandPointOnLense(O, lenseRadius, 1);

		//cast ray to convergence point to find color
		vector<int> colorContribution = findColor(*generateRay(randPointOnLense,C),myScene,0);
		secondaryRaysColor[0] =  secondaryRaysColor[0] + colorContribution[0]; 
		secondaryRaysColor[1] =  secondaryRaysColor[1] + colorContribution[1]; 
		secondaryRaysColor[2] =  secondaryRaysColor[2] + colorContribution[2]; 
	}

	//average the color
	vector<int> newColor = vectorMath::scalarMultiply(vectorMath::plus(vectorMath::scalarMultiply(primaryRayColor,2),secondaryRaysColor),1.0/(float)(numSecondaryRays + 2)); 
	return newColor; 
}

BMP rTrace(int imgWidth, int imgHeight, Scene &myScene)
{
	//create an new picture to store the scene into
	BMP Output;
	Output.SetSize(imgWidth , imgHeight);
	Output.SetBitDepth( 32 );

	//cast several rays per pixel
	int numRays = 1;
	int count = 0;
	int width = Output.TellWidth();
	int height = Output.TellHeight();
	int totalNumPixels = width * height;
	int progress = 0;
	int progressPeriod = 10; 
	int checkPointNumber = 1; 
	for(int columns = 0; columns < width; columns++)
	{
		for(int rows  = 0;   rows < height; rows++)
		{	
			progress++;

			int checkPoint = (totalNumPixels/progressPeriod)*checkPointNumber;
		//cout<<"checkPoint = "<<checkPoint<<endl;
		//cout<<"progress ="<<progress<<endl;
		if(progress == checkPoint)
		{
			checkPointNumber++; 
			cout<<(int)(((float)progress/totalNumPixels)*100)<<"% complete.....     ";
			currTime = time(NULL); 
			totalTime = currTime - startTime; 
			if(totalTime < 60)	cout<<totalTime<<" seconds elapsed!"<<endl;									//seconds
			else if(totalTime < 60 * 60) cout<<(float)totalTime/60.0<<" minutes elapsed!"<<endl;			//minutes
			else if(totalTime < 60.0 * 60.0 * 60.0) cout<<(float)totalTime/(60.0*60.0)<<" hours elapsed!"<<endl;	//hours
			else  cout<<"took "<<(float)totalTime/(60.0*60.0 * 24.0)<<" days elapsed!"<<endl;				//days
		}
			Output(columns,rows)->Red  = 0;
			Output(columns,rows)->Green = 0;
			Output(columns,rows)->Blue = 0;

			//average the contribution of each ray into the pixel's final color
			for(int rayNum = 0; rayNum < numRays; rayNum++)
			{
				vector<int> newColor =  castRaysPerPixel(columns,rows,rayNum, numRays, myScene,imgWidth,imgHeight, count);

				Output(columns,rows)->Red += newColor[0]/numRays;
				Output(columns,rows)->Green += newColor[1]/numRays;
				Output(columns,rows)->Blue += newColor[2]/numRays;
				count++;
			}
		}
		//int percentage = int(((float)(progress)/totalNumPixels) * 100);
		//if( progress != 0 && percentage % progressPeriod == 0) cout<<percentage<<"% complete"<<endl;
		
		}
		return Output;
}

void reverseImageVert(BMP* myImgPtr)
{
	//flip the image vertically
	for(int column = 0; column < myImgPtr->TellWidth(); column++)
		for(int row = 0; row < myImgPtr->TellHeight()/2; row++)
		{
			int swapRow = myImgPtr->TellHeight() - row -1; 
			int red = (*myImgPtr)(column,row)->Red;
			int green = (*myImgPtr)(column,row)->Green;
			int blue = (*myImgPtr)(column,row)->Blue;
			
			(*myImgPtr)(column,row)->Red = (*myImgPtr)(column, swapRow)->Red;
			(*myImgPtr)(column,row)->Green = (*myImgPtr)(column, swapRow)->Green;
			(*myImgPtr)(column,row)->Blue = (*myImgPtr)(column, swapRow)->Blue;
			
			(*myImgPtr)(column,swapRow)->Red = red; 
			(*myImgPtr)(column,swapRow)->Green= green;
			(*myImgPtr)(column,swapRow)->Blue = blue;
		}
}

void reverseImageHoriz(BMP* myImgPtr)
{
	//flip the image horizontally
	for(int column = 0; column < myImgPtr->TellWidth()/2; column++)
		for(int row = 0; row < myImgPtr->TellHeight(); row++)
		{
			int swapColumn = myImgPtr->TellWidth() - column -1; 
			int red = (*myImgPtr)(column,row)->Red;
			int green = (*myImgPtr)(column,row)->Green;
			int blue = (*myImgPtr)(column,row)->Blue;
			
			(*myImgPtr)(column,row)->Red = (*myImgPtr)(swapColumn, row)->Red;
			(*myImgPtr)(column,row)->Green = (*myImgPtr)(swapColumn, row)->Green;
			(*myImgPtr)(column,row)->Blue = (*myImgPtr)(swapColumn, row)->Blue;
			
			(*myImgPtr)(swapColumn,row)->Red = red; 
			(*myImgPtr)(swapColumn,row)->Green= green;
			(*myImgPtr)(swapColumn,row)->Blue = blue;
		}
}

Scene* createScene()
{
	//TODO:fix the dynamic viewing functionality. It We should get the same scene no matter how big the balls are or how many balls we add. 
	//create my Scene
	Scene* myScene = new Scene();
	
	float focalLengthRatio = 2.0; //10.0 is the standard
	
	//focal length is set to enlarge as the output image increases in size. This gives the same view of the scene no matter how big the picture is.
	//it effectively pushes the pinhole further away from the Image Plane or pixels in the back of the camera as the output Image grows
	myScene->myCamera->focalLength = outputImageWidth/(focalLengthRatio * cameraCoefficient);
	cout<<"focal length = "<<myScene->myCamera->focalLength<<endl;
	
	//system("pause");

								/*create Spheres*/	
	//left sphere
	vector<float> sphereCenter;
	float leftSphereRadius = 3.0; 
	sphereCenter.push_back(-8.0);
	sphereCenter.push_back(-15.0);
	sphereCenter.push_back(0.0);
	Sphere* leftSphere = new Sphere(255,0,0,DIFFUSE, sphereCenter, leftSphereRadius); //red
	leftSphere->setName("leftSphere"); 
//	leftSphere->setMaterialProperty(SPECULAR);
	//backWall->albedo = 0.5;

	
	//middle sphere
	float middleSphereRadius = 3.0; 
	vector<float> middleSphereCenter;
	middleSphereCenter.push_back(0.0);
	middleSphereCenter.push_back(-3.0);
	middleSphereCenter.push_back(0.0);
	Sphere* middleSphere = new Sphere(0,255,0,DIFFUSE, middleSphereCenter, middleSphereRadius);//green
	middleSphere->setName("middleSphere"); 

	//right sphere
	float rightSphereRadius = 5.0;
	sphereCenter[0] = 10.0;
	sphereCenter[1] = 10.0;//rightSphereRadius + 2.0;
	sphereCenter[2] = 0.0;
	Sphere* rightSphere = new Sphere(0,0,255, DIFFUSE, sphereCenter, rightSphereRadius); //blue
	rightSphere->setName("rightSphere"); 
	//rightSphere->mobile = true; 
	//rightSphere->setXBounds(sphereCenter[0] + rightSphereRadius, -3.0 * rightSphereRadius + 10 * rightSphereRadius);

	//create back wall, its white
	float spaceInBetween = 1.0;
	Box* backWall = new Box();
	float backWallWidth = 32;//(numSpheres -1) * spaceInBetween +  numSpheres * largestSphereRadius * 2; //space in between + diameter of spheres
	float backWallHeight = backWallWidth;
	float backWallx = -backWallWidth/2; //start to the left of the origin
	float backWally = 3.0 * 5.0 ; //start deep into the the picture
	float backWallz = -(3.0); //start below the origin
	backWall->setDimensions(backWallWidth,0, backWallHeight,FORWARD);
	backWall->setFrontLowerLeftCorner(backWallx,backWally,backWallz);
	backWall->setColor(255,225,225);
	backWall->setName("backWall"); 

	//create front wall, its pink
	float frontWallWidth = backWallWidth;
	float frontWallHeight = backWallHeight;
	float frontWallx = backWallx; //start to the left of the origin
	float frontWally = -30.0; //start deep into the the picture
	float frontWallz = backWallz; //start below the origin
	Box* frontWall = new Box();
	frontWall->setDimensions(frontWallWidth,0,frontWallHeight,FORWARD);
	frontWall->setFrontLowerLeftCorner(frontWallx,frontWally,frontWallz);
	frontWall->setColor(255,100,100);
	frontWall->setName("frontWall"); 
	
	//create top wall, its blue
	Box* ceiling = new Box();
	float ceilingWidth = backWallWidth + 4.0;
	float ceilingLength = abs(frontWally - backWally); // 6 units before the origin and 3 units after in the y direction
	float ceilingX = backWallx- 2.0; //start to the left of the origin
	float ceilingY = frontWally; //start behind the camera
	float ceilingZ = backWallHeight + backWallz; //start above the origin
	ceiling->setDimensions(ceilingWidth, ceilingLength,0,DOWN);
	ceiling->setFrontLowerLeftCorner(ceilingX,ceilingY, ceilingZ);
	ceiling->setColor(0,0,255);
	ceiling->setName("ceiling"); 
	//ceiling->setMaterialProperty(SPECULAR);

	//create floor, its light green
	Box* floor = new Box();
	float floorWidth = ceilingWidth;
	float floorLength = ceilingLength;
	float floorX = ceilingX; //start to the left of the origin
	float floorY = ceilingY; //start behind the camera
	float floorZ = backWallz; //start below the origin
	floor->setDimensions(floorWidth,floorLength,0,UP);
	floor->setFrontLowerLeftCorner(floorX,floorY,floorZ);
	floor->setColor(200,255,200);
	floor->setName("floor"); 

	//create left wall, its yellowish
	Box* leftWall = new Box();
	float leftWallLength = ceilingLength;
	float leftWallHeight = backWallHeight;
	float leftWallx = backWallx; //start to the left of the origin
	float leftWally = frontWally; //start deep into the the picture
	float leftWallz = backWallz; //start below the origin
	leftWall->setDimensions(0,leftWallLength, leftWallHeight,RIGHT);
	leftWall->setFrontLowerLeftCorner(leftWallx,leftWally,leftWallz);
	leftWall->setColor(255,255,0);
	leftWall->setName("leftWall"); 

	//create right wall, its purple-ish
	Box* rightWall = new Box();
	float rightWallLength =ceilingLength;
	float rightWallHeight = leftWallHeight;
	float rightWallx =  leftWallx + backWallWidth; //start to the right of the origin
	float rightWally =  leftWally; //start deep into the the picture
	float rightWallz =  leftWallz; //start below the origin
	rightWall->setDimensions(0,rightWallLength,rightWallHeight,RIGHT);
	rightWall->setFrontLowerLeftCorner(rightWallx,rightWally,rightWallz);
	rightWall->setColor(100,20,150);
	rightWall->setName("rightWall"); 

	//set some global variables that determine the view, camera location
	float cameraY = frontWally + .1;//+ ceilingLength/2.0;
	float cameraZ = 0.0 ;

	//set the viewing point that the camera faces
	myScene->myCamera->setLocation(0.0, cameraY, cameraZ); //camera is near the center of the room
	vector<float> focusVector;
	focusVector.push_back(0.0); focusVector.push_back(0.0); focusVector.push_back(0.0); //the origin
	float focusPointX = 0.0;
	float focusPointY = focusVector[1] + myScene->myCamera->getLocation()[1] + myScene->myCamera->focalLength;
	float focusPointZ = 0.0;
	myScene->myCamera->changeDirection(focusPointX , focusPointY,focusPointZ);//focus in between both balls
	cout<<"frontWally = "<<frontWally<<endl;
	cout<<"backWally = "<<backWally<<endl;
	cout<<"camera y location = "<<myScene->myCamera->getLocation()[1]<<endl;
	
	leftSphere->mobile = true; 
	leftSphere->setXBounds(leftWallx + 6.0 , rightWallx - 6.0);

	//add all the shapes to my scene
	myScene->placeShape(ceiling);
	myScene->placeShape(floor);
	myScene->placeShape(leftWall);
	myScene->placeShape(rightWall);
	myScene->placeShape(frontWall);
	myScene->placeShape(backWall);
	
	myScene->placeShape(leftSphere);
	myScene->placeShape(middleSphere); 
	myScene->placeShape(rightSphere);

	//place light source
	float lightSourceX = frontWallx + floorWidth/2;
	float lightSourceY = backWally - floorLength/2.5;
	//float lightSourceZ = backWallz +  backWallHeight/1.2;
	float lightSourceZ = ceilingZ - 0.1;
	LightSource* ceilingLight = new LightSource(lightSourceX,lightSourceY,lightSourceZ);
	float lightAreaWidth = ceilingWidth/3.0;
	float lightAreaLength = ceilingLength/8.0;
	float lightAreaHeight = 0.0;
	ceilingLight->area->setDimensions(lightAreaWidth, lightAreaLength,0.0,DOWN);
	ceilingLight->area->setFrontLowerLeftCorner(lightSourceX - lightAreaWidth/2,lightSourceY - lightAreaLength/2, lightSourceZ - lightAreaHeight);
	ceilingLight->area->facing = DOWN;
	//myScene->placeLightSource(lightSourceX,lightSourceY,lightSourceZ);
	myScene->placeLightSource(*ceilingLight);
	
	//place second light source
	lightSourceX = frontWallx + floorWidth/1.2;
	lightSourceY = backWally - floorWidth/2.3;
	lightSourceZ = backWallz +  backWallHeight/1.2;
	//myScene->placeLightSource(lightSourceX,lightSourceY,lightSourceZ);

	//check for lighting (area or point source lighting)
	if(lightOption == AREA)
	{
		for(int i = 0; i < myScene->myLightSources.size(); i++)
			myScene->myObjects.push_back(myScene->myLightSources[i].area);
	}
	return myScene;
}

void main(int argc, char** argv)
{
	
	//TODO: Image is stretched a bit for some reason. figure out why


	//test for incircle
	/*float h			= 0.0;
	float k			= 0.0;
	float radius	= 1.0;
	float pointx	= .86;
	float pointy =.5;
	bool test = inCircle(h,k, radius, pointx, pointy);

	cout<<"test = "<<test<<endl;
	cout<<"point = <"<<pointx<<","<<pointy<<">"<<endl;
	*/
	
	srand(time(NULL)); 
	startTime = time(NULL); 
	
	Scene* myScene = createScene();	
	
	//raytrace the scene to get an image back 
	if(DEBUG) cout<< "******************** DEBUG MODE **********************"<<endl;
	cout<<"Ray Tracing...."<<" "<<outputImageWidth<<" x "<<outputImageHeight<<" scene"<<endl; 
	BMP newImage = rTrace(outputImageWidth, outputImageHeight,*myScene);

	//image is flipped when stored, reverse it
	reverseImageHoriz(&newImage);
	cout<<"casted "<<rayNumber<<" rays total"<<endl; 

	//render the image
	newImage.WriteToFile("Scene.bmp");
	cout<<"wrote to file"<<endl;
	endTime = time(NULL); 
	totalTime = endTime - startTime; 
	if(totalTime < 60)	cout<<"took "<<totalTime<<" seconds to raytrace the scene!"<<endl;										//seconds
	else if(totalTime < 60 * 60) cout<<"took "<<(float)totalTime/60.0<<" minutes to raytrace the scene!"<<endl;					//minutes
	else if(totalTime < 60.0 * 60.0 * 60.0) cout<<"took "<<(float)totalTime/(60.0*60.0)<<" hours to raytrace the scene!"<<endl;	//hours
	else  cout<<"took "<<(float)totalTime/(60.0*60.0 * 24.0)<<" days to raytrace the scene!"<<endl;								//days
	system("pause");
}