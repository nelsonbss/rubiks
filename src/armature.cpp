//#include "armature.h"
//
//armature::armature(ofVec3f cntr, float w, float h, float d, float bs){
//	center = cntr;
//
//	width  = w;
//	height = h;
//	depth = d;
//	blockSide = bs;
//	//side = height/blockSide;
//	side = bs;
//
//	btogglePlanes = false;
//
//	rot.x=0;
//	rot.y=0;
//	rot.z=0;
//}
////--------------------------------------------------------------
//void armature::setup(){
//	plane1.set( width, height );
//	plane1.setPosition(0-(width/2)+side+1, 0, center.z);
//	plane1.rotate(90, 0, 1.0, 0.0);
//
//	plane2.set( width, height );
//	plane2.setPosition(0+(width/2)-side-1, 0, center.z);
//	plane2.rotate(90, 0, 1.0, 0.0);
//
//	plane3.set( width, height);
//	plane3.setPosition(0, 0+(height/2)-side-1, center.z);
//	plane3.rotate(90, 1.0, 0.0, 0.0);
//	plane3.rotate(90, 0.0, 1.0, 0.0);
//
//	plane4.set( width, height);
//	plane4.setPosition(0, 0-(height/2)+side+1, center.z);
//	plane4.rotate(90, 1.0, 0.0, 0.0);
//	plane4.rotate(90, 0.0, 1.0, 0.0);
//
//	plane5.set( width, height );
//	plane5.setPosition(0, 0, center.z+(width/2)-side-1);
//	plane5.rotate(0, 0, 1.0, 0.0);
//
//	plane6.set( width, height );
//	plane6.setPosition(0, 0, center.z-(width/2)+side+1);
//	plane6.rotate(0, 0, 1.0, 0.0);
//	///////// CYLINDERS //////////////////////////////////////////////////////////////////////
//	cylinder1.set(height/30,height);
//	cylinder1.setPosition(0, 0, 0);
//	cylinder1.setResolution(20, 1, 4);
//
//	cylinder2.set(height/30,height);
//	cylinder2.setPosition(0, 0, 0);
//	cylinder2.setResolution(20, 1, 4);
//	cylinder2.rotate(90, 1.0, 0.0, 0.0);
//
//	cylinder3.set(height/30,height);
//	cylinder3.setPosition(0, 0, 0);
//	cylinder3.setResolution(20, 1, 4);
//	cylinder3.rotate(90, 0.0, 0.0, 1.0);
//	//END///// CYLINDERS //////////////////////////////////////////////////////////////////////
//
//	/////////BOXES//////////////////////////////////////////////////////////////////////
//	//r//
//	box1.set( side,side,depth);//width, height, thick
//	box1.setPosition(0-(width/2), 0, 0);
//	box1.rotate(90, 0, 1.0, 0.0);
//
//	box2.set( side,side,depth);
//	box2.setPosition(0+width/2, 0,0);
//	box2.rotate(90, 0, 1.0, 0.0);
//	//g//
//	box3.set( side,side,depth);
//	box3.setPosition(0, 0-(height/2), 0);
//	box3.rotate(90, 1.0, 0.0, 0.0);
//	box3.rotate(90, 0.0, 1.0, 0.0);
//
//	box4.set( side,side,depth);
//	box4.setPosition(0, 0+height/2, 0);
//	box4.rotate(90, 1.0, 0.0, 0.0);
//	box4.rotate(90, 0.0, 1.0, 0.0);
//	//b//
//	box5.set( side,side,depth);
//	box5.setPosition(0, 0, width/2);
//	box5.rotate(0, 0, 1.0, 0.0);
//
//	box6.set( side,side,depth);
//	box6.setPosition(0, 0,-width/2);
//	box6.rotate(0, 0, 1.0, 0.0);
//	//END/////BOXES//////////////////////////////////////////////////////////////////////
//}
////--------------------------------------------------------------
//void armature::update(){
//}
//
//void armature::update(string _eventName, SubObEvent* _event){
//}
//
////--------------------------------------------------------------
//void armature::togglePlanes(){
//	if(btogglePlanes == true){
//		cout << "Time: " << ofGetElapsedTimef() << endl;
//		btogglePlanes =false;
//	}else{
//		btogglePlanes=true;
//	}
//}
////--------------------------------------------------------------
//void armature::draw(){
//	glPushMatrix();
//
//	glTranslatef(center.x,center.y,center.z);
//	glRotatef(rot.z,0,0,1);
//	glRotatef(rot.y,0,1,0);
//	glRotatef(rot.x,1,0,0);
//	
//	
//
//	////////////////////boxes//////////////////////////////////////////////////////////////////////
//	ofFill();
//	ofSetColor(0,0,0);
//
//	box1.draw();
//	box2.draw();
//	box3.draw();
//	box4.draw();
//	box5.draw();
//	box6.draw();
//
//	//ofSetColor(255);
//
//	//// Planes ////////////////////////////////////////////////////////////////
//	//if(btogglePlanes == true) {
//	//	ofFill();
//	//	ofSetColor(255,250,10,150);
//	//	plane1.draw();
//	//	ofSetColor(255,250,10,150);
//	//	plane2.draw();
//
//	//	ofSetColor (255,0,0,150);
//	//	plane3.draw();
//	//	ofSetColor(255,0,0,150);
//	//	plane4.draw();
//
//	//	ofSetColor(0,0,255,150);
//	//	plane5.draw();
//	//	ofSetColor(0,0,255,150);
//	//	plane6.draw();
//	//}
//
//	////////////////// Cylinders ////////////////////////////////////////////////////////////////////
//	//cylinder 1//----------------------------------------------------------------------------
//	//ofSetColor(255);
//	ofSetColor(0);
//	ofFill();
//	cylinder1.draw();
//	cylinder2.draw();
//	cylinder3.draw();
//	///////////////////////////////////////////////////////////////////////////////////////
//
//	glPopMatrix();
//}
////--------------------------------------------------------------
//void armature::moveA(ofVec3f input){
//	center.x +=  input.x;
//	center.y -= input.y;
//	center.z += input.z;
//}
////--------------------------------------------------------------
//void armature::rotateA(ofVec3f input){
//	rot.x += input.x;
//	rot.y += -input.y;
//	rot.z += input.z;
//}
#include "armature.h"

