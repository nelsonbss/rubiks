#include "drawingCanvas.h"
#include "sgCore.h"

drawingCanvas::drawingCanvas(ofVec3f posCanvasi, int widthi, int heighti){
	posCanvas = posCanvasi;
	width = widthi;
	height = heighti;

	myPolyline = new ofPolyline();
	myPolyline2 = new ofPolyline();

	poly2exists = false;
	closed = false;
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
	bool intersect = false;
	vector< ofPoint > points = myPolyline->getVertices();
	int size = points.size();

	if(mouse.distance(lastMouse) > 3){
		//check for intersection first!!
		if(size > 10){
			//check lines on the ofPolyline
			for(int i=0; i< myPolyline->size()-1; i ++){
				//only check if line is close to mouse
				//if((mouse.distance(points[i]) < 6) || (mouse.distance(points[i+1]) < 6) ){
					intersect = intersection(points[i],points[i+1],lastMouse,mouse);
					if(intersect){
						//intersection found
						i = myPolyline->size();//escape for loop
					}
				//}
			}
		}
		if(!intersect){
			//no intersection
			myPolyline->addVertex(mouse);
			//fix offset of point since they are in in the "middle" of the screen
			//they have to be where the slicing takes place
			myPolyline2->addVertex(ofVec2f(mouse.x-posCanvas.x,mouse.y-posCanvas.y));
		}else{
			//YES intersection
			cout << "intersetion" << endl;
		}
	}
}
//--------------------------------------------------------------
bool drawingCanvas::intersection(ofVec2f line1A,ofVec2f line1B,ofVec2f line2A,ofVec2f line2B){
	float s1;
	float s2;
	s1 = ((line1B.x-line1A.x)*(line2A.y-line1A.y))-((line1B.y-line1A.y)*(line2A.x-line1A.x));
	s2 = ((line1B.x-line1A.x)*(line2B.y-line1A.y))-((line1B.y-line1A.y)*(line2B.x-line1A.x));
	if(s1 == s2){
		return false;
	}else{
		return true;
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
	if(closed == false){
		//check if its inside the area to be able to draw
		if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
			if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
				lastMouse = ofVec2f(x,y);
				firstMouse = lastMouse; //to be able to close shape
				myPolyline->addVertex(lastMouse);//first vertex
				myPolyline2->addVertex(ofVec2f(lastMouse.x-posCanvas.x,lastMouse.y-posCanvas.y));
				poly2exists = true;
			}
		}
	}
}
//--------------------------------------------------------------
void drawingCanvas::mouseReleased(int x, int y, int button){
	//add last point
	myPolyline->addVertex(firstMouse);//close last vertex, with first vertex
	myPolyline2->addVertex(ofVec2f(firstMouse.x-posCanvas.x,firstMouse.y-posCanvas.y));//polyline2 is the one that is on slicing position
	closed = true;
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