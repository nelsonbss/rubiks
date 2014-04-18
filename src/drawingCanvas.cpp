#include "drawingCanvas.h"
#include "sgCore.h"

drawingCanvas::drawingCanvas(ofVec3f posCanvasi, int widthi, int heighti){
	posCanvas = posCanvasi;
	width = widthi;
	height = heighti;
	
	//myPolyline = new ofPolyline();
	//myPolyline2 = new ofPolyline();

	poly2exists = false;
	closed = false;
	drawDummy = false;

	SubObMediator::Instance()->addObserver("ibox:1", this);
	SubObMediator::Instance()->addObserver("ibox:0", this);

	bDrawing = false;

	myCanvasImage.loadImage("drawingGrid.png");
}

drawingCanvas::drawingCanvas(){

	poly2exists = false;
	closed = false;
	drawDummy = false;

	SubObMediator::Instance()->addObserver("ibox:1", this);
	SubObMediator::Instance()->addObserver("ibox:0", this);

	bDrawing = false;

	myCanvasImage.loadImage("drawingGrid.png");
}

void drawingCanvas::reset(){
	closed = false;
	myPolyline.clear();
	myPolyline2.clear();
}

//--------------------------------------------------------------
void drawingCanvas::setup(){
}

void drawingCanvas::setViewport(ofRectangle _vp){
	vp = _vp;
	drawArea.x = (vp.width / 2) - 100;
	drawArea.y = (vp.height / 2) - 100;
	drawArea.width = 200;
	drawArea.height = 200;
}
//--------------------------------------------------------------
void drawingCanvas::update(){
}

ofVec2f drawingCanvas::getRealPoint(ofVec2f _p){
	//return ofVec2f(_p.x - drawArea.x - vp.x - drawArea.width / 2,(_p.y - drawArea.y - vp.y - drawArea.height / 2));
	//ofVec2f realPoint(_p.x - vp.x - drawArea.x,vp.height - (_p.y - vp.y));
	ofVec2f realPoint((_p.x - (vp.x + drawArea.x) - drawArea.width / 2),(/*drawArea.height - */(_p.y - (vp.y + drawArea.y)) - drawArea.height / 2));
	return realPoint;
}

//--------------------------------------------------------------
void drawingCanvas::draw(){  
	ofDisableDepthTest();
	glPushMatrix();
	ofTranslate(drawArea.x, drawArea.y,0);
	ofNoFill();
	ofSetColor(ofColor(1,0,0));
	ofSetLineWidth(2);
	//the coordinates for this box are from the center!!!! not the left/up corner!!!
	//myCanvasImage.draw(50,-50,0,400,400);
	//ofBox(posCanvas.x,posCanvas.y,posCanvas.z,width,height,0);
	//ofRect(drawArea.x,drawArea.y,drawArea.width,drawArea.height);

	myCanvasImage.draw(0,0,drawArea.width, drawArea.height);

	ofFill();
	ofSetColor(ofColor(0,255,0,255));
	ofPushMatrix();
	ofTranslate(drawArea.width / 2, drawArea.height / 2, 0);
	myPolyline.draw();
	if(drawDummy){
		ofSetColor(ofColor(255,255,255,255));
		ofSetLineWidth(5);
		myDummyLine.draw();

	//	//ofLine(dummyA,dummyB);
	}
	ofPopMatrix();
	/*ofCircle(250,150,0, 10.0);
	ofSetColor(ofColor(255,0,0,255));
	ofCircle(0,0,0, 10.0);*/
	glPopMatrix();
	ofEnableDepthTest();

}

void drawingCanvas::update(string _eventName, SubObEvent _event){
	if(_eventName == "ibox:1"){
		int phase = _event.getArg("phase")->getInt();
		cout << "puzzle phase = " << phase << endl;
		
		if(phase == 0){
			ofVec2f pos = _event.getArg("absPos")->getVec2();
			mousePressed(pos.x, pos.y, 0);
		}
		if(phase == 1){
			ofVec2f pos = _event.getArg("absPos")->getVec2();
			mouseDragged(pos.x, pos.y, 0);
		}
	}
	if(_eventName == "ibox:0"){
		if(bDrawing){
			ofVec2f pos = _event.getArg("absPos")->getVec2();
			mouseReleased(pos.x,pos.y,0);
		}
	}
}

