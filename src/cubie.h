#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "GuiNode.h"
#include "sgCore.h"
#include "matrix.h"

#include <vector>

enum{ROTATE_UP, ROTATE_DOWN, ROTATE_LEFT, ROTATE_RIGHT};

class cubie: public GuiNode{
	
public:
	cubie(float x,float y,float z,int id, int selObjId, ofVec3f offset);
    
	void setup();
	void update();
	void draw();
	//void unDraw();// has legacy of how to detach from scene
	void exit();

	int id; //id of the cuie to be used on the selection of the cuie to rotate
	int getId();

	int selectedObjectID; // keeps the id of the object selected to become a puzzle, used for coloring

	sgCGroup *objects;
	float numObjs;
	float getNumObjs();
	sgC3DObject **objectList;
	vector<ofMesh> myMeshs; // this will be used to store the objet to be drawn 
	vector<ofVbo>  myVbos;

	float color;

	void setObjects(sgCGroup *objs,int cubieId,ofVec3f v);
	
	//this vector (point) is the offset of the cutter, to rotate correctly (when cutter was not rotated) when its offcentered by user on the armature stage
	ofVec3f pointRotate;
	
	///whole puzzle
	SG_VECTOR pos;
	void move(SG_VECTOR p);

	//armature rotations
	void undoArmRotations(ofVec3f v);
	ofVec3f armRotations;

	//this is vector of matrix objects that have all the transformations for each cubie.
	//its a vector since we don't know how many transformations a cubie is going to have
	vector<matrix> myMatrix; 
	void faceRotate(SG_VECTOR axis, bool dir);
	bool moving;
	bool isMoving();

	//float ct1;
	//float ct2;
	float animTime;

	bool sample;
	double rotXa;

	//undo
	bool undoing;
	void unDo(SG_VECTOR axis,bool di);

	///color change
	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc);
	void colorBlackSides(int cubieId, float playRoom);
	
	/*
	Paul's
	*/

	bool processMouse(int _x, int _y, int _state){return false;}
	void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);
	void update(string _subName, Subject* _sub){}
	void update(string _eventName, SubObEvent* _event);
	void execute();

	bool bDraw;
	int rotationDirection;

	bool bRotate;
	bool getRotate();
	int getRotationDirection(){return rotationDirection;}
	void setRotate(bool _rotate);

	void getCentroid();
	void projectCentroid();
	ofVec3f centroid3d;
	ofVec3f centroid2d;
	ofVec3f centroidColor;
};

#endif /* defined(__Tcubie__cubie__) */