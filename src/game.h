#ifndef __Tgame__game__
#define __Tgame__game__

#include "ofMain.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

class game {
public:
	game(SG_VECTOR p, float w, float h);

	void setup();
	void update();
	void draw();

	int step;

	SG_VECTOR pos;
	float width;
	float height;

	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	puzzle *myPuzzle;

	void loadObject (int objID,SG_VECTOR p,SG_VECTOR t);
	void createSlicer();
	void createPuzzle(SG_VECTOR p);

	void restart();

	void rotatePuzzleH();
	void rotatePuzzleV();

	////////////move all puzzle: every cubbie moves the same way
	void movePRightF(bool active);
	void movePLeftF(bool active);
	void movePUpF(bool active);
	void movePDownF(bool active);
	//
	bool movePRight;
	bool movePLeft;
	bool movePUp;
	bool movePDown;
	/////////////////////////////rotate all puzzle
	SG_VECTOR rot;
	void rotate(SG_VECTOR r);
	bool rotateB;

	////////////////
	bool updatePuzzle;

	

	//face rotations
	float tempDeg;
	bool faceRotateCx;
	bool faceRotateCCx;
	bool faceRotateCy;
	bool faceRotateCCy;
	bool faceRotateCz;
	bool faceRotateCCz;

};
#endif /* defined(__Tgame__game__) */