//--------------------------------------------------------------
void drawingCanvas::makeLine(ofVec2f mouse){
	int intersect=0;
	vector< ofPoint > points = myPolyline.getVertices();
	int size = points.size();
	if(mouse.distance(lastMouse) > 2){
		//check for intersection first!!
		if(size > 10){
			//check lines on the ofPolyline
			if(drawDummy==false){
				for(int i=0; i< myPolyline.size()-2; i ++){
					//only check if line is close to mouse
					//if((points[i].x > mouse.x-6) && (points[i].x < mouse.x+6)){
					//	if((points[i].y > mouse.y-6) && (points[i].y < mouse.y+6)){
					intersect = intersection(points[i],points[i+1],getRealPoint(lastMouse),getRealPoint(mouse));
					if(intersect == 2){
						//intersection found
						i = myPolyline.size();//escape for loop
					}
					//	}
					//}
				}
				if(intersect == 0){
					//no intersection
					//myPolyline.addVertex(ofVec2f(mouse.x,mouse.y));
					//cout << "MX: " << mouse.x << "   my:  " << mouse.y << endl;
					//fix offset of point since they are in in the "middle" of the screen
					//they have to be where the slicing takes place
					//////////////////////myPolyline2->addVertex(ofVec2f(mouse.x-posCanvas.x,mouse.y-posCanvas.y));
					//cout << "adding point " << realPoint.x << ", " << realPoint.y << endl;
					myPolyline2.addVertex(getRealPoint(mouse));
					myPolyline.addVertex(getRealPoint(mouse));
					drawDummy = false;
				}else if(intersect == 2){
					//YES intersection
					//draw the segment to the mouse but not saving it into the polyline
					drawDummy = true;
					//myDummyLine = new ofPolyline();//////////////////////////////////////////////////////////////remember to dlete this *memory
					myDummyLine.clear();
					myDummyLine.addVertex(points[points.size()-1]);
					myDummyLine.addVertex(getRealPoint(mouse));
				}
			}else{
				//do comparison wih dummyline, until there is no intersection with dummy line, there is no more adition to the real polyline
				/*intersect = intersection(,lastMouse,mouse);*/
				vector< ofPoint > pointsDummy = myDummyLine.getVertices();
				for(int i=0; i< myPolyline.size()-2; i ++){
					//only check if line is close to mouse
					intersect = intersection(points[i],points[i+1],pointsDummy[0],pointsDummy[1]);
					if(intersect == 2){
						//intersection found
						i = myPolyline.size();//escape for loop
					}
				}
				if(intersect == 2){
					//YES intersection
					//draw the segment to the mouse but not saving it into the polyline
					drawDummy = true;
					myDummyLine.clear();
					myDummyLine.addVertex(points[points.size()-1]);
					myDummyLine.addVertex(getRealPoint(mouse));
				}else{
					//no intersection
					//return to real polyline
					drawDummy = false;
					myDummyLine.clear();
				}
			}
		}else{
			//not looking for intersection
			myPolyline.addVertex(getRealPoint(mouse));
			//fix offset of point since they are in in the "middle" of the screen
			//they have to be where the slicing takes place
			////////////////////////myPolyline2->addVertex(ofVec2f(mouse.x-posCanvas.x,mouse.y-posCanvas.y));
			myPolyline2.addVertex(getRealPoint(mouse));
			drawDummy = false;
		}
	}
}
//--------------------------------------------------------------
int drawingCanvas::intersection(ofVec2f lineA1,ofVec2f lineA2,ofVec2f lineB1,ofVec2f lineB2){
	float s1;
	float s2;
	//this check if points of second line segment are on the same side of the first line segment or not
	//s1 = ((lineA2.x-lineA1.x)*(lineB1.y-lineA1.y))-((lineA2.y-lineA1.y)*(lineB1.x-lineA1.x));
	//s2 = ((lineA2.x-lineA1.x)*(lineB2.y-lineA1.y))-((lineA2.y-lineA1.y)*(lineB2.x-lineA1.x));
	//if(s1 == s2){
	//	return false;
	//}else{
	//	return true;
	//}

	//////better approach
	float a1, a2, b1, b2, c1, c2;
	float r1, r2 , r3, r4;
	float denom, offset, num;

	// Compute a1, b1, c1, where line joining points 1 and 2
	// is "a1 x + b1 y + c1 = 0".
	a1 = lineA2.y - lineA1.y;
	b1 = lineA1.x - lineA2.x;
	c1 = (lineA2.x * lineA1.y) - (lineA1.x * lineA2.y);

	// Compute r3 and r4.
	r3 = ((a1 * lineB1.x) + (b1 * lineB1.y) + c1);
	r4 = ((a1 * lineB2.x) + (b1 * lineB2.y) + c1);

	// Check signs of r3 and r4. If both point 3 and point 4 lie on
	// same side of line 1, the line segments do not intersect.
	if ((r3 != 0) && (r4 != 0) && same_sign(r3, r4)){
		return 0;//DONT intersect
	}

	// Compute a2, b2, c2
	a2 = lineB2.y - lineB1.y;
	b2 = lineB1.x - lineB2.x;
	c2 = (lineB2.x * lineB1.y) - (lineB1.x * lineB2.y);

	// Compute r1 and r2
	r1 = (a2 * lineA1.x) + (b2 * lineA1.y) + c2;
	r2 = (a2 * lineA2.x) + (b2 * lineA2.y) + c2;

	// Check signs of r1 and r2. If both point 1 and point 2 lie
	// on same side of second line segment, the line segments do
	// not intersect.
	if ((r1 != 0) && (r2 != 0) && (same_sign(r1, r2))){
		return 0;//DONT intersect
	}

	//Line segments intersect: compute intersection point.
	denom = (a1 * b2) - (a2 * b1);

	if (denom == 0) {
		return 1;//collinear
	}

	if (denom < 0){ 
		offset = -denom / 2; 
	} 
	else {
		offset = denom / 2 ;
	}

	// The denom/2 is to get rounding instead of truncating. It
	// is added or subtracted to the numerator, depending upon the
	// sign of the numerator.
	//num = (b1 * c2) - (b2 * c1);
	//if (num < 0){
	//	x = (num - offset) / denom;
	//} 
	//else {
	//	x = (num + offset) / denom;
	//}

	//num = (a2 * c1) - (a1 * c2);
	//if (num < 0){
	//	y = ( num - offset) / denom;
	//} 
	//else {
	//	y = (num + offset) / denom;
	//}

	// lines_intersect
	return 2;

}
//----------------------------------------------------------
bool drawingCanvas::same_sign(float a, float b){
	return (( a * b) >= 0);
}
//--------------------------------------------------------------
void drawingCanvas::mouseDragged(int x, int y, int button){
	if(closed == false){
		//if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
		//	if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
		//if((437 < x) && (x < 713)){
		//	if((728 < y) && (y < 1004)){
			
		ofVec2f mouse(x,y);
		if((x != 0 && y != 0) && !bSetFirstPoint){
			firstMouse = mouse; //to be able to close shape
			bSetFirstPoint = true;
		}		
		makeLine(mouse);
		if(!drawDummy){
			lastMouse = mouse;
		}
		//	}
		//}
	}
}
//--------------------------------------------------------------
void drawingCanvas::mousePressed(int x, int y, int button){
	bSetFirstPoint = false;
	//if(!bDrawing){
		//cout << "starting drawing" << endl;
		if(closed == false){
			//check if its inside the area to be able to draw
			//if((posCanvas.x-(width/2) < x) && (x < posCanvas.x+(width/2))){
			//	if((posCanvas.y-(height/2) < y) && (y < posCanvas.y+(height/2))){
			if((x > drawArea.x) && (x < drawArea.x + drawArea.width)){
				if((y > drawArea.y) && (y < drawArea.y + drawArea.height)){
					lastMouse = ofVec2f(x,y);
					if((x != 0 && y != 0) && !bSetFirstPoint){
						firstMouse = lastMouse; //to be able to close shape
						bSetFirstPoint = true;
					}
					//myPolyline.addVertex(lastMouse);//first vertex
					////////////////myPolyline2->addVertex(ofVec2f(lastMouse.x-posCanvas.x,lastMouse.y-posCanvas.y));
					myPolyline2.addVertex(getRealPoint(lastMouse));
					myPolyline.addVertex(getRealPoint(lastMouse));
					poly2exists = true;
				}
				else{
					poly2exists = false;
				}
			}
		}
		bDrawing = true;
	//}
}
//--------------------------------------------------------------
void drawingCanvas::mouseReleased(int x, int y, int button){
	bDrawing = false;
	if(closed == false){
		cout << "adding last point" << endl;
		cout << firstMouse.x << ", " << firstMouse.y << endl;
		//add last point
		//myPolyline.addVertex(firstMouse);//close last vertex, with first vertex
		//////////////myPolyline2->addVertex(ofVec2f(firstMouse.x-posCanvas.x,firstMouse.y-posCanvas.y));//polyline2 is the one that is on slicing position
		myPolyline2.addVertex(getRealPoint(firstMouse));
		myPolyline.addVertex(getRealPoint(firstMouse));	
		closed = true;
		poly2exists = true;
		cout << "finishing drawing" << endl;
	}
	if(drawDummy == true){
		drawDummy = false;
		myDummyLine.clear();
	}
}
//--------------------------------------------------------------
ofPolyline* drawingCanvas::getPolyline(){
	poly2exists = false;
	return &myPolyline2;
}
//--------------------------------------------------------------
bool drawingCanvas::drawingExists(){
	return poly2exists;
}
//--------------------------------------------------------------
void drawingCanvas::exit(){
	/*free(myPolyline);
	if(poly2exists){
		free(myPolyline2);
	}*/
	//myCanvasImage.clear();
}