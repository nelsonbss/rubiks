#ifndef __Tgame__game__
#define __Tgame__game__

#include "ofMain.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "cutter.h"
#include "slicer.h"
#include "armature.h"
#include "puzzle.h"
#include "ofxAssimpModelLoader.h"

class game {
public:
	game(SG_VECTOR p, float w, float h, SG_VECTOR puzzlePos);

	void setup();
	void update();
	void draw();

	int step;

	SG_VECTOR posGame;
	float width;
	float height;

	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	armature *myArmature;
	puzzle *myPuzzle;

	void loadObject (int objID,SG_VECTOR p,SG_VECTOR t);
	void createSlicer();
	void createPuzzle(SG_VECTOR p);

	int getCurrentStep();
	void setCurrentStep(int step);
	void restart();
	/////////////////////////////move all puzzle: every cubbie moves the same way
	SG_VECTOR posP;
	void moveP (SG_VECTOR p);
	/////////////////////////////rotate all puzzle
	SG_VECTOR rotP;
	void rotateP(SG_VECTOR r);

	////////////////
	bool updatePuzzle;
	//face rotations
	int idcubie;
	bool dir;
	SG_VECTOR axis;
	void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir);
	bool faceRotate;

	//////////////////////////move  armature
	SG_VECTOR posA;
	void moveA (SG_VECTOR pa);
	/////////////////////////////rotate armature
	SG_VECTOR rotA;
	void rotateA(SG_VECTOR ra);

	///other objects
	ofxAssimpModelLoader bunny;
	sgCObject *sgBunny;


	////color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);
};
#endif /* defined(__Tgame__game__) */
