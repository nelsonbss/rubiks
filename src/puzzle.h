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
	puzzle(float x, float y, float z);

	void setup();
	void update();
	void draw();

	void exit();

	void loadPieces(sgCGroup **pcs, int selObjId);
	int giveNumCubies();
	sgCGroup* cubieGroup;
	float numPieces;
	cubie **myCubies; //malloc numPieces of these

	SG_VECTOR pos;
	void move(SG_VECTOR p);

	SG_VECTOR rot;
	void rotate(SG_VECTOR r);

	double ***p3DArray;
	 /* one-liner */
    //typedef std::vector<std::vector<std::vector<int> > >ThreeDimensions;
    /* expanded */
    typedef std::vector<int>OneDimension;
    typedef std::vector<OneDimension>TwoDimensions;
    typedef std::vector<TwoDimensions>ThreeDimensions;
	ThreeDimensions three_dim1;
	void rotateByIDandAxis(int id, SG_VECTOR axis,bool dir);
	void rearange3dArray(SG_VECTOR axis, int plane, bool dir);

	void unDo(int id, SG_VECTOR axis, bool dir);

	//color faces
	void colorFaces();
	void colorCubiesBlackSides();
	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);
};

#endif /* defined(__Tpuzzle__puzzle__) */