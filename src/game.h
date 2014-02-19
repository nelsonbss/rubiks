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
	//move all puzzle: every cubbie moves the same way
	bool movePRight;
	bool movePLeft;
	bool movePUp;
	bool movePDown;

	float width;
	float height;

	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	puzzle *myPuzzle;

	void displayObject (int objID,SG_VECTOR p);
	bool draw3dObject;
	
	void createSlicer();

	bool makeCut;
	bool drawCuts;
	bool drawCuts1;
	void createPuzzle(SG_VECTOR p);
	bool puzzleExists;
	bool updatePuzzle;

	void rotatePuzzleH();
	void rotatePuzzleV();
	//rotate all puzzlw: every cubbie rotates the same way
	bool rotatePHright;
	bool rotatePHleft;
	bool rotatePVup;
	bool rotatePVdown;

};
#endif /* defined(__Tgame__game__) */
