#ifndef __Tmatrix__matrix__
#define __Tmatrix__matrix__

#include "matrix.h"
#include "sgCore.h"

class matrix {
	
public:
	matrix(SG_VECTOR axis, double deg, bool di);
   
	bool dir;//direction of rotation true-> Clockwise, false->CC
	//SG_POINT point; //rotations
	double deg; //both YES
	SG_VECTOR vector; //YES
};

#endif /* defined(__Tmatrix__matrix__) */


