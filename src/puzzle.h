#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"
#include <vector>
#include <iostream>

class puzzle {

public:
	puzzle(SG_VECTOR p, ofVec3f offset);

	void setup();
	void update();
	void draw();

	void exit();

	void loadPieces(sgCGroup **pcs, int selObjId, ofVec3f v, float angleA, ofVec3f axistbA, int mode);
	//mode:
	//1 is for regular euler angle rotation inside rotateSlicer
	//2 is for quaternion rotate angle and vector info
	int giveNumCubies();
	sgCGroup* cubieGroup;
	float numPieces;
	cubie **myCubies; //malloc numPieces of these
	ofVec3f cubiesOffset;
	//ofVec3f cubiesOffrotate;

	SG_VECTOR pos;
	void move(SG_VECTOR p);

	SG_VECTOR rot;
	void rotate(SG_VECTOR r);
	void rotateTB(float angle, ofVec3f axis);
	ofVec3f qaxis; 
	float qangle; 

	typedef std::vector<int>OneDimension;
	typedef std::vector<OneDimension>TwoDimensions;
	typedef std::vector<TwoDimensions>ThreeDimensions;
	ThreeDimensions three_dim1;
	void rotateByIDandAxis(int id, SG_VECTOR axis,bool dir);
	void rearange3dArray(SG_VECTOR axis, int plane, bool dir);

	void unDo(int id, SG_VECTOR axis, bool dir);

	bool moving;
	bool isMoving();

	//color faces
	void colorFaces(int objectID);
	void colorCubiesBlackSides();
	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);



	//   //current state of the rotation
	//ofQuaternion curRot;
	////a place to store the mouse position so we can measure incremental change
	//ofVec2f lastMouse;
	//void mouseDragged(int x, int y, int button);
	//void mousePressed(int x, int y, int button);
};

#endif /* defined(__Tpuzzle__puzzle__) */