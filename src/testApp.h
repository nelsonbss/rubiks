#pragma once

#include "ofMain.h"
#include "sgCore.h"
#include "GuiConfigurator.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "SubObEvent.h"
#include "SceneManager.h"
#include "game.h"
#include "GestureManager.h"
#include <math.h>
#include "menuPuzzle.h"
#include "ofxAssimpModelLoader.h"
#include "hiddenButton.h"
#include <vector>

class testApp : public ofBaseApp, public Observer, public Subject{

public:
	testApp();
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void updateMouseState(const char * _state, int _x, int _y, int _button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	///OBJ file loading and convertion
	//void loadOBJfiles();
	//ofxAssimpModelLoader bunny;
	//sgCObject *sgBunny;

	vector <game*> myGames;


	map<int, sgCObject*> objectsMP;
	int numObjects;
	ofxAssimpModelLoader loaderG;
	void loadObjDirG(string _path);
	/////////////////////////////////////////games
	/////////////////////////puzzles
	SG_VECTOR posGame;
	float width;
	float height;

	SG_VECTOR slicingPos;  //where it gets sliced
	//vector<myobject3D*> objectDisplayedVector;
	myobject3D *objectToMakePuzzle;
	cutter *myCutter;
	slicer *mySlicer;
	armature *myArmature;
	
	//bool rotate;
		////////////////////////////////////////middle puzzles
	SG_VECTOR middlePuzzlePos;
	vector<menuPuzzle*> middlePuzzles;
	puzzle **myPuzzles;
	//vector<puzzle> myPuzzles;
	menuPuzzle *puzzleDisplayed;
	int puzzleCounter; 
	ofVec3f offsetSlicer;
	ofVec3f rotateSlicer;

	
	
	
	map<string,int> gameIds;
	map<int, string> gameTags;



	////////////////rendering  and textures
	void initOFRender();
	void startOFLights();
	void updateOFLights();
	void stopOFLights();

	//ofMesh myMesh;
	ofLight pointLight;
	ofLight spotLight;
	ofLight directionalLight;

	ofMaterial material;
	ofImage ofLogoImage;

	//face rotations
	/*float tempDeg;
	bool faceRotateC;
	bool faceRotateCC;*/

	//subob stuff
	map<string,string> attrs;
	void update(string _subName, Subject* _sub);
	void update(string _eventName, SubObEvent _event);
	string getAttr(const char* _key){return attrs[_key];}

	ofVec2f lastRightDragPos;

	float timeOfLastInteraction;
	float timeOutCounter;
	float timeOfLastInput;
	float inputDelayTime;
	//float radius;
	//ofVec3f center;
	//bool bShiny;
	//bool bSmoothLighting;
	//bool bPointLight, bSpotLight, bDirLight;
	//bool bUseTexture;

	GestureManager gm;
	int touchId;
	int nextTouchId;
	int touchIdOffset;



	//SubObEvent ev;

	ofPoint unprojectPoint(ofVec3f pnt);
	double gestureAngles[6];
	ofLight light;


	//hidden buttons
	hiddenButton *hb1;
	bool hb1Flag;
	bool hb1Flag2;
	float shutDownTime;
	hiddenButton *hb2;
	bool hb2Flag;
};
