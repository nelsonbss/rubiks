#ifndef __Tmatrix__matrix__
#define __Tmatrix__matrix__

#include "matrix.h"
#include "ofMain.h"
#include "sgCore.h"

class matrix {
	
public:
	matrix(int type,SG_POINT point, SG_VECTOR axis, float deg);
    
	int type; //1 for rotation, 2 for translation
	SG_POINT point; //rotations YES, translations NO
	SG_VECTOR vector; //both YES
	float deg; //both YES
};

#endif /* defined(__Tmatrix__matrix__) */


