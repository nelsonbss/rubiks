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
	glPushMatrix();
	glTranslatef(posCanvas.x,posCanvas.y,posCanvas.z);
	
	ofFill();
	ofSetColor(ofColor(255,255,255));
	ofBox(0,0,0,width,height,0);
	glPopMatrix();
}
//--------------------------------------------------------------
void makeLine(ofVec2f mouse){

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
	lastMouse = ofVec2f(x,y);
}
//--------------------------------------------------------------
void drawingCanvas::mouseReleased(int x, int y, int button){
}