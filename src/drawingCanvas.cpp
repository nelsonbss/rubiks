#include "drawingCanvas.h"
#include "sgCore.h"

drawingCanvas::drawingCanvas(ofVec3f posCanvasi, int widthi, int heighti){
	posCanvas = posCanvasi;
	width = widthi;
	height = heighti;

	myPolyline = new ofPolyline();
}
//--------------------------------------------------------------
void drawingCanvas::setup(){
}
//--------------------------------------------------------------
void drawingCanvas::update(){
}
//--------------------------------------------------------------
void drawingCanvas::draw(){  
	//glPushMatrix();
	ofFill();
	ofSetColor(ofColor(255,255,0));
	//ofBox(posCanvas.x,posCanvas.y,posCanvas.z,width,height,0);
	ofSetColor(ofColor(255,0,0));
	myPolyline->draw();

	//glPopMatrix();
}
//--------------------------------------------------------------
void drawingCanvas::makeLine(ofVec2f mouse){
	if(mouse.distance(lastMouse) > 5){
		myPolyline->addVertex(mouse);
	}
}
//--------------------------------------------------------------
void drawingCanvas::exit(){
	free(myPolyline);
}
//--------------------------------------------------------------
void drawingCanvas::mouseDragged(int x, int y, int button){
	ofVec2f mouse(x,y);
	makeLine(mouse);
	lastMouse = mouse;
}
//--------------------------------------------------------------
void drawingCanvas::mousePressed(int x, int y, int button){
	//check if its inside the area to be able to draw
	if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
		if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
			lastMouse = ofVec2f(x,y);
			myPolyline->addVertex(lastMouse);
		}
	}
}
//--------------------------------------------------------------
void drawingCanvas::mouseReleased(int x, int y, int button){
	myPolyline->close();
}