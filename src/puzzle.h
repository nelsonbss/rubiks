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
	puzzle(SG_VECTOR p, ofVec3f offset, ofVec3f offRotate);

	void setup();
	void update();
	void draw();

	void exit();

	void loadPieces(sgCGroup **pcs, int selObjId);
	int giveNumCubies();
	sgCGroup* cubieGroup;
	float numPieces;
	cubie **myCubies; //malloc numPieces of these
	ofVec3f cubiesOffset;
	ofVec3f cubiesOffrotate;

	SG_VECTOR pos;
	void move(SG_VECTOR p);

	SG_VECTOR rot;
	void rotate(SG_VECTOR r);

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




	///quaternions
	ofQuaternion qx;
	ofQuaternion qy;
	ofQuaternion qt;
};

#endif /* defined(__Tpuzzle__puzzle__) */