#pragma once

#include "ofMain.h"
#include "sgCore.h"
#include "game.h"
#include <vector>

class testApp : public ofBaseApp{

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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	////////////////
	void initScene();
	void drawElements();
	////////////////

	////////////////
	vector<game> myGames;
	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	puzzle *myPuzzle;
	////////////////

	bool puzzleExists;

	bool updatePuzzle;

	bool makeCut;
	bool drawCuts;
	bool drawCuts1;
	bool draw3dObject;
	//move all puzzle: every cubbie moves the same way
	bool movePRight;
	bool movePLeft;
	bool movePUp;
	bool movePDown;
	//rotate all puzzlw: every cubbie rotates the same way
	bool rotatePHright;
	bool rotatePHleft;
	bool rotatePVup;
	bool rotatePVdown;

	//face rotations
	float tempDeg;
	bool faceRotateC;
	bool faceRotateCC;
	bool faceRotateCy;
	bool faceRotateCCy;
	bool faceRotateCz;
	bool faceRotateCCz;

	int randcubie;

	float ct1;
	float ct2;
	float deg;
	////////////////rendering  and textures
	void initOFRender();
	ofMesh myMesh;
	ofLight pointLight;
	ofLight spotLight;
	ofLight directionalLight;

	ofMaterial material;
	ofImage ofLogoImage;

	float radius;
	ofVec3f center;
	bool bShiny;
	bool bSmoothLighting;
	bool bPointLight, bSpotLight, bDirLight;
	bool bUseTexture;
};
