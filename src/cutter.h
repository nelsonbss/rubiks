#ifndef __Tcutter__cutter__
#define __Tcutter__cutter__

#include "ofMain.h"
#include "sgCore.h"

class cutter {
public:
	cutter(float thick, float tam, float tamPiece,float numCutrc, float x, float y,float z);
    
	void setup();
	void update();
	void draw();
	void exit();

	//sgCGroup* getCutterPlanes();//it will return the object, i.e to be drawn
 //   sgCGroup* getCutterCubes();

	void addGroupToScene(sgCGroup *group);
	void deleteGroupFromScene(sgCGroup *group);
	//sgCGroup* getGroup();

	float cutterThick; // ideally it will be 0.1; how thick is the "blade"
	float cutterSize; //width and height of the "blade"
	float tamCubie;
	//whole cutter coordinates
	ofVec3f posCutter;
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	float rotH;
	float rotV;
	void rotateHright();
	void rotateHleft();
	void rotateVup();
	void rotateVdown();

	float numCutter;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///cutting planes
	sgCBox *x1; //cutting "planes"
	SG_VECTOR transPlaneX1;//this is to do the translation of the plane, and remember its position for slicing
	sgCBox *x2;
	SG_VECTOR transPlaneX2;
	sgCBox *y1;
	SG_VECTOR transPlaneY1;
	sgCBox *y2;
	SG_VECTOR transPlaneY2;
	sgCBox *z1;
	SG_VECTOR transPlaneZ1;
	sgCBox *z2;
	SG_VECTOR transPlaneZ2;

	sgCObject  **planes;
    sgCGroup *allPlanes;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////cutting cubes
	SG_VECTOR centerCube; 
	float infinity;
	sgCBox *c1; //cutting "planes"
	SG_VECTOR posCube1;//this is to do the translation of the plane, and remember its position for slicing
	sgCBox *c2;
	SG_VECTOR posCube2;
	sgCBox *c3;
	SG_VECTOR posCube3;
	sgCBox *c4;
	SG_VECTOR posCube4;
	sgCBox *c5;
	SG_VECTOR posCube5;
	sgCBox *c6;
	SG_VECTOR posCube6;
	sgCBox *c7;
	SG_VECTOR posCube7;
	sgCBox *c8;
	SG_VECTOR posCube8;
	sgCBox *c9;
	SG_VECTOR posCube9;
	sgCBox *c10;
	SG_VECTOR posCube10;
	sgCBox *c11;
	SG_VECTOR posCube11;
	sgCBox *c12;
	SG_VECTOR posCube12;
	sgCBox *c13;
	SG_VECTOR posCube13;
	sgCBox *c14;
	SG_VECTOR posCube14;
	sgCBox *c15;
	SG_VECTOR posCube15;
	sgCBox *c16;
	SG_VECTOR posCube16;
	sgCBox *c17;
	SG_VECTOR posCube17;
	sgCBox *c18;
	SG_VECTOR posCube18;
	sgCBox *c19;
	SG_VECTOR posCube19;
	sgCBox *c20;
	SG_VECTOR posCube20;
	sgCBox *c21;
	SG_VECTOR posCube21;
	sgCBox *c22;
	SG_VECTOR posCube22;
	sgCBox *c23;
	SG_VECTOR posCube23;
	sgCBox *c24;
	SG_VECTOR posCube24;
	sgCBox *c25;
	SG_VECTOR posCube25;
	sgCBox *c26;
	SG_VECTOR posCube26;
	sgCBox *c27;
	SG_VECTOR posCube27;
	sgCObject  **cubes;
    sgCGroup *allCubes;
	////////////////////////////////////////////////////////////////////////////////////////////////////

};
#endif /* defined(__Tcutter__cutter__) */
