#include "menuPuzzle.h"
#include "puzzle.h"
#include "sgCore.h"
#include "ofRender.h"

///////////////////////////////////////////////////////
menuPuzzle::menuPuzzle( ) : GuiNode(){
}
menuPuzzle::menuPuzzle(SG_VECTOR p, SG_VECTOR t, int ID ) : GuiNode(){

	id = ID;
	objectId=0;
	//object = NULL;
	temp = NULL;

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

	offsetSlicer = ofVec3f (0,0,0);
	rotateSlicer = ofVec3f (0,0,0);

	timeOfLastInteraction = 0;

	myMenuPuzzle = NULL;
	MenuPuzzleBR = NULL;
	MenuPuzzleBL = NULL;
	MenuPuzzleTR = NULL;
	MenuPuzzleTL = NULL;

	menuPuzzleRotation = 0;
	ct1 = 0;
	ct2 = 0;
	animpos.x = startPos.x;
	animpos.y = startPos.y;
	draggingMe = false;
	ct1 = ofGetElapsedTimeMillis();
	activeAnimation = false;
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
	draggingMe = false;
	startPos.x = animpos.x;
	startPos.y = animpos.y;
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

	//light.setPointLight();
	light.setSpotlight(60.0);
	//light.setDirectional();
	light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	light.setSpecularColor( ofColor(255.f, 255.f, 255.f));

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
	ofr->sgCoretoOFmesh(temp,myMesh,-2,objectId,"no"); //-2 for plain color
	myVbo.setMesh(myMesh, GL_STATIC_DRAW);
	free(ofr);


	saveanim = false;
}
//-------------------------------------------------------------------------------------------
void menuPuzzle::setup(SG_POINT targetposIn){


	light.setSpotlight(60.0);
	light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	light.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
	ofr->sgCoretoOFmesh(temp,myMesh,-2,objectId,"no"); //-2 for plain color
	myVbo.setMesh(myMesh, GL_STATIC_DRAW);
	free(ofr);



	targetpos.x = targetposIn.x;
	targetpos.y = targetposIn.y;

	saveanim = true;
}
//------------------------------------------------------------------------------------------
void menuPuzzle::update(){

	SG_POINT rotP = {position.x,position.y,position.z};
	SG_VECTOR rotV = {1,0,0};
	SG_VECTOR offset = {0,0,0}; //for the cube to be in place
	temp->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(-40));
	SG_VECTOR rotV2 = {0,1,0};
	temp->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(menuPuzzleRotation));
	menuPuzzleRotation = menuPuzzleRotation - 0.3;
	temp->ApplyTempMatrix(); 

	if(activeAnimation == true){
		///////////////move puzzles
		ct2 = ofGetElapsedTimeMillis();
		double diff = ct2 - ct1;
		double velPixels = 10;
		double move = (diff * velPixels)/500;
		ct1 = ct2;


		if(saveanim==false){
			animpos.x = animpos.x - move;
		}else{
			targetpos.x = targetpos.x - move;
		}

		if(draggingMe == false){
			viewport.x =  animpos.x;
			viewport.y =  animpos.y;
		}

		if(viewport.x < -(viewport.width)){
			viewport.x = ofGetWindowWidth();
			animpos.x = viewport.x;
		}

		if(saveanim==true){
			SG_VECTOR distance = sgSpaceMath::VectorsSub(targetpos,animpos);
			if (distance.x > 0.9 || distance.y > 0.9){
				if( targetpos.x != animpos.x || targetpos.y != animpos.y){
					animpos.x = animpos.x + distance.x/10;
					animpos.y = animpos.y + distance.y/10;
				}
				float playRoom = 1.1;
				if(((targetpos.x - playRoom) <= animpos.x) && 
					(animpos.x <= (targetpos.x + playRoom)) &&
					((targetpos.y - playRoom) <= animpos.y) && 
					(animpos.y <= (targetpos.y + playRoom))
					){
						animpos.x = targetpos.x;
						animpos.y = targetpos.y;
						saveanim = false;
				}
			}else{
				animpos.x = targetpos.x;
				animpos.y = targetpos.y;
				saveanim = false;
			}

		}
	}


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
			draggingMe = true;
			if(getParam("send-select") == "true"){
				input("select", 0, 0, 0, ofVec2f(_x, _y), ofVec2f(0,0));
			}
			lastMouse.set(_x, _y);
			return true;
	}
	draggingMe = false;
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

	//glPushMatrix();
	ofPushMatrix();
	//ofEnableDepthTest();
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	//temp->DestroyTempMatrix();
	ofTranslate(viewport.width / 2, viewport.height / 2, position.z);
	glScalef(0.3,0.3,0.3);


	//if(objectId>=8){
	//	ofEnableLighting();
	//	light.enable();
	//	light.setPosition(viewport.width / 2, viewport.height / 2, 400);
	//	light.lookAt(ofVec3f(tempPos.x, tempPos.y, tempPos.z));
	//	light.disable();
	//	ofDisableLighting();
	//}

	myMenuPuzzle->drawMenuPuzzle();


	//myVbo.draw(GL_TRIANGLES, 0,myMesh.getNumIndices());
	//ofDisableDepthTest();
	ofPopMatrix();
	ofDisableDepthTest();
	ofPopView();
	if(bWatchTime){
		if(ofGetElapsedTimeMillis() - timeOfLastInteraction > 1000){
			cout << "watch time triggered." << endl;
			//setPosition();
			draggingMe = false;
			startPos.x = animpos.x;
			startPos.y = animpos.y;
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
void menuPuzzle::loadObjectMP(sgC3DObject *obj, int ID,vector< ofVec3f > ObjectUniqueNormalsIN){

	if(temp==NULL){
		//object = obj; //to make a puzzle at the beggining 
		temp = obj;
	}else{
		//sgDeleteObject(object);
		//object = obj;
		sgDeleteObject(temp);
		temp = obj;
	}
	objectId = ID;
	ObjectUniqueNormals = ObjectUniqueNormalsIN;
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
void menuPuzzle::loadPuzzle(puzzle *inpuzzle, int station){
	if(station==0){
		myMenuPuzzle = inpuzzle;
	}else if(station==1){
		MenuPuzzleBR = inpuzzle;
	}else if(station==2){
		MenuPuzzleBL = inpuzzle;
	}else if(station==3){
		MenuPuzzleTR = inpuzzle;
	}else if(station==4){
		MenuPuzzleTL = inpuzzle;
	}
}
//------------------------------------------------------------------
puzzle *menuPuzzle::cloneMyPuzzle(puzzle* toClone, int selObjId){
	//takes toClone as a template to create new *puzzle

	//create a new puzzle with the data from the already loaded puzzle
	puzzle *tempPuzzle = new puzzle(toClone->pos,toClone->cubiesOffset,toClone->gridSize,toClone->ObjectUniqueNormals);
	//create new cuibies to transfer info to them
	for(int i=0;i<tempPuzzle->numPieces;i++){
		cubie *auxCubie = new cubie(tempPuzzle->pos.x,tempPuzzle->pos.y,tempPuzzle->pos.z,i,selObjId,tempPuzzle->cubiesOffset,tempPuzzle->gridSize);
		//auxCubie->setup();
		//add this cubie to mycubies[]
		tempPuzzle->myCubies[i] = auxCubie;
	}
	//copy mesh data from  toClone->myCubies;
	for(int i=0;i<tempPuzzle->numPieces;i++){
		int numObjs2 = toClone->myCubies[i]->numObjs;
		tempPuzzle->myCubies[i]->numObjs = numObjs2;
		tempPuzzle->myCubies[i]->armRotations = toClone->myCubies[i]->armRotations;
		tempPuzzle->myCubies[i]->pointRotate = toClone->myCubies[i]->pointRotate;
		tempPuzzle->myCubies[i]->objectList = (sgC3DObject**)malloc(numObjs2*sizeof(sgC3DObject*));
		for (int j=0; j < numObjs2; j++){
			//get sgcore object information
			tempPuzzle->myCubies[i]->objectList[j] = (sgC3DObject*) toClone->myCubies[i]->objectList[j]->Clone();
		}
		tempPuzzle->myCubies[i]->setup();
	}
	//create the meshes from the sgCore objects
	//so the objects can be renderes by openFrameworks
	for(int i=0;i<tempPuzzle->numPieces;i++){
		//tempPuzzle->myCubies[i]->crateOfMeshs();
		tempPuzzle->myCubies[i]->myMeshs = toClone->myCubies[i]->myMeshs;
	}

	//for(int i=0;i<tempPuzzle->numPieces;i++){
	//	int numObjs2 = toClone->myCubies[i]->numObjs;
	//	for (int j=0; j < numObjs2; j++){
	//		tempPuzzle->myCubies[i]->myMeshs = toClone->myCubies[i]->myMeshs.;
	//		tempPuzzle->myCubies[i]->myVbos = toClone->myCubies[i]->myVbos;
	//	}
	//}

	//generate puzzle colors
	//if(selObjId < 8 ){
	//	tempPuzzle->colorFaces(selObjId);
	//}else{
	//	tempPuzzle->colorTorus();
	//}

	//copy the color information for each myMeshs[] from toClone->myCubies[i]
	for(int i=0;i<tempPuzzle->numPieces;i++){
		int numObjs2 = toClone->myCubies[i]->numObjs;
		for (int j=0; j < numObjs2; j++){
			vector< ofFloatColor > tcolors = toClone->myCubies[i]->myMeshs[j].getColors();
			tempPuzzle->myCubies[i]->myMeshs[j].clearColors();
			tempPuzzle->myCubies[i]->myMeshs[j].addColors(tcolors);
		}
	}

	return tempPuzzle;
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
	//free the puzzles
	myMenuPuzzle->exit();
	MenuPuzzleBR->exit();
	MenuPuzzleBL->exit();
	MenuPuzzleTR->exit();
	MenuPuzzleTL->exit();

	free(myMenuPuzzle);
	free(MenuPuzzleBR);
	free(MenuPuzzleBL);
	free(MenuPuzzleTR);
	free(MenuPuzzleTL);
}
//-------------------------------------------------------
puzzle* menuPuzzle::getPuzzle(string gameTag){
	if(gameTag=="bl"){
		if(MenuPuzzleBL != NULL){
			return MenuPuzzleBL;
		}else{
			//it is a saved puzzle and needs to clone the "bl" for the first time
			return MenuPuzzleBL = cloneMyPuzzle(myMenuPuzzle,objectId);
		}
	}else if(gameTag=="br"){
		if(MenuPuzzleBR != NULL){
			return MenuPuzzleBR;
		}else{
			//it is a saved puzzle and needs to clone the "br" for the first time
			return MenuPuzzleBR = cloneMyPuzzle(myMenuPuzzle,objectId);
		}
	}else if(gameTag=="tl"){
		if(MenuPuzzleTL != NULL){
			return MenuPuzzleTL;
		}else{
			//it is a saved puzzle and needs to clone the "tl" for the first time
			return MenuPuzzleTL = cloneMyPuzzle(myMenuPuzzle,objectId);
		}
	}else{
		//tr
		if(MenuPuzzleTR != NULL){
			return MenuPuzzleTR;
		}else{
			//it is a saved puzzle and needs to clone the "tr" for the first time
			return MenuPuzzleTR = cloneMyPuzzle(myMenuPuzzle,objectId);
		}
	}
	//return myMenuPuzzle; // this is never returned, its the one being showed in the middle
}