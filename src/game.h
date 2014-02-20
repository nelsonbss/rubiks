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
	////////////move all puzzle: every cubbie moves the same way
	SG_VECTOR pos;
	void move (SG_VECTOR p);
	/////////////////////////////rotate all puzzle
	SG_VECTOR rot;
	void rotate(SG_VECTOR r);

	////////////////
	bool updatePuzzle;
	//face rotations
	int idcubie;
	bool dir;
	SG_VECTOR axis;
	void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir);
	bool faceRotate;

};
#endif /* defined(__Tgame__game__) */