armature::armature(ofVec3f cntr, float w, float h, float d, float bs, int gSize){
	center = cntr;

	width  = w;
	height = h;
	depth = d;
	blockSide = bs;
	//side = height/blockSide;
	side = bs;

	//btogglePlanes = false;

	rot.x=0;
	rot.y=0;
	rot.z=0;

	gridSize=gSize;
	numCylinders=3*gridSize;


}
//--------------------------------------------------------------
void armature::setup(){
	double cylPos=300/gridSize;
	double cylPosNow=-150+cylPos/2;

	for (int i=0;i<numCylinders;i++) {
		cylinders.push_back(ofCylinderPrimitive());
	}

	for (int k=0;k<numCylinders;k+=3) {
		int index=k/3;
		cylPosNow=-150+(cylPos/2)+cylPos*index;

		double clySizeNow=cylPos-10;
		if ((gridSize%2)==0) {
			if (index==gridSize/2) {
				clySizeNow-=20;
				cylPosNow+=10;
			} else if (index+1==gridSize/2) {
				clySizeNow-=20;
				cylPosNow-=10;
			}
		}

		cylinders[k].set(20,clySizeNow);
		cylinders[k].setPosition(0,0,cylPosNow);
		cylinders[k].setResolution(9,1,2);
		cylinders[k].rotate(90,1.0,0.0,0.0);

		cylinders[k+1].set(20,clySizeNow);
		cylinders[k+1].setPosition(0,cylPosNow,0);
		cylinders[k+1].setResolution(9,1,2);
		cylinders[k+1].rotate(90,0.0,1.0,0.0);

		cylinders[k+2].set(20,clySizeNow);
		cylinders[k+2].setPosition(cylPosNow,0,0);
		cylinders[k+2].setResolution(9,1,2);
		cylinders[k+2].rotate(90,0.0,0.0,1.0);

	}




	cylinderA.set(5,300);
	cylinderA.setPosition(0, 0, 0);
	cylinderA.setResolution(9, 1, 2);
	cylinderB.set(5,300);
	cylinderB.setPosition(0, 0, 0);
	cylinderB.setResolution(9, 1, 2);
	cylinderC.set(5,300);
	cylinderC.setPosition(0, 0, 0);
	cylinderC.setResolution(9, 1, 2);

	cylinderA.rotate(90, 0.0, 0.0, 1.0);
	cylinderB.rotate(90, 0.0, 1.0, 0.0);
	cylinderC.rotate(90, 1.0, 0.0, 0.0);

}
//--------------------------------------------------------------
void armature::update(){
}

void armature::update(string _eventName, SubObEvent* _event){
}

//--------------------------------------------------------------
//void armature::togglePlanes(){
//	if(btogglePlanes == true){
//		cout << "Time: " << ofGetElapsedTimef() << endl;
//		btogglePlanes =false;
//	}else{
//		btogglePlanes=true;
//	}
//}
//--------------------------------------------------------------
void armature::draw(){
	glPushMatrix();

	glTranslatef(center.x,center.y,center.z);
	glRotatef(-rot.y,1,0,0);
	glRotatef(rot.x,0,1,0);
	glRotatef(rot.z,0,0,1);
	
	////////////////// Cylinders ////////////////////////////////////////////////////////////////////
	ofSetColor(0);
	ofFill();
	for (int i=0;i<numCylinders;i++) {
		cylinders[i].draw();
	}
	ofSetColor(255);
	ofFill();
	cylinderA.draw();
	cylinderB.draw();
	cylinderC.draw();
	///////////////////////////////////////////////////////////////////////////////////////
	glPopMatrix();
}
//--------------------------------------------------------------
void armature::moveA(ofVec3f input){
	center.x += input.x;
	center.y += input.y;
	center.z += input.z;
}
//--------------------------------------------------------------
void armature::rotateA(ofVec3f input){
	rot.x += input.x;
	rot.y += input.y;
	rot.z += input.z;
}
