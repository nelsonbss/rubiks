#include "menuPuzzle.h"
#include "puzzle.h"
#include "sgCore.h"
#include "ofRender.h"

///////////////////////////////////////////////////////
menuPuzzle::menuPuzzle(SG_VECTOR p, SG_VECTOR t) : GuiNode(){
	position.x = p.x;
	position.y = p.y;
	position.z = p.z;

	tempPos.x = t.x;
	tempPos.y = t.y;
	tempPos.z = t.z;

	tempSize.x = 200;
	tempSize.y = 200;

	addParam("drag", "true");
	addParam("n", "1");
	addParam("droppable", "true");

	//pos.set((float)tempPos.x, (float)tempPos.y);
	//size.set(200,200);
	//scale = 1.0;

	//setPosition(ofVec2f((float)tempPos.x, (float)tempPos.y));
	//setSize(ofVec2f(200,200));
	//setScale(1.0);

	objectId=0;
	object = NULL;

	bHidden = false;
	bActive = true;
}

void menuPuzzle::nodeInit(){
	//drawPos.set((float)tempPos.x, (float)tempPos.y);
	//drawPos.set(200,200);
	//drawSize.set(200,200);

	//ofVec2f pctPos((float)tempPos.x / (float)ofGetWidth(), (float)tempPos.y / (float)ofGetHeight());

	//setPosition(pctPos);
	//setSize(ofVec2f(200.0 / (float)ofGetWidth(),200) / (float)ofGetHeight());
	//setScale(1.0);

	//setPosition();

	//drawPos.set((float)tempPos.x, (float)tempPos.y);
	drawPos.set(0,0);
	drawSize.set(200,200);

	activate();
}

//--------------------------------------------------------------
void menuPuzzle::setup(){
	//the real object is never rendered or moved::::it is used to make the boolean intersection
	//the rendered and animated object is temp

	temp = (sgC3DObject *) object->Clone();

	SG_VECTOR transP = {position.x,position.y,position.z};
	object->InitTempMatrix()->Translate(transP);//this translates the object to be cut!!
	object->ApplyTempMatrix();  
	object->DestroyTempMatrix();

	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
	//ofr->sgCoretoOFmesh(temp,myMesh,-1); //-1 because its not a cubie but want color on the sample object
	ofr->sgCoretoOFmesh(temp,myMesh,-2,objectId); //-2 for plain color
	myVbo.setMesh(myMesh, GL_STATIC_DRAW);
	free(ofr);
}
//--------------------------------------------------------------
void menuPuzzle::update(){
	SG_VECTOR transP = {tempPos.x,tempPos.y,tempPos.z};
	//drawPos.set((float)tempPos.x, (float)tempPos.y);
	//SG_VECTOR transP = {drawPos.x, drawPos.y, 0};

	SG_POINT rot = {0,0,0};
	SG_VECTOR rotM = {1,0,0};
	/*temp->InitTempMatrix()->Rotate(rot,rotM,ofDegToRad(45));
	SG_VECTOR rotM2 = {1,0,0};
	temp->GetTempMatrix()->Rotate(rot,rotM2,ofDegToRad(45));*/

	temp->InitTempMatrix()->Translate(transP);
	if(objectId == 2){
		//cube
		SG_VECTOR offset = {-0,-50,-0}; //for the cube to be in place
		temp->GetTempMatrix()->Translate(offset);//this translates the object to be cut!!

		//apply armature axis rotations (x-y-z) to the real object
		SG_POINT rotP = {0,0,0};
		SG_VECTOR rotV = {1,0,0};
		object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
		SG_VECTOR rotV2 = {0,1,0};
		object->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
		SG_VECTOR rotV3 = {0,0,1};
		object->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
		object->ApplyTempMatrix(); 
		object->DestroyTempMatrix(); 
	}else if(objectId == 3){
		////cone..pyramid
		//SG_POINT rotP = transP;
		//SG_VECTOR rotV = {1,0,0};
		//temp->GetTempMatrix()->Rotate(rotP,rotV,ofDegToRad(90));
		//SG_VECTOR offset = {0,100,0}; 
		//temp->GetTempMatrix()->Translate(offset);
	}else if(objectId == 4){
		//rabbit
		SG_POINT rotP2 = transP;//{tempPos.x,tempPos.y,tempPos.z};
		SG_VECTOR rotV2 = {1,0,0};
		temp->GetTempMatrix()->Rotate(rotP2,rotV2,ofDegToRad(180));
	}else if(objectId == 200){
		//extruded object
		SG_VECTOR offset = {0,70,0}; 
		temp->GetTempMatrix()->Translate(offset);
	}
	temp->ApplyTempMatrix();  
}

bool menuPuzzle::isInside(int _x, int _y){
    cout << getName() << " checking insides " << tempPos.x << ", " << tempPos.x + tempSize.x << " - " << tempPos.y << ", " << tempPos.y + tempSize.y;
	cout << " against " << _x << ", " << _y << endl;
    //cout << getName() << " checking insides." << endl;
	if((_x > tempPos.x && _x < (tempPos.x + tempSize.x) &&
       (_y > tempPos.y && _y < (tempPos.y + tempSize.y)))){
		   if(getParam("send-select") == "true"){
			   input("select", 0, 0, 0, ofVec2f(_x, _y), ofVec2f(0,0));
		   }
		   return true;
       }
    return false;
}

