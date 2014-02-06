#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"

class puzzle {

public:
	puzzle(float x, float y);

	void setup();
	void update();
	void draw();
	void unDraw();

	void loadPieces(sgCGroup **pcs);
	int giveNumCubies();

	float numPieces;
	cubie **myCubies; //malloc numPieces of these

	float posX;
	float posY;
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
};

#endif /* defined(__Tpuzzle__puzzle__) */