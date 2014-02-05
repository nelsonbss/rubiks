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

	sgCGroup *objects;
	float numObjs;
	sgCObject **objectList;
	
	void setObjects(sgCGroup *objs);
	sgCGroup* copyObjects();

	float moveH;
	float moveV;
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	void rotateUp();
};

#endif /* defined(__Tcubie__cubie__) */