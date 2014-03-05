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
#include "ofxTrackball.h"

class testApp : public ofBaseApp, public Observer, public Subject{

public:
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
	void loadOBJfiles();
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
	sgCObject *sgTeapot;

	////////////////
	vector<game*> myGames;
	int currentGame;

	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	puzzle *myPuzzle;

	//bool rotate;

	////////////////rendering  and textures
	void initOFRender();
	void startOFLights();
	void updateOFLights();
	void stopOFLights();

	ofMesh myMesh;
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
	void update(string _eventName, SubObEvent* _event);
    string getAttr(const char* _key){return attrs[_key];}

	ofVec2f lastRightDragPos;

	float timeOfLastInteraction;
    float timeOutCounter;
	float timeOfLastInput;
    float inputDelayTime;
	float radius;
	ofVec3f center;
	bool bShiny;
	bool bSmoothLighting;
	bool bPointLight, bSpotLight, bDirLight;
	bool bUseTexture;

	///for color changing //first version
	ofFloatColor pickColorMouse();

	GestureManager *gm;
};
