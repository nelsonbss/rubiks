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
#include "history.h"
#include "armRotations.h"

#include <vector>

class game {
public:
	game(SG_VECTOR p, float w, float h, SG_VECTOR puzzlePos);

	void setup();
	void update();
	void draw();
	void exit();

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
	int objectID;

	/*
	Paul's
	*/
	void guiLoad(int _obj);
	bool bHaveNewObject;
	int newObject;
	
	void loadArmature(int type);
	int armID;
	void createCutterSlicer();//(float thick, float tamPlane, float tamCuby,float numCutr, float x, float y, float z);

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

	////////////////puzzle movements
	//offset vars
	ofVec3f offsetSlicer;
	ofVec3f giveOffset();
	ofVec3f rotateSlicer;
	ofVec3f giveRotation();
	float tamCubie;

	//face rotations
	bool updatePuzzle;
	int idcubie;
	bool dir;
	SG_VECTOR axis;
	void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir);
	bool faceRotate;
	//history to undo
	vector<history> historyV; 
	void unDo();

	//////////////////////////move  armature
	float tamSideArmature;
	SG_VECTOR posA;
	void moveA (ofVec3f input);
	/////////////////////////////rotate armature
	SG_VECTOR rotA;
	void rotateA(ofVec3f input);

	///other objects
	ofxAssimpModelLoader bunny;
	sgCObject *sgBunny;
	ofxAssimpModelLoader tetrahedron;
	sgCObject *sgTetrahedron;
	ofxAssimpModelLoader dodecahedron;
	sgCObject *sgDodecahedron;
	ofxAssimpModelLoader icosahedron;
	sgCObject *sgIcosahedron;
	ofxAssimpModelLoader octahedron;
	sgCObject *sgOctahedron;
	ofxAssimpModelLoader pot;
	sgC3DObject *sgTeapot;
	
	////color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);

	//armature rotation history
	vector<armRotations> armRotH;

	//gui communication
	bool rotateB;
	void guiInput(int in);
};
#endif /* defined(__Tgame__game__) */