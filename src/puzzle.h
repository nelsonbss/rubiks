#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"

class puzzle {

public:
	puzzle();

	void setup();
	void update();
	void draw();
	void unDraw();

	void rotate();

	void loadPieces(sgCGroup **pcs);
	int giveNumCubies();

	float numPieces;
	cubie **myCubies; //malloc numPieces of these

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
};

#endif /* defined(__Tpuzzle__puzzle__) */