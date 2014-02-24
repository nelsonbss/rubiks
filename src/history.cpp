#include "history.h"
#include "sgCore.h"

history::history(int idcubie,  SG_VECTOR v,bool di){
	id = idcubie;
	dir = di;
	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;
}