//------------------------------------------------------------------------
void menuPuzzle::nodeDraw(){  

	ofNoFill();
	ofSetColor(0,0,255);
	ofRect(tempPos.x, tempPos.y, tempSize.x, tempSize.y);
	ofSetColor(255,255,255);
	
	glPushMatrix();
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	temp->DestroyTempMatrix();
	glScalef(0.39,0.39,0.39);
	//myMenuPuzzle->draw();
	myVbo.draw(GL_TRIANGLES, 0,myMesh.getNumIndices());
	glPopMatrix();
}
//----------------------------------------------------------------
void menuPuzzle::loadObject(sgC3DObject *obj, int ID){

	if(object==NULL){
		object = obj;
	}else{
		sgDeleteObject(object);
		object = obj;
	}
	objectId = ID;

	//if(objectId == 1){
	//	//torus
	//	SG_VECTOR offset = {0,0,0}; //for torus to be in place, the 
	//	object->InitTempMatrix()->Translate(offset);//this translates the object to be cut!!
	//	//SG_POINT rotP = {0,0,0};
	//	//SG_VECTOR rotV = {1,0,0};
	//	//object->GetTempMatrix()->Rotate(rotP,rotV,ofDegToRad(45));
	//	object->ApplyTempMatrix();  
	//	object->DestroyTempMatrix();
	//}else if(objectId == 2){
	//	//cube
	//	SG_VECTOR offset = {-150,-150,-150}; //for the cube to be in center  place, it has sides of 300
	//	object->InitTempMatrix()->Translate(offset);//this translates the object to be cut!!
	//	object->ApplyTempMatrix();  
	//	object->DestroyTempMatrix();
	//}else if(objectId == 3){
	//	//cone..pyramid
	//	/*SG_POINT rotP = {0,0,0};
	//	SG_VECTOR rotV = {1,0,0};
	//	object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(90));
	//	SG_VECTOR offset = {0,100,0}; 
	//	object->GetTempMatrix()->Translate(offset);
	//	object->ApplyTempMatrix();  
	//	object->DestroyTempMatrix();*/
	//}else if(objectId == 4){
	//	//rabbit
	//	SG_POINT rotP = {0,0,0};
	//	SG_VECTOR rotV = {1,0,0};
	//	object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(180));
	//	/*SG_VECTOR offset = {0,800,200}; 
	//	object->GetTempMatrix()->Translate(offset);*/
	//	object->ApplyTempMatrix();  
	//	object->DestroyTempMatrix();
	//}else if(objectId == 200){
	//	//extruded object
	//	SG_VECTOR offset = {0,0,0}; 
	//	object->InitTempMatrix()->Translate(offset);
	//	object->ApplyTempMatrix();  
	//	object->DestroyTempMatrix();
	//}
}
//--------------------------------------------------------------
void menuPuzzle::loadPuzzle(puzzle *inpuzzle){
	myMenuPuzzle = inpuzzle;
}
//----------------------------------------------------------------
sgC3DObject* menuPuzzle::getObject(){
	return temp;
}
//-----------------------------------------------------------------
void menuPuzzle::colorFacesMenu(){
	ofRender *ofr = new ofRender();
	if(objectId == 1 || objectId == 4){
		//torus or bunny
		ofr->colorTorus(myMesh);
	}else if( objectId == 200){
		ofr->colorFacesExtrudedMenu(myMesh,armRot);
	}
	else{
		ofr->colorFacesMenu(myMesh,armRot, 0.01, objectId);
	}
	free(ofr);

	//have to replace the vbo
	ofVbo tempVbo;
	tempVbo.setMesh(myMesh, GL_STATIC_DRAW);
	myVbo=tempVbo;
}

void menuPuzzle::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	//cout << "Type = " << _type << " dX, dY = " << _deltaPos.x << ", " << _deltaPos.y << endl;
	if(_type == "drag"){
		cout << getName() << " dragging" << endl;
		tempPos.x += _deltaPos.x;
		tempPos.y += _deltaPos.y;
		drawPos.set(tempPos.x, tempPos.y);
	}
	if(_type == "tap"){
		cout << name << " - executing" << endl;
		execute();
	}
	/*
	if(!bWatchTime){
		bWatchTime = true;
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
	*/
}

//----------------------------------------------------------------
void menuPuzzle::applyArmRotations(ofVec3f v){
	armRot = (v)*-1;
	//apply armature axis rotations (x-y-z) to the real object
	SG_POINT rotP = {0,0,0};
	SG_VECTOR rotV = {1,0,0};
	temp->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
	SG_VECTOR rotV2 = {0,1,0};
	temp->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
	SG_VECTOR rotV3 = {0,0,1};
	temp->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
	temp->ApplyTempMatrix();
	temp->DestroyTempMatrix(); 
}
//----------------------------------------------------------------
void menuPuzzle::exit(){
	sgCObject::DeleteObject(temp);
}
//-------------------------------------------------------
puzzle* menuPuzzle::getPuzzle(){
	return myMenuPuzzle;
}
void menuPuzzle::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void menuPuzzle::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void menuPuzzle::mouseReleased(int x, int y, int button){
}