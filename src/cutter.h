#ifndef __Tcutter__cutter__
#define __Tcutter__cutter__

#include "ofMain.h"
#include "sgCore.h"

class cutter {
public:
	cutter(float thick, float tam, float tamPiece);
    
	void setup();
	void update();
	void draw();

	sgCGroup* getCutter();//it will return the object, i.e to be drawn
   
	float cutterThick; // ideally it will be 0.1; how thick is the "blade"
	float cutterSize; //width and height of the "blade"
	float tamCubie;
	//whole cutter coordinates
	float posX;
	float posY;


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
};
#endif /* defined(__Tcutter__cutter__) */
