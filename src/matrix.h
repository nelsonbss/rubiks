#ifndef __Tmatrix__matrix__
#define __Tmatrix__matrix__

#include "matrix.h"
#include "ofMain.h"
#include "sgCore.h"

class matrix {
	
public:
	matrix(SG_VECTOR axis, float deg,bool di);
   
	bool dir;//direction of rotation true-> Clockwise, false->CC
	//SG_POINT point; //rotations
	SG_VECTOR vector; //YES
	float deg; //both YES
};

#endif /* defined(__Tmatrix__matrix__) */


