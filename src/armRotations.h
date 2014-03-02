#ifndef __TarmRotations__armRotations__
#define __TarmRotations__armRotations__

#include "matrix.h"
#include "sgCore.h"
#include "ofMain.h"

class armRotations {
	
public:
	armRotations(ofVec3f axis);
	SG_VECTOR vector;
};

#endif /* defined(__TarmRotations__armRotations__) */