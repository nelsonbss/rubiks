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
#include "Picker.h"

#define DELAY 30000

enum{UP_MODE_MOUSE, UP_MODE_COLOR, UP_MODE_P};

class game : public Observer{
public:
	game(SG_VECTOR p, float w, float h, SG_VECTOR puzzlePos, ofRectangle _vp, float iddleTime, string myPrefix);

	//void setup(sgCObject *sgBunnyi,sgCObject *sgTetrahedroni,sgCObject *sgDodecahedroni,sgCObject *sgIcosahedroni,sgCObject *sgOctahedroni, string _prefix);//,sgCObject *sgTeapoti);
	void setup(string _prefix);
	void setup();
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

	string station;
	///OBJ file loading and convertion
	//sgCObject *sgBunny;
	//sgCObject *sgTetrahedron;
	//sgCObject *sgDodecahedron;
	//sgCObject *sgIcosahedron;
	//sgCObject *sgOctahedron;
	//sgCObject *sgTeapot;

	map<int, sgCObject*> objects;
	ofxAssimpModelLoader loader;
	void loadObjDir(string _path);
	////
	void loadObjectG(int objID,SG_VECTOR p,SG_VECTOR t);
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
	bool bHaveMakeOne;
	void guiReset(){bHaveReset = true;}

	void update(string _eventName, SubObEvent _event);

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
	//void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir);
	void rotateByIDandAxis(int id, SG_VECTOR axis, bool dir, float angle);
	float angleR;

	bool faceRotate;
	//history to undo
	vector<history> historyV; 
	void unDo();
	void decideMove();

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
	void changeFaceColor(ofVec2f pos, ofFloatColor c);

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
	drawingCanvas myCanvas;
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
	void loadMenuObject (int objID,SG_VECTOR p,SG_VECTOR t);
	void loadArmatureMenu(int type);
	void makePuzzle();
	bool savePuzzleB;
	menuPuzzle* savePuzzle(SG_POINT slicingPos, SG_VECTOR middlePuzzlePos);
	vector< ofFloatColor > colorsVMenu;
	vector< ofVec3f > uniqueNormals;
	////////////////////////////////////////////
	Picker picker;
	ofVec3f unprojectedPoint;
	ofVec3f projectedPoint;
	ofVec3f lastUnprojectedPoint;
	ofVec3f mousePoint;
	bool bUnproject;
	bool bDragInput;

	ofRectangle viewport;
	void drawViewportOutline(const ofRectangle & _vp);

	bool bUseViewport;
	void setUseViewport(bool _b){bUseViewport = _b;}
	void toggleUseViewport(){bUseViewport != bUseViewport;}

	void unprojectPoint(ofVec3f _pnt);
	void projectPoint(ofVec3f _pnt);
	int unprojectMode;
	ofFloatColor newFaceColor;

	ofCamera cam;
	ofVec3f camPosition;

	string prefix;
	void setPrefix(string _p){prefix = _p;}

	void updateGui();

	void goToAttract();

	int timeOfLastInteraction;
	bool bInAttract;
	void startMove(ofVec3f _pnt);
	void makeMove(ofVec3f _pnt);
	float getMainComponent(ofVec3f _pnt);
	bool bHaveAxis;
	SG_VECTOR v;
	double gestureAngles[6];
	bool bClicked;

	ofVec3f cubiePos;
	double lastDragDistance;

	ofVec3f cp0;
	ofVec3f cp1;
	ofVec3f cp2;
	ofVec3f cp3;
	ofVec3f cp4;
	ofVec3f cp5;
	ofVec3f cp6;

	//void drawPoints();

	string currentLanguage;
	void setLanguage(string _lang);

	ofLight light;
	int dragId;

	void setPage(string _page);
};
#endif /* defined(__Tgame__game__) */
