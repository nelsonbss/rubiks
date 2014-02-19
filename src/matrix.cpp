#include "matrix.h"
#include "sgCore.h"

matrix::matrix(int t,SG_POINT p, SG_VECTOR v, float d){
	type = t;
	if(type == 1){
		point = p;
	}else{
		SG_POINT n = {0,0,0};
		point = n;
	}
	vector = v; 
	deg = d;
}
//--------------------------------------------------------------