#ifndef __Tcubie__cubie__
#define __Tcubie__cubie__

#include "ofMain.h"
#include "sgCore.h"

class cubie {
	
public:
	cubie(float x,float y);
    
	void setup();
	void update();
	void draw();
	void unDraw();

	sgCGroup *objects;
	float numObjs;
	sgCObject **objectList;
	
	void setObjects(sgCGroup *objs);
	sgCGroup* copyObjects();

	float posX;
	float posY;
	void moveH(float px);
	void moveV(float py);
	
	float rotH;
	float rotV;
	void rotateH(float radH);
	void rotateV(float radV);
};

#endif /* defined(__Tcubie__cubie__) */