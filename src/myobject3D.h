#ifndef __Tmyobject3D__myobject3D__
#define __Tmyobject3D__myobject3D__

#include "ofMain.h"
#include "sgCore.h"

class myobject3D {
	
public:
	myobject3D (SG_VECTOR pos, SG_VECTOR t);
    
	void setup();
	void update();
	void draw();
	void exit();

	void loadObject(sgC3DObject *obj, int ID); //it will load a sgCore lib object: torus, box...
	sgC3DObject* getObject();//it will return the object, i.e to be used in boolean

	int objectId; //the id of the 3d selected object, the id comes from the main app
	sgC3DObject *object;
	sgC3DObject *temp;//the original will be in 0,0,0 to be cut. the temp will be moved around and be drawn.
	ofMesh myMesh;// this will be used to draw the object, 
	ofVbo myVbo;

	SG_VECTOR pos;
	SG_VECTOR tempPos;
	ofVec3f armRot;
	void applyArmRotations(ofVec3f v, float angleA, ofVec3f axistbA, int mode);
	int mode;
	float quatAngle;
	SG_VECTOR rotVQuat;
	//mode:
	//1 is for regular euler angle rotation inside rotateSlicer
	//2 is for quaternion rotate angle and vector info
};

#endif /* defined(__Tmyobject3D__myobject3D__) */
