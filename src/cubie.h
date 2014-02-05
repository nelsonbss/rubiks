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
	void unDraw();

	void rotate();

	sgCGroup *objects;
	float numObjs;
	sgCObject **objectList;
	float move;

	void setObjects(sgCGroup *objs);
	sgCGroup* copyObjects();
};

#endif /* defined(__Tcubie__cubie__) */