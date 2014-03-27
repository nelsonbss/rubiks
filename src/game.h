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
#include "Observer.h"
#include "drawingCanvas.h"
#include "menuPuzzle.h"

class game : public Observer{
public:
	game(SG_VECTOR p, float w, float h, SG_VECTOR puzzlePos, float iddleTime);

	void setup(sgCObject *sgBunnyi,sgCObject *sgTetrahedroni,sgCObject *sgDodecahedroni,sgCObject *sgIcosahedroni,sgCObject *sgOctahedroni);//,sgCObject *sgTeapoti);
	void update();
	void draw();
	void exit();

	int step;

	SG_VECTOR posGame;
	float width;
	float height;

	SG_VECTOR slicingPos;  //where it gets sliced
	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	armature *myArmature;
	puzzle *myPuzzle;


	///OBJ file loading and convertion
	sgCObject *sgBunny;
	sgCObject *sgTetrahedron;
	sgCObject *sgDodecahedron;
	sgCObject *sgIcosahedron;
	sgCObject *sgOctahedron;
	sgCObject *sgTeapot;
	////
	void loadObject (int objID,SG_VECTOR p,SG_VECTOR t);
	int objectID;

	/*
	Paul's
	*/

	void guiLoad(int _obj);
	bool bHaveNewObject;
	int newObject;

	bool bHaveNext;
	void guiNext(){bHaveNext = true;}

	bool bHaveReset;
	void guiReset(){bHaveReset = true;}

	void update(string _eventName, SubObEvent* _event);

	bool bExtrude;
	void guiExtrude(){bExtrude = true;}

	ofFloatColor sc;

	/*
	*/

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

	////////////////armature offset
	//offset vars
	ofVec3f offsetSlicer;
	ofVec3f giveOffset();
	ofVec3f rotateSlicer;
	ofVec3f giveRotation();
	float tamCubie;

	/////////////////face rotations
	bool updatePuzzle;
	int idcubie;
	bool dir;
	SG_VECTOR axis;
	void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir);
	bool faceRotate;
	//history to undo
	vector<history> historyV; 
	void unDo();
	//face rotation bytwo ids
	void rotateTwoIds(int cubieA, int cubieB,bool inside);
	bool faceRotateB;
	int idcubieA;
	int idcubieB;

	//////////////////////////move  armature
	float tamSideArmature;
	SG_VECTOR posA;
	void moveA (ofVec3f input);
	/////////////////////////////rotate armature
	SG_VECTOR rotA;
	void rotateA(ofVec3f input);

	////color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);

	//armature rotation
	void applyArmRotations();

	//gui communication
	bool rotateB;
	void guiInput(int in);

	///////////////////  puzzle trackball
	ofVec3f axistb;
	float angle;
	//current state of the rotation
	ofQuaternion curRot;
	//a place to store the mouse position so we can measure incremental change
	ofVec2f lastMouse;
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	//inactive state timer
	int goToAttractStepI;
	int goToAttractStepS;
	float iddleTimer;

	//drawing canvas
	ofVec3f posCanvas;
	drawingCanvas *myCanvas;
	bool canvasB;
	void prepareDrawing();
	ofImage myCanvasImage; //DrawingGrid_555x35.png

	//object extrusion
	void clearDisplayedObject();
	void extrudeObject();
	bool extrudeObject(ofPolyline *drawing);
	sgC3DObject* extrudedObject;
	bool extrudedB;


	///////interaction with puzzles on the center
	void loadPuzzle(puzzle *inputPuzzle); //load a puzzle from the puzzle menu on the center
	bool savePuzzleB;
	menuPuzzle* savePuzzle(SG_POINT slicingPos, SG_VECTOR middlePuzzlePos);
};
#endif /* defined(__Tgame__game__) */
