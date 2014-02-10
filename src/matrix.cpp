#include "matrix.h"
#include "sgCore.h"

matrix::matrix(SG_VECTOR v, float d,bool di){
	dir = di;
	vector = v; 
	deg = d;
}
//--------------------------------------------------------------