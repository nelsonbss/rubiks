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
	vector<game *> myGames;

	myobject3D *objectDisplayed;
	cutter *myCutter;
	slicer *mySlicer;
	puzzle *myPuzzle;

	bool rotate;

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

	float radius;
	ofVec3f center;
	bool bShiny;
	bool bSmoothLighting;
	bool bPointLight, bSpotLight, bDirLight;
	bool bUseTexture;

	///for color changing //first version
	ofFloatColor pickColorMouse();
};
