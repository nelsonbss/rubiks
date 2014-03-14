#include "drawingCanvas.h"
#include "sgCore.h"

drawingCanvas::drawingCanvas(ofVec3f posCanvasi, int widthi, int heighti){
	posCanvas = posCanvasi;
	width = widthi;
	height = heighti;

	myPolyline = new ofPolyline();
	myPolyline2 = new ofPolyline();

	poly2exists = false;
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
	ofNoFill();
	ofSetColor(ofColor(0,0,0));
	//the coordinates for this box are from the center!!!! not the left/up corner!!!
	ofBox(posCanvas.x,posCanvas.y,posCanvas.z,width,height,0);
	ofSetColor(ofColor(255,0,0));
	myPolyline->draw();

	//glPopMatrix();
}
//--------------------------------------------------------------
void drawingCanvas::makeLine(ofVec2f mouse){
	if(mouse.distance(lastMouse) > 3){
		myPolyline->addVertex(mouse);
		//fix offset of point since they are in in the "middle" of the screen
		//they have to be where the slicing takes place
		myPolyline2->addVertex(ofVec2f(mouse.x-posCanvas.x,mouse.y-posCanvas.y));
	}
}
//--------------------------------------------------------------
void drawingCanvas::mouseDragged(int x, int y, int button){
	if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
		if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
			ofVec2f mouse(x,y);
			makeLine(mouse);
			lastMouse = mouse;
		}
	}
}
//--------------------------------------------------------------
void drawingCanvas::mousePressed(int x, int y, int button){
	//check if its inside the area to be able to draw
	if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
		if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
			lastMouse = ofVec2f(x,y);
			firstMouse = lastMouse;
			myPolyline->addVertex(lastMouse);
			myPolyline2->addVertex(ofVec2f(lastMouse.x-posCanvas.x,lastMouse.y-posCanvas.y));
			poly2exists = true;
		}
	}
}
//--------------------------------------------------------------
void drawingCanvas::mouseReleased(int x, int y, int button){
	//add last point
	myPolyline->addVertex(firstMouse);
	myPolyline2->addVertex(ofVec2f(firstMouse.x-posCanvas.x,firstMouse.y-posCanvas.y));
	//myPolyline->close();
}
//--------------------------------------------------------------
ofPolyline* drawingCanvas::getPolyline(){
	poly2exists = false;
	return myPolyline2;
}
//--------------------------------------------------------------
bool drawingCanvas::drawingExists(){
	return poly2exists;
}
//--------------------------------------------------------------
void drawingCanvas::exit(){
	free(myPolyline);
	if(poly2exists){
	free(myPolyline2);
	}
}