#ifndef __TmenuPuzzle__menuPuzzle__
#define __TmenuPuzzle__menuPuzzle__

#include "puzzle.h"
#include "sgCore.h"
#include "GuiNode.h"
#include "CubieMesh.h"

class GuiNode;

class menuPuzzle : public GuiNode{

public:
	menuPuzzle (SG_VECTOR pos, SG_VECTOR t, int ID);
	menuPuzzle ();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	void loadPuzzle(puzzle *inpuzzle, int station);
	puzzle *myMenuPuzzle;

	puzzle *MenuPuzzleBR;
	puzzle *MenuPuzzleBL;
	puzzle *MenuPuzzleTR;
	puzzle *MenuPuzzleTL;

	puzzle *getPuzzle(string gameTag);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void draw();
	void setup();
	void update();
	void exit();
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	int id;
	void loadObjectMP(sgC3DObject *obj, int ID,vector< ofVec3f > ObjectUniqueNormals); //it will load a sgCore lib object: torus, box...
	//sgC3DObject* getObject();//it will return the object, i.e to be used in boolean

	int objectId; //the id of the 3d selected object, the id comes from the main app
	//sgC3DObject *object;
	sgC3DObject *temp;//the slicing object will be in 0,0,0 to be cut. the temp will be moved around and be drawn.
	CubieMesh myMesh;// this will be used to draw the object, 
	ofVbo myVbo;

	vector< ofVec3f > ObjectUniqueNormals;

	////to controll colors on displayed object and displayed puzzle for that object
	////so that both objects have the same color
	vector< ofFloatColor > colorsVMenu;
	vector< ofVec3f > uniqueNormals;
	ofVec3f offsetSlicer;
	ofVec3f rotateSlicer;
	SG_VECTOR position;
	SG_VECTOR tempPos;
	ofVec3f drawPos;
	SG_VECTOR tempSize;
	void colorFacesMenu();
	ofVec3f armRot;
	////void applyArmRotations(ofVec3f v);

	bool isInside(int _x, int _y);

	///*Gui Stuff*/
	void nodeInit();
	void nodeDraw(); 
	void nodeExecute();
	void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);
	bool processMouse(int _x, int _y, int _state){return false;}
	//virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent _event);

	bool bHidden;
	bool bActive;
	bool bReadyForInput;
	ofVec3f startPos;

	ofRectangle viewport;
	bool isActive(){return true;}
	bool bWatchTime;
	//////ofxTimer *timer;
	bool bSend;
	int timeOfLastInteraction;
	string targetGame;

	ofLight light;
};

#endif /* defined(__TmenuPuzzle__menuPuzzle__) */