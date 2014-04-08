#ifndef __TdrawingCanvas__drawingCanvas__
#define __TdrawingCanvas__drawingCanvas__

#include "sgCore.h"
#include "ofMain.h"
#include "Observer.h"
#include "SubObEvent.h"
#include "SubObMediator.h"

class drawingCanvas: public Observer{

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

	//ofImage myCanvasImage; //DrawingGrid_555x35.png

	ofPolyline *myPolyline;//the user sees
	ofPolyline *myPolyline2;//its on the slicing position
	ofPolyline *myDummyLine;
	bool poly2exists;
	bool drawingExists();
	bool closed;
	ofVec2f lastMouse;
	ofVec2f firstMouse;
	void makeLine(ofVec2f mouse);
	ofPolyline* getPolyline();

	int intersection(ofVec2f line1A,ofVec2f line1B,ofVec2f line2A,ofVec2f line2B);
	bool same_sign(float a, float b);
	bool drawDummy;
	ofVec2f dummyA;
	ofVec2f dummyB;

	void update(string _eventName, SubObEvent* _event);
	bool bDrawing;

	ofImage myCanvasImage; //DrawingGrid_555x35.png
};

#endif /* defined(__TdrawingCanvas__drawingCanvas__) */




