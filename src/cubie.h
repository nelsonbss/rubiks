#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "sgCore.h"
#include "matrix.h"

#include <vector>

class cubie {
	
public:
	cubie(float x,float y,float z,int id);
    
	void setup();
	void update();
	void draw();
	void unDraw();
	void exit();

	int id;

	sgCGroup *objects;
	float numObjs;
	sgC3DObject **objectList;
	vector<ofMesh> myMeshs; // this will be used to store the objet to be drawn 
	vector<ofVbo>  myVbos;

	float color;
	bool okDraw;

	void setObjects(sgCGroup *objs,int cubieId);
	sgCGroup* copyObjects();

	float posX;
	float posY;
	float posZ;
	void moveH(float px);
	void moveV(float py);
	
	float rotH;
	float rotV;
	void rotateH(float radH); //rotates the cubie as part of the whole puzzle object, NOT as a faceMove rotation
	void rotateV(float radV);

	//this is vector of matrix objects that have all the transformations for each cubie.
	//its a vector since we don't know how many transformations a cubie is going to have

	vector<matrix> myMatrix; 
	void faceRotate(SG_VECTOR axis, float deg, bool dir);
	float rotX;
	float rotY;
	float rotZ;
};

#endif /* defined(__Tcubie__cubie__) */