#include "menuPuzzle.h"
#include "puzzle.h"
#include "sgCore.h"
#include "ofRender.h"

///////////////////////////////////////////////////////
menuPuzzle::menuPuzzle(SG_VECTOR p, SG_VECTOR t, int ID ) : GuiNode(){

	id = ID;
	objectId=0;
	////////object = NULL;
	////////temp = NULL;

	////////myMesh = CubieMesh();
	////////myVbo = ofVbo();

	////////armRot = ofVec3f (0,0,0);
	tempPos.x = t.x;
	tempPos.y = t.y;
	tempPos.z = t.z;

	tempSize.x = 180;
	tempSize.y = 180;

	addParam("drag", "true");
	addParam("n", "1");
	addParam("droppable", "true");
	addParam("tap","true");
	addParam("active","true");
	addParam("hidden","false");
	addParam("draw-position", "front");

	//pos.set((float)tempPos.x, (float)tempPos.y);
	//size.set(200,200);
	//scale = 1.0;

	//setPosition(ofVec2f((float)tempPos.x, (float)tempPos.y));
	//setSize(ofVec2f(200,200));
	//setScale(1.0);

	drawPos.x = tempPos.x;
	drawPos.y = tempPos.y;

	startPos = drawPos;

	viewport.x = drawPos.x;   
	viewport.y = drawPos.y;
	viewport.width = tempSize.x;
	viewport.height = tempSize.y;

	position.x = tempSize.x / 2;
	position.y = tempSize.y / 2;
	position.z = 0;

	bHidden = false;
	bActive = true;

	bReadyForInput = true;
	bWatchTime = false;

	//timer = new ofxTimer();

	setName("menu-puzzle-" + ofToString(id));

	timeOfLastInteraction = 0;
}
//-------------------------------------------------------------------------------------------------------
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
	//drawPos.set(0,0);
	//drawSize.set(200,200);
	
	

	activate();
	SubObMediator::Instance()->addObserver("menupuzzle-intercepted",this);
}
//---------------------------------------------------------------------------------------------------
void menuPuzzle::nodeExecute(){
	//bReadyForInput = false;
	//timer->addTimer(1000, (int*)&bReadyForInput, 1);
	SubObEvent ev;
	ev.setName("menupuzzle-selected");
	ev.addArg("puzzle-id", id);
	ev.addArg("game-tag", targetGame);
	SubObMediator::Instance()->sendEvent(ev.getName(), ev);
	viewport.x = startPos.x;
	viewport.y = startPos.y;
	bWatchTime = true;
}
//-------------------------------------------------------------------------------------------
void menuPuzzle::setup(){
	//GLfloat light_ambient[] = { 0.0, 0.2, 0.0, 1.0 };
	//GLfloat light_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	//GLfloat light_specular[] = { 0.0, 1.0, 0.0, 1.0 };
	//GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glEnable(GL_LIGHT0);
	//GLfloat light_ambient1[] = { 0.0, 0.0, 0.2, 1.0 };
	//GLfloat light_diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
	//GLfloat light_specular1[] = { 0.0, 0.0, 1.0, 1.0 };
	//GLfloat light_position1[] = { -1.0, 1.0, 1.0, 0.0 };
	//glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	//glEnable(GL_LIGHT1);

	////light.setPointLight();
	//light.setSpotlight(60.0);
	////light.setDirectional();
	//light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	//light.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	//the real object is never rendered or moved::::it is used to make the boolean intersection
	//the rendered and animated object is temp

	//temp = (sgC3DObject *) object->Clone();

	//SG_VECTOR transP = {position.x,position.y,position.z};
	//object->InitTempMatrix()->Translate(transP);//this atranslates the object to be cut!!
	//object->ApplyTempMatrix();  
	//object->DestroyTempMatrix();

	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
	//ofr->sgCoretoOFmesh(temp,myMesh,-1); //-1 because its not a cubie but want color on the sample object
	ofr->sgCoretoOFmesh(temp,myMesh,-2,objectId); //-2 for plain color
	myVbo.setMesh(myMesh, GL_STATIC_DRAW);
	free(ofr);
}
//------------------------------------------------------------------------------------------
void menuPuzzle::update(){

	SG_POINT rotP = {position.x,position.y,position.z};
	SG_VECTOR rotV = {1,0,0};
	SG_VECTOR offset = {0,0,0}; //for the cube to be in place
	temp->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(-45));
	SG_VECTOR rotV2 = {0,1,0};
	temp->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(35));
	temp->ApplyTempMatrix(); 

	//temp->InitTempMatrix()->Translate(transP);
	if(objectId == 2){
		////cube
		//SG_VECTOR offset = {-50,-50,-0}; //for the cube to be in place
		//temp->GetTempMatrix()->Translate(offset);//this translates the object to be cut!!

		//apply armature axis rotations (x-y-z) to the real object
		//SG_POINT rotP = {0,0,0};
		//SG_VECTOR rotV = {1,0,0};
		//object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
		//SG_VECTOR rotV2 = {0,1,0};
		//object->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
		//SG_VECTOR rotV3 = {0,0,1};
		//object->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
		//object->ApplyTempMatrix(); 
		//object->DestroyTempMatrix(); 
	}else if(objectId == 3){
		////cone..pyramid
		//SG_POINT rotP = transP;
		//SG_VECTOR rotV = {1,0,0};
		//temp->GetTempMatrix()->Rotate(rotP,rotV,ofDegToRad(90));
		//SG_VECTOR offset = {0,100,0}; 
		//temp->GetTempMatrix()->Translate(offset);
	}else if(objectId == 4){
		////rabbit
		//SG_POINT rotP2 = transP;//{tempPos.x,tempPos.y,tempPos.z};
		//SG_VECTOR rotV2 = {1,0,0};
		//temp->GetTempMatrix()->Rotate(rotP2,rotV2,ofDegToRad(180));
	}else if(objectId == 200){
		//extruded object
		SG_VECTOR offset = {0,70,0}; 
		temp->GetTempMatrix()->Translate(offset);
	}
	//temp->ApplyTempMatrix();  
}
//--------------------------------------------------------------------------------
void menuPuzzle::update(string _eventName, SubObEvent _event){
	if(_eventName == "menupuzzle-intercepted"){
		string intercepter = _event.getArg("intercepter")->getString();
		targetGame = ofSplitString(intercepter,":")[0];
		cout << "got drop from " << targetGame << endl;
		if(_event.getArg("object-name")->getString() == getName()){
			//setPosition();
			execute();
		}
	}
}
//-------------------------------------------------------------------------------
bool menuPuzzle::isInside(int _x, int _y){
	//cout << getName() << " checking insides " << viewport.x << ", " << viewport.x + viewport.width << " - " << viewport.y << ", " << viewport.y + viewport.height;
	//cout << " against " << _x << ", " << _y << endl;
	//cout << getName() << " checking insides." << endl;
	if((_x > viewport.x && _x < (viewport.x + viewport.width) &&
		(_y > viewport.y && _y < (viewport.y + viewport.height)))){
			if(getParam("send-select") == "true"){
				input("select", 0, 0, 0, ofVec2f(_x, _y), ofVec2f(0,0));
			}
			lastMouse.set(_x, _y);
			return true;
	}
	return false;
}
//------------------------------------------------------------------------
void menuPuzzle::nodeDraw(){  
	ofNoFill();
	ofSetColor(0,0,255);
	//ofRect(viewport.x, viewport.y, viewport.width, viewport.height);
	//ofSetColor(255,255,255);

	ofPushView();
	ofEnableDepthTest();
	ofViewport(viewport);
	ofSetupScreen();
	//ofEnableDepthTest();
	//glPushMatrix();
	ofPushMatrix();
	//////////glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	//temp->DestroyTempMatrix();
	ofTranslate(viewport.width / 2, viewport.height / 2, position.z);
	glScalef(0.3,0.3,0.3);


	//ofEnableLighting();
	//light.enable();
	//light.setPosition(viewport.width / 2, viewport.height / 2, 400);
	//light.lookAt(ofVec3f(tempPos.x, tempPos.y, tempPos.z));
	//light.disable();
	//ofDisableLighting();

	//myMenuPuzzle->draw();
	myVbo.draw(GL_TRIANGLES, 0,myMesh.getNumIndices());
	ofPopMatrix();
	//ofDisableDepthTest();
	ofPopView();
	if(bWatchTime){
		if(ofGetElapsedTimeMillis() - timeOfLastInteraction > 1000){
			cout << "watch time triggered." << endl;
			//setPosition();
			viewport.x = startPos.x;
			viewport.y = startPos.y;
			bWatchTime = 0;
		}
		if(ofGetElapsedTimeMillis() - timeOfLastInteraction > 500){
			if(bSend){
				SubObEvent ev;
				ev.setName("menupuzzle-dropped");
				ev.addArg("object-name", getName());
				ev.addArg("position", ofVec2f(viewport.x + (viewport.width / 2), viewport.y + (viewport.height / 2)));
				SubObMediator::Instance()->sendEvent("menupuzzle-dropped", ev);
				bSend = false;
			}
		}
	}
}
//----------------------------------------------------------------
void menuPuzzle::loadObjectMP(sgC3DObject *obj, int ID){

	if(object==NULL){
		//object = obj; //to make a puzzle at the beggining 
		temp = obj;
	}else{
		//sgDeleteObject(object);
		//object = obj;
		sgDeleteObject(temp);
		temp = obj;
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
	//cube
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
////----------------------------------------------------------------
//sgC3DObject* menuPuzzle::getObject(){
//	return temp;
//}
//-----------------------------------------------------------------
void menuPuzzle::colorFacesMenu(){
	ofRender *ofr = new ofRender();
	if(objectId >= 8 ){//(objectId == 1 || objectId == 4){
		//torus or bunny
		ofr->colorTorusMenu(myMesh, colorsVMenu);
	}else if( objectId == 200){
		ofr->colorFacesExtrudedMenu(myMesh,armRot);
	}
	else{
		ofr->colorFacesMenu(myMesh,armRot, 0.01, objectId,colorsVMenu,uniqueNormals);
	}
	free(ofr);
	//have to replace the vbo
	ofVbo tempVbo;
	tempVbo.setMesh(myMesh, GL_STATIC_DRAW);
	myVbo=tempVbo;
}
//---------------------------------------------------------------------------------------
void menuPuzzle::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	cout << "Type = " << _type << " dX, dY = " << _deltaPos.x << ", " << _deltaPos.y << endl;
	if(_type == "drag"){
		/*
		cout << getName() << " dragging" << endl;
		tempPos.x += _deltaPos.x;
		tempPos.y += _deltaPos.y;
		drawPos.set(tempPos.x, tempPos.y);
		*/
		ofVec2f currentMouse = _absPos;
		ofVec2f delta = currentMouse - lastMouse;
		//drawPos += _deltaPos;
		//dragPos = drawPos;
		viewport.x += delta.x;
		viewport.y += delta.y;
		lastMouse = currentMouse;
	}
	if(_type == "tap"){
		cout << name << " - executing" << endl;
		execute();
	}
	if(!bWatchTime){
		bWatchTime = true;
	}
	if(!bSend){
		bSend = true;
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
}



















//----------------------------------------------------------------
//void menuPuzzle::applyArmRotations(ofVec3f v){
//	armRot = (v)*-1;
//	//apply armature axis rotations (x-y-z) to the real object
//	SG_POINT rotP = {0,0,0};
//	SG_VECTOR rotV = {1,0,0};
//	temp->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
//	SG_VECTOR rotV2 = {0,1,0};
//	temp->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
//	SG_VECTOR rotV3 = {0,0,1};
//	temp->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
//	temp->ApplyTempMatrix();
//	temp->DestroyTempMatrix(); 
//}
//----------------------------------------------------------------
void menuPuzzle::exit(){
	sgCObject::DeleteObject(temp);
}
//-------------------------------------------------------
puzzle* menuPuzzle::getPuzzle(){
	return myMenuPuzzle;
}