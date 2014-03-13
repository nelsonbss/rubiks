#include "drawingCanvas.h"
#include "sgCore.h"

drawingCanvas::drawingCanvas(ofVec3f posCanvasi, int widthi, int heighti){
	posCanvas = posCanvasi;
	width = widthi;
	height = heighti;
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
