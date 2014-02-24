#ifndef __Thistory__history__
#define __Thistory__history__

#include "matrix.h"
#include "sgCore.h"

class history {
	
public:
	history(int idcubie, SG_VECTOR axis, bool di);
   
	int id;
	bool dir;//direction of rotation true-> Clockwise, false->CC
	SG_VECTOR vector; //YES
};

#endif /* defined(__Thistory__history__) */