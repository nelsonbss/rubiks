#include "matrix.h"
#include "sgCore.h"

matrix::matrix(SG_VECTOR v, double d,bool di){
	dir = di;
	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;
	deg = d;
}
//--------------------------------------------------------------