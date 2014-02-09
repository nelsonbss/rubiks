#include "matrix.h"
#include "sgCore.h"

matrix::matrix(int t, SG_VECTOR v, float d,bool di){
	//type = t;
	//if(type == 1){
		//point = p;
		dir = di;
	//}else{
		//SG_POINT n = {0,0,0};
		//point = n;
		//dir = false;
	//}
	vector = v; 
	deg = d;
}
//--------------------------------------------------------------