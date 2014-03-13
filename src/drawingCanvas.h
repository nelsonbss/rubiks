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

	void exit();

	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	ofVec3f posCanvas;
	int width;
	int height;

	ofPolyline *myPolyline;
	ofPolyline *myPolyline2;
	bool poly2exists;
	bool drawingExists();
	ofVec2f lastMouse;
	ofVec2f firstMouse;
	void makeLine(ofVec2f mouse);
	ofPolyline* getPolyline();
};

#endif /* defined(__TdrawingCanvas__drawingCanvas__) */




