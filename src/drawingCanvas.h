#ifndef __TdrawingCanvas__drawingCanvas__
#define __TdrawingCanvas__drawingCanvas__

#include "sgCore.h"
#include "ofMain.h"

class drawingCanvas {
	
public:
	drawingCanvas(ofVec3f posCanvas, int width, int heigh);

	void setup();
	void update();
	void draw();

	ofVec3f posCanvas;
	int width;
	int height;
};

#endif /* defined(__TdrawingCanvas__drawingCanvas__) */




