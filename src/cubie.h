#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "sgCore.h"
#include "matrix.h"

#include <vector>

class cubie {
	
public:
	cubie(float x,float y);
    
	void setup();
	void update();
	void draw();
	void unDraw();
	void exit();

	sgCGroup *objects;
	float numObjs;
	sgCObject **objectList;

	float color;
	bool okDraw;

	void setObjects(sgCGroup *objs);
	sgCGroup* copyObjects();

	float posX;
	float posY;
	void moveH(float px);
	void moveV(float py);
	
	float rotH;
	float rotV;
	void rotateH(float radH); //rotates the cubie as part of the whole puzzle object NOT as a faceMove rotation
	void rotateV(float radV);

	//this is vector of matrix objects that have all the transformations for each cubie.
	//its a vector since we don't know how many transformations a cubie is going to have

	vector<matrix> myMatrix; 
	void faceRotate(SG_POINT point, SG_VECTOR axis, float deg);
};

#endif /* defined(__Tcubie__cubie__) */