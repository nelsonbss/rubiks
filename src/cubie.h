#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "sgCore.h"
#include "matrix.h"

#include <vector>

class cubie {
	
public:
	cubie(float x,float y,float z,int id, int selObjId);
    
	void setup();
	void update();
	void draw();
	//void unDraw();// has legacy of how to detach from scene
	void exit();

	int id; //id of the cuie to be used on the selection of the cuie to rotate
	int getId();

	int selectedObjectID; // keeps the id of the object selected to become a puzzle, used for coloring

	sgCGroup *objects;
	float numObjs;
	float getNumObjs();
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
	void unDo(SG_VECTOR axis,bool di);

	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);
	void colorBlackSides(int cubieId, float playRoom);
	
};

#endif /* defined(__Tcubie__cubie__) */