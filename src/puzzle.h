#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"
#include "GuiNode.h"

#define MAX_DIST 800.0

class puzzle : public GuiNode{

public:
	puzzle(SG_VECTOR p, ofVec3f offset);

	int id;

	void setup();
	void update();
	void draw();

	void exit();

	void loadPieces(sgCGroup **pcs, int selObjId, ofVec3f v);
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
	void rearange3dArray(SG_VECTOR axis, int plane, bool dir);
	//face rotation bytwo ids
	int rotateTwoIds(int cubieA, int cubieB,bool inside);

	void unDo(int id, SG_VECTOR axis, bool dir);

	bool moving;
	bool isMoving();

	//color faces
	void colorFaces(int objectID);
	void colorCubiesBlackSides();
	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);

	/*
	Cubie picking - GuiNode stuff
	*/

	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent* _event);

	bool isInside(int _x, int _y);
	void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);

	bool bDrawLine;
	bool bHaveLine;
	ofVec3f lineStart;
	ofVec3f lineStop;

	float maxDist;

	bool bHaveActiveCubie;
	int activeCubie;

	//   //current state of the rotation
	//ofQuaternion curRot;
	////a place to store the mouse position so we can measure incremental change
	//ofVec2f lastMouse;
	//void mouseDragged(int x, int y, int button);
	//void mousePressed(int x, int y, int button);
};

#endif /* defined(__Tpuzzle__puzzle__) */