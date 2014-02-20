#include "armature.h"

armature::armature(ofVec3f cntr, float w, float h, float d, float bs){
	center = cntr;
	width  = w;
	height = h;
	depth = d;
	blockSide = bs;
	side = height/blockSide;

	btogglePlanes = false;

	rot.x=0;
	rot.y=0;
	rot.z=0;
}
//--------------------------------------------------------------
void armature::setup(){
	plane1.set( width, height );
	plane1.setPosition(0-(width/2)+side+1, 0, center.z);
	plane1.rotate(90, 0, 1.0, 0.0);

	plane2.set( width, height );
	plane2.setPosition(0+(width/2)-side-1, 0, center.z);
	plane2.rotate(90, 0, 1.0, 0.0);

	plane3.set( width, height);
	plane3.setPosition(0, 0+(height/2)-side-1, center.z);
	plane3.rotate(90, 1.0, 0.0, 0.0);
	plane3.rotate(90, 0.0, 1.0, 0.0);

	plane4.set( width, height);
	plane4.setPosition(0, 0-(height/2)+side+1, center.z);
	plane4.rotate(90, 1.0, 0.0, 0.0);
	plane4.rotate(90, 0.0, 1.0, 0.0);

	plane5.set( width, height );
	plane5.setPosition(0, 0, center.z+(width/2)-side-1);
	plane5.rotate(0, 0, 1.0, 0.0);

	plane6.set( width, height );
	plane6.setPosition(0, 0, center.z-(width/2)+side+1);
	plane6.rotate(0, 0, 1.0, 0.0);
	///////// CYLINDERS //////////////////////////////////////////////////////////////////////
	cylinder1.set(height/30,height);
	cylinder1.setPosition(0, 0, center.z);
	cylinder1.setResolution(20, 1, 4);

	cylinder2.set(height/30,height);
	cylinder2.setPosition(0, 0, center.z);
	cylinder2.setResolution(20, 1, 4);
	cylinder2.rotate(90, 1.0, 0.0, 0.0);

	cylinder3.set(height/30,height);
	cylinder3.setPosition(0, 0, center.z);
	cylinder3.setResolution(20, 1, 4);
	cylinder3.rotate(90, 0.0, 0.0, 1.0);
	//END///// CYLINDERS //////////////////////////////////////////////////////////////////////

	/////////BOXES//////////////////////////////////////////////////////////////////////
	//r//
	box1.set( side,side,depth);//width, height, thick
	box1.setPosition(0-(width/2), 0, center.z);
	box1.rotate(90, 0, 1.0, 0.0);

	box2.set( side,side,depth);
	box2.setPosition(0+width/2, 0, center.z);
	box2.rotate(90, 0, 1.0, 0.0);
	//g//
	box3.set( side,side,depth);
	box3.setPosition(0, 0-(height/2), center.z);
	box3.rotate(90, 1.0, 0.0, 0.0);
	box3.rotate(90, 0.0, 1.0, 0.0);

	box4.set( side,side,depth);
	box4.setPosition(0, 0+height/2, center.z);
	box4.rotate(90, 1.0, 0.0, 0.0);
	box4.rotate(90, 0.0, 1.0, 0.0);
	//b//
	box5.set( side,side,depth);
	box5.setPosition(0, 0, center.z+width/2);
	box5.rotate(0, 0, 1.0, 0.0);

	box6.set( side,side,depth);
	box6.setPosition(0, 0, center.z-width/2);
	box6.rotate(0, 0, 1.0, 0.0);
	//END/////BOXES//////////////////////////////////////////////////////////////////////
}

//--------------------------------------------------------------
void armature::update(){
	/*rot.x +=0.1;
	rot.y +=0;
	rot.z +=0;*/
}

void armature::togglePlanes(){
	if(btogglePlanes == true){
		cout << "Time: " << ofGetElapsedTimef() << endl;
		btogglePlanes =false;
	}else{
		btogglePlanes=true;
	}
}

//--------------------------------------------------------------
void armature::draw(){
	glPushMatrix();
	glTranslatef(center.x,center.y,center.z);
	glRotatef(rot.x,1,0,0);
	glRotatef(rot.y,0,1,0);
	glRotatef(rot.z,0,0,1);

	////////////////////boxes//////////////////////////////////////////////////////////////////////
	ofFill();
	//ofSetColor(255,0,0);
	ofSetColor(0,0,0);
	box1.draw();

	ofFill();
	//ofSetColor(255,0,0);
	ofSetColor(0,0,0);
	box2.draw();

	ofFill();
	//ofSetColor(0,255,0);
	ofSetColor(0,0,0);
	box3.draw();

	ofFill();
	//ofSetColor(0,255,0);
	ofSetColor(0,0,0);
	box4.draw();

	ofFill();
	//ofSetColor(0,0,255);
	ofSetColor(0,0,0);
	box5.draw();

	ofFill();
	//ofSetColor(0,0,255);
	ofSetColor(0,0,0);
	box6.draw();

	ofFill();
	ofSetColor(255);

	//if(mode == 3) {
	//	float angle = ofGetElapsedTimef()*3.2;
	//	float strength = (sin( angle+.25 )) * .5f * 5.f;
	//	ofVec3f faceNormal;
	//	for(int i = 0; i < triangles.size(); i++ ) {
	//		// store the face normal here.
	//		// we change the vertices, which makes the face normal change
	//		// every time that we call getFaceNormal //
	//		faceNormal = triangles[i].getFaceNormal();
	//		for(int j = 0; j < 3; j++ ) {
	//			triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength);
	//		}
	//	}
	//}


	// Planes ////////////////////////////////////////////////////////////////
	if(btogglePlanes == true) {
		ofFill();
		ofSetColor(255,250,10,150);
		plane1.draw();
		ofSetColor(255,250,10,150);
		plane2.draw();

		ofSetColor (255,0,0,150);
		plane3.draw();
		ofSetColor(255,0,0,150);
		plane4.draw();

		ofSetColor(0,0,255,150);
		plane5.draw();
		ofSetColor(0,0,255,150);
		plane6.draw();
	}

	////////////////// Cylinders ////////////////////////////////////////////////////////////////////
	//cylinder 1//----------------------------------------------------------------------------
	//ofSetColor(255);
	ofSetColor(0);
	ofFill();
	cylinder1.draw();

	//cylinder 2//----------------------------------------------------------------------------
	ofFill();
	cylinder2.draw();

	ofFill();
	cylinder3.draw();
	///////////////////////////////////////////////////////////////////////////////////////

	
	glPopMatrix();
}
