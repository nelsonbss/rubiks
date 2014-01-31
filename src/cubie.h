#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "sgCore.h"

class cubie {
	
public:
	cubie();
    
	void setup();
	void update();
	void draw();

	sgCGroup *objects;

	void setObjects(sgCGroup *objs);
};

#endif /* defined(__Tcubie__cubie__) */