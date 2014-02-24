#include "history.h"
#include "sgCore.h"

history::history(int idcubie,  SG_VECTOR v, double d,bool di){
	id = idcubie;
	dir = di;
	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;
	deg = d;
}