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
	//void unDraw();// has legacy of how to detach from scene
	void exit();

	int id;
	int getId();

	sgCGroup *objects;
	float numObjs;
	sgC3DObject **objectList;
	vector<ofMesh> myMeshs; // this will be used to store the objet to be drawn 
	vector<ofVbo>  myVbos;

	float color;
	bool okDraw;

	void setObjects(sgCGroup *objs,int cubieId);
	sgCGroup* copyObjects();   //this is no longer needed

	SG_VECTOR pos;
	void move(SG_VECTOR p);
	
	SG_POINT pointRotate;
	SG_VECTOR rot;
	void rotate(SG_VECTOR r); //rotates the cubie as part of the whole puzzle object, NOT as a faceMove rotation

	//this is vector of matrix objects that have all the transformations for each cubie.
	//its a vector since we don't know how many transformations a cubie is going to have

	vector<matrix> myMatrix; 
	void faceRotate(SG_VECTOR axis, bool dir);
	/*double rotX;
	double rotY;
	double rotZ;*/
	bool moving;
	//bool movingXC;
	//bool movingXCC;
	//bool movingYC;
	//bool movingYCC;
	//bool movingZC;
	//bool movingZCC;

	//float ct1;
	//float ct2;
	float animTime;

	bool sample;
	double rotXa;

	//undo
	bool undoing;

	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);

	void unDo();
};

#endif /* defined(__Tcubie__cubie__) */