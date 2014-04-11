#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"
#include "GuiNode.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "SubObEvent.h"
#include "CubieMesh.h"

#define MAX_DIST 800.0

class puzzle : public Observer{

public:
	puzzle(SG_VECTOR p, ofVec3f offset, int gSize);

	int id;

	void setup();
	void update();
	void draw();

	void exit();

	void loadPieces(sgCGroup **pcs, int selObjId, ofVec3f v);
	//void loadPieces(sgCGroup **pcs, int selObjId, ofVec3f v, float angleA, ofVec3f axistbA, int mode);

	int giveNumCubies();
	sgCGroup* cubieGroup;
	float numPieces;
	cubie **myCubies; //malloc numPieces of these
	ofVec3f cubiesOffset;

	SG_VECTOR pos;
	void move(SG_VECTOR p);

	SG_VECTOR rot;
	void rotate(SG_VECTOR r);
	void rotateTB(float angle, ofVec3f axis);
	ofVec3f qaxis; 
	float qangle; 

	typedef std::vector<int>OneDimension;
	typedef std::vector<OneDimension>TwoDimensions;
	typedef std::vector<TwoDimensions>ThreeDimensions;
	ThreeDimensions three_dim1;
	void rotateByIDandAxis(int id, SG_VECTOR axis,bool dir);
	void rotateByIDandAxis(int id, SG_VECTOR axis,bool dir,float angle);
	void rotateByIDandAxisNew(int id, SG_VECTOR axis, bool dir, float angle);


	void rearange3dArray(SG_VECTOR axis, int plane, bool dir);
	void rearrange3dArrayNew(int axisNum,int plane, bool dir);

	//face rotation bytwo ids
	int rotateTwoIds(int cubieA, int cubieB,bool inside);
	bool faceRotateB;

	void unDo(int id, SG_VECTOR axis, bool dir);

	bool pmoving;
	bool isMoving();

	int selected[36];
	int counter;
	int selX;
	int selY;
	int selZ;
	void decideMove();

	//color faces
	void colorFaces(int objectID);
	void colorFacesMenuPuzzle(int objectID,vector< ofVec3f > &menuUniqueNormals, vector< ofFloatColor > &vcolors);

	void colorCubiesBlackSides();
	void colorTorus();
	void colorTorusMenuPuzzle();
	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);

	int gridSize;

	/*
	Cubie picking - GuiNode stuff
	*/

	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent _event);

	bool isInside(int _x, int _y);
	void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);

	bool bDrawLine;
	bool bHaveLine;
	ofVec3f lineStart;
	ofVec3f lineStop;

	float maxDist;

	bool bHaveActiveCubie;
	bool bHaveRotationCubie;
	int activeCubie;
	Triangle activeTriangle;
	int rotationCubie;

	void doRotation();

	void changeFaceColor(ofVec3f _pnt, ofFloatColor _c);

	//   //current state of the rotation
	//ofQuaternion curRot;
	////a place to store the mouse position so we can measure incremental change
	//ofVec2f lastMouse;
	//void mouseDragged(int x, int y, int button);
	//void mousePressed(int x, int y, int button);

	bool bUnproject;
	ofVec3f mousePoint;
	ofVec3f unprojectedPoint;
	bool bHavePoint;
	
	void setMousePoint(ofVec3f _pnt);
	void checkCubiesForHit(ofVec3f _pnt);
	void dragInput(ofVec3f _pnt);
	ofVec3f getDir(ofVec3f _pnt);
	float getMainComponent(ofVec3f _pnt);
	bool bHaveAxis;
	SG_VECTOR v;
	void endRotation(){bHaveAxis = false; activeCubie = -1;}


	//void mousePressed(int x, int y, int button);
	//ofPoint unprojectPoint(ofVec3f _pnt);
	//void unprojectPoint();
	//double gestureAngles[6];
};

#endif /* defined(__Tpuzzle__puzzle__) */