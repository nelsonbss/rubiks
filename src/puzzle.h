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

	void loadPieces(sgCGroup **pcs);
	int giveNumCubies();

	float numPieces;
	cubie **myCubies; //malloc numPieces of these

	SG_VECTOR pos;

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	float rotH;
	float rotV;
	void rotateHright();
	void rotateHleft();
	void rotateVup();
	void rotateVdown();

	double ***p3DArray;
	void faceRotate(SG_POINT point, SG_VECTOR axis, float deg,bool dir);
	 /* one-liner */
    //typedef std::vector<std::vector<std::vector<int> > >ThreeDimensions;
    /* expanded */
    typedef std::vector<int>OneDimension;
    typedef std::vector<OneDimension>TwoDimensions;
    typedef std::vector<TwoDimensions>ThreeDimensions;
	ThreeDimensions three_dim1;
	void rotateByIDandAxis(int id, SG_VECTOR axis,bool dir,float deg);
	void rearange3dArray(SG_VECTOR axis, int plane, bool dir);

	void unDo();

};

#endif /* defined(__Tpuzzle__puzzle__) */