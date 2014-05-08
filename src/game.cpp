#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"
#include "drawingCanvas.h"
#include "SubObEvent.h"
#include "SubObMediator.h"
#include "GuiConfigurator.h"
#include "menuPuzzle.h"

#include <vector>

#include <thread>

#define planeThicknes 0.001
#define tamCutter 1000

#define canvasSide 500

#define USE_LIGHT 1  //0 is off

game::game(SG_VECTOR gamePos, float w, float h, SG_VECTOR displayPos, ofRectangle _vp, float iddleTime, string myPrefix){


	posGame = gamePos;
	slicingPos = posGame;
	width = w;
	height = h;

	station = myPrefix;

	iddleTimer = iddleTime;

	posP.x = displayPos.x; //for the puzzle & sample object
	posP.y = displayPos.y;
	posP.z = displayPos.z;

	tamSideArmature = 300;
	posA.x = displayPos.x; //for the armature
	posA.y = displayPos.y;
	posA.z = displayPos.z;

	//drawing canvas
	posCanvas.x = 0;//displayPos.x;
	posCanvas.y = 0;//displayPos.y;
	posCanvas.z = 0;//displayPos.z;
	canvasB = false;

	//rotP.x = 0; //rotation of puzzle
	//rotP.y = 0;
	//rotP.z = 0;

	//offset
	tamCubie = 0;  //when creating armature this gets a size
	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;

	rotateSlicer.x = 0;
	rotateSlicer.y = 0;
	rotateSlicer.z = 0;

	armID = -1; //initialized in -1 when there is no armature selected
	objectID = -1; //initialized on -1 because on stage=0 there is no object selected


	creatingPuzzle = false;
	puzzleFinished = false;
	numPuzzlePieces = 0;
	cubieToCut = 0;

	extrudedB = false;
	bExtrude = false;

	myCanvasImage.loadImage("drawingGrid.png");
	//sc = ofFloatColor (1, 1, 0); //yellow

	savePuzzleB = false;
	hasSaved = false;

	angleR = 0; //is the angle sent to the rotateByIDandAxisNew
	bUnproject = false;

	bDragInput = false;

	viewport = _vp;

	posP.x = viewport.getWidth() / 2;
	posP.y = viewport.getHeight() / 2;
	posP.z = -300;

	posA = posP;

	bUseViewport = true;

	camPosition.set(viewport.width / 2, viewport.height / 2, 400);

	timeOfLastInteraction = ofGetElapsedTimeMillis();
	bInAttract = false;

	currentLanguage = "english";

	myCanvas.setViewport(viewport);

	faceRotate = false;
	faceRotateB = false;//used in the 2 id rotation function

	/*
	thread t1(task1, "Hello");
	t1.join();*/
}
//----------------------------------------------------------------------------------------
void game::loadObjDir(string _path){
	ofDirectory dir(_path);
	dir.allowExt("obj");
	dir.listDir();
	vector<ofFile> files = dir.getFiles();
	for(auto fIter = files.begin(); fIter != files.end(); fIter++){
		string name = fIter->getFileName();
		int id = ofToInt(ofSplitString(name, "_")[0]);
		loader.loadModel(_path + name);
		ofMesh tempMesh = loader.getMesh(0);
		vector<ofVec3f> verts = tempMesh.getVertices();
		//make an array[] from this vector
		SG_POINT *vert = new SG_POINT[verts.size()];
		for(int i=0;i<verts.size(); i++){
			vert[i].x = verts[i].x;
			vert[i].y = verts[i].y;
			vert[i].z = verts[i].z;
		}
		//get indices from mesh
		vector<ofIndexType> indices = tempMesh.getIndices();
		//make an array[] from this vector
		SG_INDEX_TRIANGLE *indexes = new SG_INDEX_TRIANGLE[indices.size()];
		for(int i=0;i<indices.size(); i++){
			indexes->ver_indexes[i] = indices[i];
		}
		//generate sgC3DObject from geometry information
		sgCObject* tObj = sgFileManager::ObjectFromTriangles(vert,verts.size(),indexes,indices.size()/3); 
		objects[id] = tObj;
		delete [] vert;
		delete [] indexes;
	}
}
//--------------------------------------------------------------
//void game::setup(sgCObject *sgBunnyi,sgCObject *sgTetrahedroni,sgCObject *sgDodecahedroni,sgCObject *sgIcosahedroni,sgCObject *sgOctahedroni, string _prefix){//,sgCObject *sgTeapoti){
//	//gets the .obj files loaded and converted into sgC3DObject
//
//	prefix = _prefix;
//
//	sgBunny = sgBunnyi;
//	sgTetrahedron = sgTetrahedroni;
//	sgDodecahedron = sgDodecahedroni;
//	sgIcosahedron = sgIcosahedroni;
//	sgOctahedron = sgOctahedroni;
//	//sgTeapot = sgTeapoti;
//
//	setup();
//}
//---------------------------------------------------------------
void game::setup(string _prefix){
	prefix = _prefix;
	setup();
}
//-------------------------------------------------------------
void game::setup(){


	SubObMediator::Instance()->addObserver(prefix + ":ibox:2", this);
	SubObMediator::Instance()->addObserver(prefix + ":ibox-tap", this);
	SubObMediator::Instance()->addObserver(prefix + ":new-color", this);
	SubObMediator::Instance()->addObserver(prefix + ":ibox:1", this);
	SubObMediator::Instance()->addObserver(prefix + ":ibox:0", this);
	SubObMediator::Instance()->addObserver(prefix + ":object-selected", this);
	SubObMediator::Instance()->addObserver(prefix + ":armature-selected", this);
	SubObMediator::Instance()->addObserver(prefix + ":next-step", this);
	SubObMediator::Instance()->addObserver(prefix + ":reset", this);
	SubObMediator::Instance()->addObserver(prefix + ":extrude", this);
	SubObMediator::Instance()->addObserver(prefix + ":extrusion-success", this);
	SubObMediator::Instance()->addObserver(prefix + ":interaction", this);
	SubObMediator::Instance()->addObserver(prefix + ":save", this); //to make the save button registered to do something
	SubObMediator::Instance()->addObserver(prefix + ":solve", this);

	goToAttract();

	SubObMediator::Instance()->addObserver(prefix + ":make-one2", this);
	SubObMediator::Instance()->addObserver(prefix + ":menupuzzle-selected", this);

	//light.setPointLight();
	light.setSpotlight(60.0);
	//light.setDirectional();
	light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	light.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	dragId = -1;
	bHaveNewObject = false;
	objectID = -1;
	step = -1;
	idcubie=0;
	faceRotate = false;
	//faceRotateB = false;//used in the 2 id rotation function
}
//----------------------------------------------------------------------
void game::update(){

	//if(step != -1){
	//	/////take time sample to see if game has to go to standBy mode
	//	goToAttractStepS = ofGetElapsedTimef();
	//	if(goToAttractStepS - goToAttractStepI >= iddleTimer){
	//		restart();
	//		step = -1;
	//	}
	//}

	if(bHaveNewObject){
		SG_VECTOR objectPos = {0,0,0};
		if(step == 1 || step == 7){
			clearDisplayedObject();
		}
		if(newObject == 50){
			prepareDrawing();
		} else {
			loadObjectG(newObject, objectPos, posP);
		}
		bHaveNewObject = false;
	}
	if(step == 1 || step == 2 || step == 3){
		//if there is an object selected
		objectDisplayed->update();
	}

	if(bExtrude){
		cout << "calling extrude." << endl;
		if(myCanvas.drawingExists()){
			//make extruded object
			if(extrudeObject(myCanvas.getPolyline())){
				objectDisplayed->update();
				SubObEvent ev;
				ev.setName(prefix + ":extrusion-success");
				SubObMediator::Instance()->sendEvent(prefix + ":extrusion-success", ev);
				//delete e;
			}else{
				prepareDrawing();
			}
		} else {
			cout << "no drawing." << endl;
		}
		bExtrude = false;
	}

	//if(step == 3){
	//	//myArmature->update();//now its rotating onits own
	//}

	///////////////////////////////////////////////////////////////////////
	/////////////////to create a puzzle on every frame
	if(creatingPuzzle){
		//loaded armature //applied armature rotations //created slicer and cutter
		//now do slicing
		//get thenumber of pieces from the slicer
		if(cubieToCut < numPuzzlePieces){
			createPuzzleOneByOne(cubieToCut);//create Puzzle goes to step 4 to show the puzzle
			cubieToCut ++; //go to next cubie
			//update the current number of cubies on the puzzle, since we are creating one by one
			myPuzzle->currentNumCubies = cubieToCut; 
			puzzleFinished = true;
			step = 4;
		}else{
			setPage("color-start");
			creatingPuzzle = false;
		}
	}

	/////////////////////////////////////////////////////
	///////////////////////////////////////update cubies
	if(puzzleFinished){
		if(step == 4 || step == 5 || step == 7){
			if(step == 7){
				myPuzzle->updateMenuPuzzle();
			}else{
				myPuzzle->update();
			}
			////////////////////////////////////////////////////move all puzzle
			//myPuzzle->move(posP);
			////////////////////////////////////////////rotate all puzzle
			//myPuzzle->rotate(rotP);
			//////////////////////////////////////////make face rotation
			if(faceRotate == true) {
				if(myPuzzle->isMoving() == false){
					myPuzzle->rotateByIDandAxisNew(idcubie,axis,dir,angleR);
					//myPuzzle->rotateByIDandAxis(idcubie,axis,dir,angleR);
					//put this move on the game history vector
					//undo will look for the other 9 cubies involved and do a pop x2 on their history
					historyV.push_back(history(idcubie,axis,!dir)); //save inverse move (!), to do it at undo, and do 2 pop 
					faceRotate = false;
					angleR = 0;
				}
			}
			//updatePuzzle = false;

			///do face rotations with two cubie ids
			//if(myPuzzle->faceRotateB == true) {
			//	//////int ans = myPuzzle->rotateTwoIds(idcubieA,idcubieB,dir);

			//	////int ans;
			//	////if(myPuzzle->bHaveActiveCubie && myPuzzle->bHaveRotationCubie){
			//	////	ans = myPuzzle->rotateTwoIds(myPuzzle->activeCubie,myPuzzle->rotationCubie,dir);
			//	////	myPuzzle->bHaveActiveCubie = false;
			//	////	myPuzzle->bHaveRotationCubie = false;
			//	////} else {
			//	////	ans = myPuzzle->rotateTwoIds(myPuzzle->activeCubie,myPuzzle->activeCubie,dir);
			//	////	myPuzzle->bHaveActiveCubie = false;
			//	////}
			//	//////ans has encripted the axis and the direction 10-x 20-y 30-z 1 or 0 direction
			//	//////put this move on the game history vector
			//	//////undo will look for the other 9 cubies involved and do a pop x2 on their history
			//	////if(ans/10 == 1){
			//	////	//1
			//	////	axis.x = 1;
			//	////	axis.y = 0;
			//	////	axis.z = 0;
			//	////}else if(ans/10 == 2){
			//	////	//2
			//	////	axis.x = 0;
			//	////	axis.y = 1;
			//	////	axis.z = 0;
			//	////}else{
			//	////	//3
			//	////	axis.x = 0;
			//	////	axis.y = 0;
			//	////	axis.z = 1;
			//	////}
			//	////bool d = ans%2;//this is 0 or 1
			//	////historyV.push_back(history(idcubieA,axis,!d)); //save inverse move (!), to do it at undo, and do 2 pop 
			//	myPuzzle->faceRotateB = false;
			//}
		}
	}

	if(bHaveNext){
		guiInput('n');
		bHaveNext = false;
	}
	if(bHaveReset){
		if(!bDragInput){
			//to prevent resetting while doing a rotation
			restart(); //same as //guiInput('r');
			bHaveReset = false;
		}
	}
	//if(bHaveMakeOne){
	//	guiInput('r');
	//	bHaveReset = false;
	//}
	//camPosition.rotate(1, ofVec3f(viewport.width / 2,viewport.height / 2, posP.z));
	if(!bInAttract){
		int currentTime = ofGetElapsedTimeMillis();
		if(currentTime - timeOfLastInteraction > DELAY){
			goToAttract();
		}
	}
}
//----------------------------------------------------------------------
void game::update(string _eventName, SubObEvent _event){
	if(_eventName == prefix + ":solve"){
		if(savePuzzleB == false){
			if(myPuzzle->isActive() == false){
				unDoMenuPuzzle();
			}
		}
	}
	if(_eventName == prefix + ":save"){
		if(savePuzzleB == false){
			if(myPuzzle->isActive() == false){
				//call save functionality here
				//unDoMenuPuzzle();
				savePuzzleB = true;
				//dont make opengl calls here... no drawing anything
				setPage("object-start");
				camPosition.set(viewport.width / 2, viewport.height / 2, 400);
			}
		}
	}
	if(_eventName == prefix + ":object-selected"){
		if(step == 0){
			//SubObEvent *ev = new SubObEvent();
			setPage("object-selected");
		}
		if(step == 0  || step == 1 || step == 6 || step == 7){
			int obj = _event.getArg("object")->getInt();
			//SG_VECTOR objectPos = {0,0,0};  //where it gets sliced
			guiLoad(obj);
		}
		if(_event.getArg("object")->getInt() == 50){
			setPage("draw");
		} else {
			setPage("object-selected");
		}
	}
	if(_eventName == prefix + ":menupuzzle-selected"){
		//setPage("draw");
		SubObEvent ev;
		ev.setName(prefix + ":interaction");
		SubObMediator::Instance()->sendEvent(ev.getName(), ev);
	}
	if(_eventName == prefix + ":next-step"){
		guiNext();
		cout << "set game to - " << step << endl;
		if(step == 0){
			//cout << "No more attract" << endl;
			setPage("object-start");
			bInAttract = false;
		} else if(step == 1){
			setPage("armature-start");

		} else if(step == 3){
			//setPage("color-start");//this is now getting called after a puzzle is finishd being created, on update()
		} else if(step == 4){
			if(creatingPuzzle == false){
				setPage("play");
			}
		} else {
			setPage("object-start");
			camPosition.set(viewport.width / 2, viewport.height / 2, 400);
			guiReset();
		}
	}
	if(_eventName == prefix + ":armature-selected"){
		string armStr = ofToString(_event.getArg("object")->getInt());
		//myGames[0]->setCurrentStep(3);
		guiInput(armStr.c_str()[0]);
		setPage("armature-selected");
	}
	if(_eventName == prefix + ":cut-object"){
		SG_VECTOR v = {0,0,0};
		createPuzzle(v);
	}
	if (_eventName == prefix + ":extrude"){
		if(step == 6){
			guiExtrude();
			camPosition.set(viewport.width / 2, viewport.height / 2, 400);
		} else {
			setPage("object-start");
			camPosition.set(viewport.width / 2, viewport.height / 2, 400);
			guiReset();
		}
	}
	if(_eventName == prefix + ":extrusion-success"){
		setPage("object-selected");
	}
	if((_eventName == prefix + ":reset") ){
		if(step >= 4){
			if(myPuzzle->isActive() == false){
				setPage("object-start");
				camPosition.set(viewport.width / 2, viewport.height / 2, 400);
				guiReset();
			}
		}else{
			setPage("object-start");
			camPosition.set(viewport.width / 2, viewport.height / 2, 400);
			guiReset();
		}

		//SceneManager::Instance()->reset();
		//cout << "RESET" << endl;
	}
	if(_eventName == prefix + ":ibox-tap"){
		//ofVec3f pos = _event->getArg("absPos")->getVec2();
		//mousePressed(pos.x, pos.y, 2);
		ofVec2f p = _event.getArg("absPos")->getVec2();
		//cout << "phase = " << phase << " p = " << p.x << ", " << p.y << endl;
		if(!bUnproject){
			bUnproject = true;
			bDragInput = false;
			mousePoint.set(p.x, p.y, 0);
		}
	}
	if(_eventName == prefix + ":ibox:0"){
		if(step == 6){
			ofVec2f pos = _event.getArg("absPos")->getVec2();
			myCanvas.mouseReleased(pos.x, pos.y,0);
		}
		if(bDragInput){
			//myPuzzle->endRotation();
			//it looks at current face rotation angle and decides to do a cmplete move, or go back to previous possition
			myPuzzle->decideMove(axis);
			faceRotate = false;
			bDragInput = false;
		}
	}
	////////////////////////////
	/////Receive input from touch.
	////////////////////////////
	if(_eventName == prefix + ":ibox:1"){
		//cout << "game bl:1" << endl;
		if(step == 3){
			ofVec3f m = _event.getArg("deltaPos")->getVec2();
			cout << m << endl;
			moveA(m);
		}
		if(step == 6){
			int p = _event.getArg("phase")->getInt();
			ofVec2f pos = _event.getArg("absPos")->getVec2();
			if(p == 0){
				myCanvas.mousePressed(pos.x, pos.y, 0);
			}
			if(p == 1){
				int n = _event.getArg("n")->getInt();
				myCanvas.mouseDragged(pos.x, pos.y, 0);
			}
		}
		if(step == 5 || step == 7){
			int phase = _event.getArg("phase")->getInt();
			int gId = _event.getArg("ID")->getInt();
			cout << "Game - phase = " << phase << endl;
			if(phase == 0){
				ofVec2f p = _event.getArg("absPos")->getVec2();
				//cout << "phase = " << phase << " p = " << p.x << ", " << p.y << endl;
				if(!bUnproject && (dragId == -1)){
					bUnproject = true;
					bDragInput = false;
					mousePoint.set(p.x, p.y, 0);
					unprojectMode = UP_MODE_MOUSE;
					dragId = gId;
				}
			} else if(phase == 1){
				ofVec2f p = _event.getArg("absPos")->getVec2();
				int n = _event.getArg("n")->getInt();
				cout << "n = " << n << " p = " << p.x << ", " << p.y << endl;
				if(n == 0){
					myPuzzle->endRotation();
					dragId = -1;
					return;
				}
				if(!bUnproject && (gId == dragId)){
					bUnproject = true;
					//bDragInput = true;
					mousePoint.set(p.x, p.y, 0);
					unprojectMode = UP_MODE_MOUSE;
				}
			} else if(phase > 1){
				cout << "Ending rotation." << endl;
				myPuzzle->endRotation();
				dragId = -1;
			}
		}

	}
	/*
	if(_eventName == "ibox-bl:2"){
	ofVec3f r = _event->getArg("deltaPos")->getVec2();
	rotateA(r);
	}
	*/
	if(_eventName == prefix + ":ibox:2"){
		//cout << "game drag - " << _event.getArg("phase")->getInt() << endl;
		if(step == 3){
			ofVec3f r = _event.getArg("deltaPos")->getVec2();
			rotateA(r);
		} else {
			if(_event.getArg("phase")->getInt() == 0){
				ofVec2f pos = _event.getArg("absPos")->getVec2();
				mousePressed(pos.x, pos.y, 2);
			} else {
				ofVec2f pos = _event.getArg("absPos")->getVec2();
				mouseDragged(pos.x, pos.y, 2);
			}
		}
	}
	if(_eventName == prefix + ":new-color"){
		//ofFloatColor menuColor = ofFloatColor (1, 0, 1); //this color comes from the GUI
		ofVec3f newColor = _event.getArg("color")->getVec3();
		ofVec2f pos = _event.getArg("pos")->getVec2();
		newFaceColor = ofFloatColor(newColor.x / 255.0, newColor.y / 255.0, newColor.z / 255.0);
		if(!bUnproject){
			mousePoint.set(pos.x+35,pos.y+19, 0);//(ofGetMouseX(), ofGetMouseY(),0);
			bUnproject = true;
			unprojectMode = UP_MODE_COLOR;
		}
		//else{
		//	if(unprojectMode != UP_MODE_P){
		//		unprojectPoint(mousePoint);//mousePoint gets set on bUnproject
		//	} else {
		//		projectPoint(mousePoint);
		//	}
		//	bUnproject = false;
		//}
		//changeColorToColor(sc,menuColor);
		//sc = menuColor;
		//changeFaceColor(pos, menuColor);
	}
	if(_eventName == prefix + ":interaction"){
		//cout << "got interaction" << endl;
		timeOfLastInteraction = ofGetElapsedTimeMillis();
	}
}
//------------------------------------------------------------------------
void game::goToAttract(){
	/*SubObEvent ev;
	guiReset();
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":color-window");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":3d-window-box");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":3d-window");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	//ev.addArg("target", prefix + ":object-drop");
	//SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":arm-window");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":next-active");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":start-help");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":puzzle-help");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":bg-language");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target", prefix + ":bg-info");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target", prefix + ":next-inactive");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target", prefix + ":make-one");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target", prefix + ":reset");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target",prefix + ":arm-help");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target",prefix + ":arm-rotate");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	ev.addArg("target",prefix + ":draw-help");
	SubObMediator::Instance()->sendEvent("hide-node", ev);

	ev.setName("unhide-node");
	ev.addArg("target", prefix + ":attract");
	SubObMediator::Instance()->sendEvent("unhide-node", ev);
	*/

	setPage("attract");

	bInAttract = true;
	camPosition.set(viewport.width / 2, viewport.height / 2, 400);
	guiReset();
}
void game::setPage(string _page){
	GuiConfigurator::Instance()->setCurrentPage(prefix, _page);
}
//-------------------------------------------------------------------------
void game::setLanguage(string _lang){
	currentLanguage = _lang;
	SubObEvent ev;
	ev.setName(prefix + ":language-changed");
	ev.addArg("lang", currentLanguage);
	SubObMediator::Instance()->sendEvent(ev.getName(), ev);
}
//-----------------------------------------------------------------------
void game::updateGui(){

}
//----------------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw game steps////////////////////////////////////
	//drawViewportOutline(viewport);
	ofDisableAlphaBlending();
	if(bUseViewport){
		/*ofPushView();
		ofViewport(viewport);
		ofSetupScreen();*/
		ofNoFill();
		ofSetColor(0.0,0.0,0.0);
		//ofRect(viewport.x, viewport.y, viewport.width, viewport.height);
		cam.begin(viewport);
		if(USE_LIGHT){
			ofEnableLighting();
			light.enable();
			light.setPosition(camPosition);
			light.lookAt(ofVec3f(posP.x, posP.y, posP.z));
		}
		//cam.disableMouseInput();
		//ofSetupScreen();
		if(step == 6){
			ofSetupScreen();
		} else {
			cam.setupPerspective();
		}
		ofEnableAlphaBlending();
		ofSetColor(1.0,1.0,1.0,0.5);
		cam.setPosition(camPosition);
		//cam.setDistance(1000);
		cam.lookAt(ofVec3f(posP.x, posP.y, posP.z));
	}
	if(step == -1){
		//waiting for initializing touch
	}
	if(step == 0){
		//is waiting for a shape to be selected
		//object menuu is showing
		//it can load an object from menu
		//or from puzzle menu in the middle
	}
	if (step == 1){
		//show selected object
		objectDisplayed->draw();
		//myCutter->draw();
	}
	if(step == 2){
		//waiting for armature to be selected
		//show selected object
		objectDisplayed->draw();
	}
	if(step == 3){
		//armature has been selected
		//show selected object
		myArmature->draw();
		objectDisplayed->draw();
	}
	if(step == 4 ){
		//made the cuts
		//show color palette
		//show puzzle

		curRot.getRotate(angle, axistb);

		glPushMatrix();
		glTranslatef(posP.x,posP.y,posP.z);
		//new trackball
		glRotatef(angle, axistb.x, axistb.y, axistb.z);
		//glTranslatef(-posP.x,-posP.y,-posP.z);
		myPuzzle->draw();
		if(bUnproject){
			unprojectPoint(mousePoint);
			bUnproject = false;
		}
		glPopMatrix();
	}
	if(step == 5){
		//show puzzle
		//curRot.getRotate(angle, axistb);

		glPushMatrix();
		//glTranslatef(posP.x,posP.y,posP.z);
		ofTranslate(posP.x, posP.y, posP.z);
		//new trackball
		//glRotatef(angle, axistb.x, axistb.y, axistb.z);
		//ofFill();
		//ofBox(100);
		//glTranslatef(-posP.x,-posP.y,-posP.z);
		myPuzzle->draw();
		if(bUnproject){
			if(unprojectMode != UP_MODE_P){
				unprojectPoint(mousePoint);//mousePoint gets set on bUnproject
			} else {
				projectPoint(mousePoint);
			}
			bUnproject = false;
		}
		glPopMatrix();
		//drawPoints();
	}
	if(step == 6){
		//show drawing area
		//ofPushMatrix();
		//ofTranslate(posP.x, posP.y, posP.z);
		myCanvas.draw();
		//ofPopMatrix();
		ofSetColor(255,255,255);
		/*myCanvasImage.draw(0,0,0,canvasSide,canvasSide);*///posCanvas.x-canvasSide/2,posCanvas.y-canvasSide/2,posCanvas.z,canvasSide,canvasSide);
	}
	if(step == 7){
		//show puzzle
		glPushMatrix();
		//glTranslatef(posP.x,posP.y,posP.z);
		ofTranslate(posP.x, posP.y, posP.z);
		//new trackball
		//glRotatef(angle, axistb.x, axistb.y, axistb.z);
		//ofFill();
		//ofBox(100);
		//glTranslatef(-posP.x,-posP.y,-posP.z);
		myPuzzle->drawMenuPuzzle();
		if(bUnproject){
			if(unprojectMode != UP_MODE_P){
				unprojectPoint(mousePoint);//mousePoint gets set on bUnproject
			} else {
				projectPoint(mousePoint);
			}
			bUnproject = false;
		}
		glPopMatrix();
		//drawPoints();
	}
	if(bUseViewport){
		//ofPopView();
		if(USE_LIGHT){
			light.disable();
			ofDisableLighting();
		}
		cam.end();
	}
	//if (step>=5) {
	//	drawPoints();
	//}
}
//--------------------------------------------------------------------------------------------------------------------------------------
void game::unprojectPoint(ofVec3f _pnt){
	unprojectedPoint = picker.unproject(mousePoint, &viewport);
	//unprojectedPoint = cam.screenToWorld(_pnt, viewport);
	//cout << "UP = " << unprojectedPoint.x << ", " << unprojectedPoint.y << ", " << unprojectedPoint.z << endl;
	if(unprojectMode == UP_MODE_MOUSE){
		if(!bDragInput){
			//cout << "down" << endl;
			myPuzzle->checkCubiesForHit(unprojectedPoint);
			lastUnprojectedPoint = mousePoint;//unprojectedPoint;
			bHaveAxis = false;
			startMove(mousePoint);
			if(myPuzzle->activeCubie != -1){
				bDragInput = true;
			}
		} else {
			//myPuzzle->dragInput((unprojectedPoint - lastUnprojectedPoint) * 25.0);
			if(myPuzzle->isMoving() == false){
				//cout << "dragg" << endl;
				makeMove((mousePoint - lastUnprojectedPoint) * 25.0);//((unprojectedPoint - lastUnprojectedPoint) * 25.0);
				//lastUnprojectedPoint = unprojectedPoint;
			}
		}
	} else if(unprojectMode == UP_MODE_COLOR){
		myPuzzle->changeFaceColor(unprojectedPoint, newFaceColor);
	}
}
//---------------------------------------------------------------------------------------------
void game::projectPoint(ofVec3f _pnt){
	projectedPoint = picker.project(_pnt, &viewport);

}
//----------------------------------------------------------------------------------------------
void game::drawViewportOutline(const ofRectangle & _vp){
	ofPushStyle();
	ofNoFill();
	ofSetColor(50);
	ofSetLineWidth(0);
	ofRect(_vp);
	ofNoFill();
	ofSetColor(25);
	ofSetLineWidth(1.0f);
	ofRect(_vp);
	ofPopStyle();
}
//----------------------------------------------------------------------
void game::rotateByIDandAxis(int id, SG_VECTOR axs, bool d, float anglei){
	if(axs.x==0 && axs.y==0 && axs.z==0){
		//stop any rotation
		faceRotate = false;
	}else{
		//allow rotation
		idcubie = id;
		dir = d;
		axis = axs;
		angleR += anglei;
		//updatePuzzle = true;
		puzzleFinished = true;
		faceRotate = true;
	}
}
//----------------------------------------------------------------------
void game::rotateTwoIds(int cubieA, int cubieB,bool inside){
	faceRotateB = true; //this is usedn on update, to do rotationanimations
	idcubieA = cubieA;
	idcubieB = cubieB;
	dir = true;
}
//----------------------------------------------------------------------
void game::loadPuzzle(puzzle *inputPuzzle,int objID, SG_VECTOR p, SG_VECTOR t){

	/////////////////////////////////////////////////////////
	///////////do game reset..because loading a puzzle can happen at anytime
	if(step == 7){
		unDoMenuPuzzle();
	}
	if(step == 6){
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
		step = 0;
		objectID = -1;
	}else if(step==4 || step==5){
		//dont delete the puzzle if it has been saved
		//if(!hasSaved){
		myPuzzle->exit();
		//}
		hasSaved = false;
		myCutter->exit();
		mySlicer->exit();
		//objectDisplayed->exit();
		objectID = -1;
		step = 0;
		armID = -1;
		puzzleFinished = false;
		creatingPuzzle = false;

	}else if (step==3){
		//objectDisplayed->exit();             //clean displayed object after puzzle is created, so we dont keep it until the exit or restart
		step = 0;
		objectID = -1;
		armID = -1;
	}else if (step==1 || step==2){
		//objectDisplayed->exit();
		step = 0;
		objectID = -1;
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
	}

	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;

	rotateSlicer.x = 0;
	rotateSlicer.y = 0;
	rotateSlicer.z = 0;

	curRot.set (ofVec4f(0,0,0,0));
	historyV.clear();

	if(extrudedB){
		sgDeleteObject(extrudedObject);
		extrudedB = false;
	}
	//////////////////////////////////////////////////////////
	//load a puzzle from the puzzle menu on the center
	//if (objectID == -1){
	//	objectDisplayed = new myobject3D(p,t,station);
	//}else{
	//	objectDisplayed->exit();
	//	delete objectDisplayed;
	//	objectDisplayed = new myobject3D(p,t,station);
	//}
	//objectID = objID;
	//bool loaded = false;
	//if(step == 0 || step==1 || step == 7){
	//	if(objID != 200){
	//		objectDisplayed->loadObjectOD((sgC3DObject*)objects[objectID]->Clone(), objectID);
	//		loaded = true;
	//	}
	//	if(objID == 200){
	//		//load extruded object
	//		objectDisplayed->loadObjectOD((sgC3DObject *)extrudedObject->Clone(),200);
	//		loaded = true;
	//	}

	//	if(loaded == true){
	////////////////////////////////
	//objectDisplayed->setup();

	myPuzzle = inputPuzzle;

	armID = myPuzzle->gridSize;

	myPuzzle->pos.x = posP.x;
	myPuzzle->pos.y = posP.y;
	myPuzzle->pos.z = posP.z;
	//updatePuzzle = true;
	step = 7;
	SubObEvent ev;
	ev.setName("hide-node");
	ev.addArg("target", prefix + ":start-help");
	SubObMediator::Instance()->sendEvent("hide-node", ev);
	setPage("play2");
	//	}
	//}
} 
//----------------------------------------------------------------------
void game::loadMenuObject(int objID, SG_VECTOR p, SG_VECTOR t,vector< ofFloatColor > colorsVMenu, vector< ofVec3f > uniqueNormals){
	/////////////////////////////////////////////////////////
	///////////do game reset..because loading a puzzle can happen at anytime
	if(step == 6){
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
		step = 0;
		objectID = -1;
	}else if(step==4 || step==5){
		myPuzzle->exit();
		myCutter->exit();
		mySlicer->exit();
		//objectDisplayed->exit();
		objectID = -1;
		step = 0;
		armID = -1;
	}else if (step==3){
		//objectDisplayed->exit();             //clean displayed object after puzzle is created, so we dont keep it until the exit or restart
		step = 0;
		objectID = -1;
		armID = -1;
	}else if (step==1 || step==2){
		//objectDisplayed->exit();
		step = 0;
		objectID = -1;
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
	}

	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;

	rotateSlicer.x = 0;
	rotateSlicer.y = 0;
	rotateSlicer.z = 0;

	curRot.set (ofVec4f(0,0,0,0));
	historyV.clear();

	if(extrudedB){
		sgDeleteObject(extrudedObject);
		extrudedB = false;
	}
	//////////////////////////////////////////////////////////
	//load a puzzle Object from the puzzle menu on the center
	if (objectID == -1){
		objectDisplayed = new myobject3D(p,t,station);
	}else{
		objectDisplayed->exit();
		delete objectDisplayed;
		objectDisplayed = new myobject3D(p,t,station);
	}
	objectID = objID;
	colorsVMenuG = colorsVMenu;
	uniqueNormalsG = uniqueNormals;
	bool loaded = false;
	if(step == 0 || step==1 || step == 7){
		if(objID != 200){
			objectDisplayed->loadObjectOD((sgC3DObject*)objects[objectID]->Clone(), objectID);
			loaded = true;
		}
		if(objID == 200){
			//load extruded object
			objectDisplayed->loadObjectOD((sgC3DObject *)extrudedObject->Clone(),200);
			loaded = true;
		}

		if(loaded == true){
			////////////////////////////////
			objectDisplayed->setup();
			//step = 1;

			//make the puzzle automatically
			int grid =3; ///have to adapt this to any armature on the menu puzzles!!!///have to adapt this to any armature on the menu puzzles!!!
			loadArmatureMenu(grid); 
			applyArmRotations();
			createCutterSlicer();
			////createPuzzle(posP);
			//vector< ofVec3f > ObjectUniqueNormals;

			//ObjectUniqueNormals.push_back(ofVec3f(1,0,0));
			//ObjectUniqueNormals.push_back(ofVec3f(0,1,0));
			//ObjectUniqueNormals.push_back(ofVec3f(0,0,1));
			//ObjectUniqueNormals.push_back(ofVec3f(-1,0,0));
			//ObjectUniqueNormals.push_back(ofVec3f(0,-1,0));
			//ObjectUniqueNormals.push_back(ofVec3f(0,0,-1));
			myPuzzle = new puzzle(posP, offsetSlicer,grid,objectDisplayed->ObjectUniqueNormals);
			myPuzzle->setup();
			mySlicer->intersectCubes((sgCObject*)objectDisplayed->getObject()); 
			myPuzzle->loadPieces(mySlicer->getPieces(),objectID,rotateSlicer);
			myPuzzle->colorFacesMenuPuzzle(objectDisplayed->objectId,uniqueNormalsG,colorsVMenuG);
			//myPuzzle->colorFaces(objectID);
			puzzleFinished = true;
			step = 7;
			SubObEvent ev;
			ev.setName("hide-node");
			ev.addArg("target", prefix + ":start-help");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
		}
	}
	setPage("play2");
}
//----------------------------------------------------------------------
void game::loadObjectG(int objID, SG_VECTOR p, SG_VECTOR t){
	if (objectID == -1){
		objectDisplayed = new myobject3D(p,t,station);
	}else{
		objectDisplayed->exit();
		delete objectDisplayed;
		objectDisplayed = new myobject3D(p,t,station);
	}
	objectID = objID;
	if(step == 0 || step==1 || step == 6){
		if(objID != 200){
			objectDisplayed->loadObjectOD((sgC3DObject*)objects[objectID]->Clone(), objectID);
		}
		//if(objID == 1){
		//	//torus
		//	objectDisplayed->loadObjectOD(sgCreateTorus(100,70,50,50),1);//(radius,thickness,meridiansDonut,meridiansDonutCut)
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}
		//if(objID == 2){
		//	//cube
		//	objectDisplayed->loadObjectOD(sgCreateBox(300,300,300),2);//(tamX,tamY,tamZ)
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}if(objID == 3){
		//	//cone
		//	//objectDisplayed->loadObjectOD(sgCreateCone(250,1,250,3),3);
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgTetrahedron->Clone(),3);
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}
		//if(objID == 4){
		//	//try to load the bunny
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgBunny->Clone(),4);
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}
		//if(objID == 5){
		//	//try to load the dodecahedron
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgDodecahedron->Clone(),5);
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}
		//if(objID == 6){
		//	//try to load the Icosahedron
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgIcosahedron->Clone(),6);
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}
		//}
		//if(objID == 7){
		//	//try to load the Octahedron
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgOctahedron->Clone(),7);
		//	if(extrudedB){
		//		sgDeleteObject(extrudedObject);
		//		extrudedB = false;
		//	}

		//}
		//if(objID == 8){
		//	//try to load the Teapot
		//	objectDisplayed->loadObjectOD((sgC3DObject *)sgTeapot->Clone(),8);
		//}
		if(objID == 200){
			//load extruded object
			objectDisplayed->loadObjectOD((sgC3DObject *)extrudedObject->Clone(),200);
		}
		objectDisplayed->setup();
		step = 1;
	}
}
//----------------------------------------------------------------------
//void game::loadArmature(int type){
//	//loads armature and creates cutter and slicer
//	//have to clear myArmature, to load the new one, like load objects!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	rotateSlicer.x=0;
//	rotateSlicer.y=0;
//	rotateSlicer.z=0;
//
//	if (armID == -1){
//		//first time
//		if(type == 1){
//			tamCubie = 100;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}else if(type == 2){
//			tamCubie = 50;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}else if(type > 1){
//			tamCubie = 100;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}
//		armID = type;
//	}else{
//		//free (myArmature);
//		//delete myArmature;
//		if(type == 1){
//			tamCubie = 100;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}else if(type == 2){
//			tamCubie = 50;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}else if(type > 1){
//			tamCubie = 100;
//			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
//		}
//		armID = type;
//	}
//
//	myArmature->setup();
//	step =3;
//}
//----------------------------------------------------------------------
void game::loadArmature(int type){
	//loads armature and creates cutter and slicer
	//have to clear myArmature, to load the new one, like load objects!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	rotateSlicer.x=0;
	rotateSlicer.y=0;
	rotateSlicer.z=0;

	//if (type==1) {
	//	type=3;
	//}

	if (armID == -1){
		armID=type;
		//first time
		tamCubie = 100;
		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie,armID);
	}else{
		//free (myArmature);
		//delete myArmature;
		armID = type;
		tamCubie = 100;
		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie,armID);
	}

	myArmature->setup();
	step =3;
}

//----------------------------------------------------------------------
void game::loadArmatureMenu(int type){
	//loads armature and creates cutter and slicer
	//have to clear myArmature, to load the new one, like load objects!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//rotateSlicer.x=0;
	//rotateSlicer.y=0;
	//rotateSlicer.z=0;

	//if (armID == -1){
	//	//first time
	//	if(type == 1){
	//		tamCubie = 100;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}else if(type == 2){
	//		tamCubie = 50;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}else if(type > 1){
	//		tamCubie = 100;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}
	//	armID = type;
	//}else{
	//	//free (myArmature);
	//	//delete myArmature;
	//	if(type == 1){
	//		tamCubie = 100;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}else if(type == 2){
	//		tamCubie = 50;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}else if(type > 1){
	//		tamCubie = 100;
	//		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
	//	}
	//	armID = type;
	//}

	//myArmature->setup();
	rotateSlicer.x=0;
	rotateSlicer.y=0;
	rotateSlicer.z=0;

	if (type==1) {
		type=3;
	}

	if (armID == -1){
		armID=type;
		//first time
		tamCubie = 100;
		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie,armID);
	}else{
		//free (myArmature);
		//delete myArmature;
		armID = type;
		tamCubie = 100;
		myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie,armID);
	}

	myArmature->setup();
	step =3;
}
//-----------------------------------------------------------------------------------------
void game::applyArmRotations(){
	//this rotates the object sgC3DObject to be sliced
	objectDisplayed->applyArmRotations(rotateSlicer);
}
//-----------------------------------------------------------------------------------------
void game::createCutterSlicer(){
	////////////////////////////////create cutter
	myCutter = new cutter(planeThicknes,tamCutter,tamCubie,1,offsetSlicer,armID);		
	myCutter->setup();
	//////////////////////////////////create slicer
	mySlicer = new slicer(myCutter,armID);
	mySlicer->setup();
}
//-----------------------------------------------------------------------------------------
void game::createPuzzle(SG_VECTOR p){
	if(step == 3){
		//////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle =  new puzzle(p, offsetSlicer,armID,objectDisplayed->ObjectUniqueNormals); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
		myPuzzle->setup();

		///////////////  BOOLEAN INTERSECTION ///////////////////////////////////
		mySlicer->intersectCubes((sgCObject*)objectDisplayed->getObject());

		//now slicer has all the parts inside sgCGroup ** pieces[]
		//puzzle recieves the armature rotations to undo them and show the puzzle in an original possition
		myPuzzle->loadPieces(mySlicer->getPieces(),objectID,rotateSlicer);
		////////////////////////////////end create puzzle/////////////////////////////////

		///////////////////////////////  color puzzle   ////////////////////////////////// 
		//color all the faces for platonic solids!! colors outside for most objects(not bunny), black on the insides
		if(objectID < 8 ){
			myPuzzle->colorFaces(objectID);
		}else{
			myPuzzle->colorTorus();
		}
		puzzleFinished = true;
		step = 4;
	}
}
//-----------------------------------------------------------------------------------------
void game::createPuzzleOneByOne(int cubieToBeMade){
	mySlicer->intersectCubesOneByOne((sgCObject*)objectDisplayed->getObject(),cubieToBeMade); 

	//now slicer has one more cubie inside sgCGroup ** pieces[]
	//puzzle recieves the armature rotations to undo them and show the puzzle in an original possition
	myPuzzle->loadPiecesOneByOne(mySlicer->getPiecesOneByOne(cubieToBeMade),objectID,rotateSlicer,cubieToBeMade);
	////////////////////////////////end create puzzle/////////////////////////////////

	///////////////////////////////  color puzzle   ////////////////////////////////// 
	//color all the faces for platonic solids!! colors outside for most objects(not bunny), black on the insides
	if(objectID < 8 ){
		myPuzzle->colorFacesOneByOne(objectID,cubieToBeMade);
	}else{
		myPuzzle->colorTorusOneByOne(cubieToBeMade);
	}
}
//----------------------------------------------------------------------
void game::moveP(SG_VECTOR p){
	myPuzzle->move(p);
}
//----------------------------------------------------------------------
//void game::rotateP(SG_VECTOR r){
//	/*myPuzzle->rotate(r);*/
//}
//----------------------------------------------------------------------
int game::getCurrentStep(){
	return step;
}
//----------------------------------------------------------------------
void game::setCurrentStep(int s){
	step = s;
}
//----------------------------------------------------------------------
//void game::changeColorToColor(ofFloatColor sc, ofFloatColor tc){
//	myPuzzle->changeColorToColor(sc,tc);
//}

void game::changeFaceColor(ofVec2f pos, ofFloatColor c){

}

//----------------------------------------------------------------------
void game::moveA (ofVec3f input){
	int xp = offsetSlicer.x + input.x;
	int xn = offsetSlicer.x + input.x;
	int yp = offsetSlicer.y + input.y;
	int yn = offsetSlicer.y + input.y;
	if(xp <= 140 &&  xn >= -140){
		if( yp <= 140 && yn >= -140){
			myArmature->moveA(input);
			//move the offset vector of the cutter at the same time as the armature
			offsetSlicer.x += input.x;
			offsetSlicer.y += input.y;
			offsetSlicer.z += input.z;
		}
	}
}
//----------------------------------------------------------------------
void game::rotateA (ofVec3f input){
	int rx = input.x;
	int ry = input.y;
	int rz = input.z;

	//if(){
	myArmature->rotateA(input);
	//move the offset vector of the cutter at the same time as the armature
	rotateSlicer.x += input.x;
	rotateSlicer.y += input.y;
	rotateSlicer.z += input.z;
	//}
}
//----------------------------------------------------------------------
ofVec3f game::giveOffset(){
	return offsetSlicer;
}
//----------------------------------------------------------------------
void game::unDo(){
	//to do 1 undo:
	//undo will look for the other 9 cubies involved and do add a move opposite to the last one
	//when it stops animation andits on the new place
	//take 2 (two) pop_back on each of the cubies history
	if(historyV.size()>0){//game history
		int idcubie = historyV[historyV.size()-1].id;
		SG_VECTOR axis = historyV[historyV.size()-1].vector;
		double dir = historyV[historyV.size()-1].dir;
		myPuzzle->unDo(idcubie,axis,dir);
		historyV.pop_back();
	}
}
//----------------------------------------------------------------------
void game::unDoMenuPuzzle(){
	//this completley undos the rotation matrix for each cubie on the puzzle
	if(historyV.size()>0){//game history
		myPuzzle->unDoMenuPuzzle();
		historyV.clear();//pop_back();
	}
}
//----------------------------------------------------------------------
void game::guiLoad(int _obj){
	/*
	SG_VECTOR objectPos = {0,0,0};
	loadObjectOD(_obj, objectPos, posP);
	*/
	bHaveNewObject = true;
	newObject = _obj;
}

//----------------------------------------------------------------------
void game::guiInput(int in){

	//any gui input resets reset timer
	//start timer to go back to inactive state
	if(step != -1){
		goToAttractStepI =  ofGetElapsedTimef();
	}
	if(in == 'b'){
		ofToggleFullscreen();
	}

	//////
	if(step == -1){
		//on attract / inactive state
		step = 0;
		goToAttractStepI =  ofGetElapsedTimef();
	}
	////////////////////////////////////////////step 0 inputs
	////////////////////////////////////////////step 0 inputs
	////////////////////////////////////////////step 0 inputs
	else if(step == 0){
		//waiting for an object to be selected
		//it cam be from the shapes on the center
		// or from the object menu on the side
		////////////////////////////////////////////////////////
		//////////////////object menu on the side
		//waiting for shape to be selected
		if(in == '1') {
			//load object recieves (object id, boolean position, display position) 
			loadObjectG(1,slicingPos,posP);
		}
		if(in == '2') {
			loadObjectG(2,slicingPos,posP);
		}
		if(in == '3') {
			loadObjectG(3,slicingPos,posP);
		}
		if(in == '4') {
			loadObjectG(4,slicingPos,posP);
		}
		if(in == '5') {
			loadObjectG(5,slicingPos,posP);
		}
		if(in == '6') {
			loadObjectG(6,slicingPos,posP);
		}
		if(in == '7') { 
			loadObjectG(7,slicingPos,posP);
		}
		//if(in == '8') { 
		//	loadObjectG(8,objectPos,posP);
		//}
		if(in == '9') {
			prepareDrawing();
		}
	}
	////////////////////////////////////////////step 1 inputs
	////////////////////////////////////////////step 1 inputs
	////////////////////////////////////////////step 1 inputs
	else if(step == 1){
		//is showing object with flat color
		//selected an object
		if(in == 'n') {
			//go to step 2
			setCurrentStep(2);
			//show armature
		} else{
			//waiting for shape to be selected
			if(in == '1') {
				//user can change the selected object
				clearDisplayedObject();
				//load object recieves (object id, boolean position, display position) 
				loadObjectG(1,slicingPos,posP); //pos.z its the torus radious
			}
			if(in == '2') {
				clearDisplayedObject();
				loadObjectG(2,slicingPos,posP);
			}
			if(in == '3') {
				clearDisplayedObject();
				loadObjectG(3,slicingPos,posP);
			}
			if(in == '4') {
				clearDisplayedObject();
				loadObjectG(4,slicingPos,posP);
			}
			if(in == '5') {
				clearDisplayedObject();
				loadObjectG(5,slicingPos,posP);
			}
			if(in == '6') {
				//user can change the selected object
				clearDisplayedObject();
				loadObjectG(6,slicingPos,posP);
			}
			if(in == '7') {
				//user can change the selected object
				clearDisplayedObject();
				loadObjectG(7,slicingPos,posP);
			}
			/*if(in == '8') { 
			loadObjectG(8,objectPos,posP);
			}*/
			if(in == '9') { 
				clearDisplayedObject();
				prepareDrawing();
			}
		}
	}
	////////////////////////////////////////////step 2 inputs
	////////////////////////////////////////////step 2 inputs
	////////////////////////////////////////////step 2 inputs
	else if(step == 2){
		//waiting for armature to be selected
		if(in == '1') {
			//3 is number of slices
			loadArmature(3); 
		}
		if(in == '2') {
			loadArmature(2);
		}
		if(in == '3') {
			loadArmature(3);
		}
		if(in == '4') {
			loadArmature(4);
		}
		if(in == '5') {
			loadArmature(5);
		}
		if(in == '6') {
			loadArmature(6);
		}
	}
	////////////////////////////////////////////step 3 inputs
	////////////////////////////////////////////step 3 inputs
	////////////////////////////////////////////step 3 inputs
	else if(step == 3){
		//armature was selected
		////showing armature    another armature can be selected
		if(in == '1') {
			//3 is number of slices
			loadArmature(3); 
		}
		if(in == '2') {
			loadArmature(2);
		}
		if(in == '3') {
			loadArmature(3);
		}
		if(in == '4') {
			loadArmature(4);
		}
		if(in == '5') {
			loadArmature(5);
		}
		if(in == '6') {
			loadArmature(6);
		}
		//////////////////////////////move all armature
		if(in == 'l') {
			ofVec3f p = ofVec3f (5,0,0);
			moveA(p);
		}
		if(in == 'j') {
			ofVec3f p = ofVec3f (-5,0,0);
			moveA(p);
		}
		if(in == 'i') {
			ofVec3f p = ofVec3f (0,-5,0);
			moveA(p);
		}
		if(in == 'k') {
			ofVec3f p = ofVec3f (0,5,0);
			moveA(p);
		}
		///z movement
		if(in == 'o') {
			ofVec3f p = ofVec3f (0,0,5);
			moveA(p);
		}
		if(in == 'p') {
			ofVec3f p = ofVec3f (0,0,-5);
			moveA(p);
		}//////////////////////////////rotate all armature
		if(in == 'c') {//rotate around y
			ofVec3f r = ofVec3f (0,5,0);
			rotateA(r);
		}
		if(in == 'x') {//rotate around y
			ofVec3f r = ofVec3f (0,-5,0);
			rotateA(r);
		}
		if(in == 'w') {//rotate around x
			ofVec3f r = ofVec3f (5,0,0); //degrees!!!
			rotateA(r);
		}
		if(in == 's') {//rotate around x
			ofVec3f r = ofVec3f (-5,0,0);
			rotateA(r);
		}
		if(in == 'e') {//rotate around z
			ofVec3f r = ofVec3f (0,0,5); //degrees!!!
			rotateA(r);
		}
		if(in == 'd') {//rotate around z
			ofVec3f r = ofVec3f (0,0,-5);
			rotateA(r);
		}
		/////////////////a puzzle can be made
		if(in == 'n') {
			if(creatingPuzzle==false){
				numPuzzlePieces = 0;
				cubieToCut = 0;
				////////////////////////////////create puzzle///////////////////////////////////////
				//select the unique normals for the object that wants to be a puzzle
				myPuzzle =  new puzzle(posP, offsetSlicer,armID, objectDisplayed->ObjectUniqueNormals); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
				myPuzzle->setup();

				//send the armature rotations to the 3dObject
				applyArmRotations();
				//now we know the offset position from the armature to create-> cutter & slicer
				createCutterSlicer();

				//get the number of pieces from the slicer...this changes with each armature
				numPuzzlePieces = mySlicer->numPieces;
				//open boolean gate to construct puzzle cubie by cubie, on every update
				creatingPuzzle = true;

				////create puzzle all at once
				//createPuzzle(posP);//create Puzzle goes to step 4 to show the puzzle
			}
		}
	}
	////////////////////////////////////////////step 4 inputs
	////////////////////////////////////////////step 4 inputs
	////////////////////////////////////////////step 4 inputs
	else if(step == 4){
		//showing puzzle with colors
		//waiting for color change
		if(in == '1') {
			//call color change funtion
			//ofFloatColor sc = ofFloatColor (1, 1, 0); //yellow
			//ofFloatColor menuColor = ofFloatColor (1, 0, 1); //this color comes from the GUI
			//changeColorToColor(sc,menuColor);
		}
		//pressed NEXT
		if(in == 'n') {
			//go to step 5
			setCurrentStep(5);
		}
		///////////////////////////////move all puzzle
		if(in == 'l') {
			SG_VECTOR p = {10,0,0};
			moveP(p);
		}
		if(in == 'j') {
			SG_VECTOR p = {-10,0,0};
			moveP(p);
		}
		if(in == 'i') {
			SG_VECTOR p = {0,-10,0};
			moveP(p);
		}
		if(in == 'k') {
			SG_VECTOR p = {0,10,0};
			moveP(p);
		}
		/////////////rotate all puzzle  // two finger swipe gesture
		//if(in == 'm') {//rotate right
		//	SG_VECTOR r = {0,10,0};//{0,0.1,0};
		//	rotateP(r);
		//}
		//if(in == 'n') {//rotate left
		//	SG_VECTOR r = {0,-10,0};//{0,-0.1,0};
		//	rotateP(r);
		//}
		//if(in == 'y') {//rotate up
		//	SG_VECTOR r =  {10,0,0};// {0.1,0,0};
		//	rotateP(r);
		//}
		//if(in == 'h') {//rotate down
		//	SG_VECTOR r = {-10,0,0};//{-0.1,0,0};
		//	ofVec3f v;
		//	rotateP(r);
		//}
	}
	////////////////////////////////////////////step 5 inputs
	////////////////////////////////////////////step 5 inputs
	////////////////////////////////////////////step 5 inputs
	else if(step == 5){
		//selected color (or not, its not mandatory)
		//pressed next on color palette step
		//showing puzzle
		//now the puzzle can be played with
		int randcubie=7;//rand()%26;//to follow this cubie for now //this will be decided upon touch, or click on top of puzzle
		if(myPuzzle->isMoving() == false){ //this is to prevent from reading events while puzzle is moving
			//if(in == 'u'){
			//	//undo last move 
			//	unDo();
			//}
			/////////////////////////////////////////////// SAVE PUZZLE /////////////////////////////////////////
			if(in == '.') {
				//save current puzzle and put it on the middle puzzle section
				savePuzzleB = true;
			}
			////////////////////////////////////////////// FACE ROTATIONS 2 ids /////////////////////////////////
			//if(in == 'z') {
			//	//do rotationbased ontwo cubies id
			//	int cubieA = 11;
			//	int cubieB = 10;
			//	rotateTwoIds(cubieA,cubieB,true);
			//}
			///////////////////////do go back animation
			if(in == 'z') {
				// simulates mouse released
				//it activates forward, or go back animations
				decideMove();
			}
			////////////////////////////////////////////// FACE ROTATIONS axis dir //////////////////////////////
			////////  x axis  ////  x axis
			if(in == 'q') {
				//clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,true,7);
			}
			if(in == 'a') {
				//counter clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
			////////  y axis  ////  y axis
			if(in == 'w') {
				//clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,true,7);
			}if(in == 's') {
				//counter clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
			////////  z axis  ////  z axis
			if(in == 'e') {
				//clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,true,7);
			}if(in == 'd') {
				//counter clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
		}
		////////////////////////////////////move all puzzle
		if(in == 'l') {
			SG_VECTOR p = {10,0,0};
			moveP(p);
		}
		if(in == 'j') {
			SG_VECTOR p = {-10,0,0};
			moveP(p);
		}
		if(in == 'i') {
			SG_VECTOR p = {0,-10,0};
			moveP(p);
		}
		if(in == 'k') {
			SG_VECTOR p = {0,10,0};
			moveP(p);
		}
		/////////////rotate all puzzle  // two finger swipe gesture
		//if(in == 'm') {//rotate right
		//	SG_VECTOR r = {0,2,0};//{0,0.1,0};
		//	rotateP(r);
		//}
		//if(in == 'n') {//rotate left
		//	SG_VECTOR r = {0,-2,0};//{0,-0.1,0};
		//	rotateP(r);
		//}
		//if(in == 'y') {//rotate up
		//	SG_VECTOR r =  {2,0,0};// {0.1,0,0};
		//	rotateP(r);
		//}
		//if(in == 'h') {//rotate down
		//	SG_VECTOR r = {-2,0,0};//{-0.1,0,0};
		//	ofVec3f v;
		//	rotateP(r);
		//}
		//if(in == 'o') {//rotate c
		//	SG_VECTOR r =  {0,0,-2};// {0.1,0,0};
		//	rotateP(r);
		//}
		//if(in == 'p') {//rotate cc
		//	SG_VECTOR r = {0,0,2};//{-0.1,0,0};
		//	ofVec3f v;
		//	rotateP(r);
		//}
	}else if(step == 6){
		////////////////////////////extrusion
		if(in == 'e') {
			//take drawing data
			//check for existing drawing
			if(myCanvas.drawingExists()){
				//make extruded object
				if(extrudeObject(myCanvas.getPolyline())){

				}else{
					prepareDrawing();
				}
			}
		}else if(in == 's'){
			ofPolyline *draw =  new ofPolyline();
			//star
			draw->addVertex(ofVec2f(-5,-210));
			draw->addVertex(ofVec2f(60,-60));
			draw->addVertex(ofVec2f(210,-45));
			draw->addVertex(ofVec2f(105,60));
			draw->addVertex(ofVec2f(150,210));
			draw->addVertex(ofVec2f(-5,135));
			draw->addVertex(ofVec2f(-150,210));
			draw->addVertex(ofVec2f(-105,60));
			draw->addVertex(ofVec2f(-210,-45));
			draw->addVertex(ofVec2f(-60,-60));
			draw->addVertex(ofVec2f(-5,-210));

			//draw->close(); // close the shape
			extrudeObject(draw);
		}
		////////////////////////////////////////////////////////
		//////////////////object menu on the side
		//waiting for shape to be selected
		else if(in == '1') {
			clearDisplayedObject();
			//load object recieves (object id, boolean position, display position) 
			loadObjectG(1,slicingPos,posP);
		}
		else if(in == '2') {
			clearDisplayedObject();
			loadObjectG(2,slicingPos,posP);
		}
		else if(in == '3') {
			clearDisplayedObject();
			loadObjectG(3,slicingPos,posP);
		}
		else if(in == '4') {
			clearDisplayedObject();
			loadObjectG(4,slicingPos,posP);
		}
		else if(in == '5') {
			clearDisplayedObject();
			loadObjectG(5,slicingPos,posP);
		}
		else if(in == '6') {
			clearDisplayedObject();
			loadObjectG(6,slicingPos,posP);
		}
		else if(in == '7') { 
			clearDisplayedObject();
			loadObjectG(7,slicingPos,posP);
		}
		//if(in == '8') { 
		//	loadObjectG(8,objectPos,posP);
		//}
		else if(in == '9') { 
			clearDisplayedObject();
			prepareDrawing();
		}
	}else if(step == 7){
		//selected color (or not, its not mandatory)
		//pressed next on color palette step
		//showing puzzle
		//now the puzzle can be played with
		int randcubie=7;//rand()%26;//to follow this cubie for now //this will be decided upon touch, or click on top of puzzle
		if(myPuzzle->isMoving() == false){ //this is to prevent from reading events while puzzle is moving
			//if(in == 'u'){
			//	//undo last move 
			//	unDo();
			//}
			/////////////////////////////////////////////// SAVE PUZZLE /////////////////////////////////////////
			if(in == '.') {
				//save current puzzle and put it on the middle puzzle section
				savePuzzleB = true;
			}
			////////////////////////////////////////////// FACE ROTATIONS 2 ids /////////////////////////////////
			//if(in == 'z') {
			//	//do rotationbased ontwo cubies id
			//	int cubieA = 11;
			//	int cubieB = 10;
			//	rotateTwoIds(cubieA,cubieB,true);
			//}
			///////////////////////do go back animation
			if(in == 'z') {
				// simulates mouse released
				//it activates forward, or go back animations
				decideMove();
			}
			////////////////////////////////////////////// FACE ROTATIONS axis dir //////////////////////////////
			////////  x axis  ////  x axis
			if(in == 'q') {
				//clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,true,7);
			}
			if(in == 'a') {
				//counter clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
			////////  y axis  ////  y axis
			if(in == 'w') {
				//clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,true,7);
			}if(in == 's') {
				//counter clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
			////////  z axis  ////  z axis
			if(in == 'e') {
				//clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,true,7);
			}if(in == 'd') {
				//counter clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,false,7);
			}
		}
		////////////////////////////////////move all puzzle
		if(in == 'l') {
			SG_VECTOR p = {10,0,0};
			moveP(p);
		}
		if(in == 'j') {
			SG_VECTOR p = {-10,0,0};
			moveP(p);
		}
		if(in == 'i') {
			SG_VECTOR p = {0,-10,0};
			moveP(p);
		}
		if(in == 'k') {
			SG_VECTOR p = {0,10,0};
			moveP(p);
		}
	}

	/////////////////////////////////////////////////////////////////
	////////////////RESART button can work on any step, its not showed on step 1 (object selected)
	if(in == 'r'){
		//tell a game to restart 
		restart();
	}
}
//----------------------------------------------------------------------
void game::decideMove(){
	//it looks at current face rotation angle and decides to do a cmplete move, or go back to previous possition
	myPuzzle->decideMove(axis);
	//}
}
//----------------------------------------------------------------------
bool game::extrudeObject(ofPolyline *drawing){
	//this function extrudes the input ofPolylne

	vector< ofPoint > points = drawing->getVertices();

	sgCObject*  cont_objcts[2000];///////////////////////////////////////////////////////////////////////////////clean this memeory!!!
	for (int i =0; i < points.size() ; i ++)  {
		if(i!= points.size()-1){
			cont_objcts[i] = sgCreateLine(points[i].x, 0, points[i].y,points[i+1].x, 0, points[i+1].y);
		}else{
			//its the last point.. make the line with the first point
			cont_objcts[i] = sgCreateLine(points[i].x, 0, points[i].y,points[0].x, 0, points[0].y);
		}
	}  

	sgCContour* win_cont = sgCContour::CreateContour(cont_objcts, points.size()-1);

	//check if its self intersecting
	if(win_cont->IsSelfIntersecting()){
		//its self intersecting
		//abort!!!
		extrudedB = false;
		//clear ofpolylines!!!
		//myCanvas->myPolyline->clear();
		//free(myCanvas->myPolyline);
		//myCanvas->myPolyline2->clear();
		free(drawing);
		sgDeleteObject(win_cont);
		return false;
	}else{
		////extrude along vector
		SG_VECTOR extVec = {0,-300,0};  
		if (objectID == -1){
			extrudedObject = (sgC3DObject*)sgKinematic::Extrude(*win_cont,NULL,0,extVec,true);
		}else{
			free(extrudedObject);
			extrudedObject = (sgC3DObject*)sgKinematic::Extrude(*win_cont,NULL,0,extVec,true);
		}

		extrudedB = true;
		//we  have the sg3DObjcect to load
		loadObjectG(200,slicingPos,posP);//using id=200

		//free(drawing);
		sgDeleteObject(win_cont);
		return true;
	}
	////////free(cont_objcts);
}
//----------------------------------------------------------------------
void game::extrudeObject(){
	//create and use circle
	SG_POINT   crCen = {0,0,0.0};  
	SG_VECTOR  crNor;  
	crNor.x = 0.0;  
	crNor.y = 3.0;  
	crNor.z = 0.0;
	sgSpaceMath::NormalVector(crNor); 
	SG_CIRCLE  crGeo;  
	crGeo.FromCenterRadiusNormal( crCen, 150, crNor);  
	sgCCircle* cr = sgCreateCircle(crGeo);   


	////extrude along vector
	SG_VECTOR extVec = {0,-300,0};  

	if (objectID == -1){
		extrudedObject = (sgC3DObject*)sgKinematic::Extrude((const sgC2DObject&)(*cr),NULL,0,extVec,true);
	}else{
		free(extrudedObject);
		extrudedObject = (sgC3DObject*)sgKinematic::Extrude((const sgC2DObject&)(*cr),NULL,0,extVec,true);
	}

	extrudedObject->SetAttribute(SG_OA_COLOR,30);
	extrudedB = true;
	sgDeleteObject(cr);

	//we  have the sg3DObjcect to load
	loadObjectG(200,slicingPos,posP);//using id=200

}
//----------------------------------------------------------------------
void game::prepareDrawing(){
	if(canvasB == false){
		//create canvas object
		//myCanvas = new drawingCanvas(posCanvas,canvasSide,canvasSide);
		myCanvas.reset();
		//canvasB = true;
	}
	else{
		/*myCanvas->exit();
		delete myCanvas;*/
		//create canvas object
		//myCanvas = new drawingCanvas(posCanvas,canvasSide,canvasSide);
		myCanvas.reset();
		//myCanvas.setViewport(viewport);
	}
	//extrusion
	step = 6;
}
//---------------------------------------------------------------------
void  game::savePuzzle(){
	hasSaved = true;
	////return the puzzle that has been created
	////build a menuPuzzle object and give it to the mainApp
	//menuPuzzle *puzzleToSave = new menuPuzzle(slicingPos, middlePuzzlePos, puzzleCounter);
	//////puzzleToSave->loadObjectMP(objectDisplayed->getObject(),objectID);
	//////puzzleToSave->setup();
	//////puzzleToSave->update();
	//////puzzleToSave->colorFacesMenu();

	//////////////need to pass this data to menuPuzzle
	//////////////vector< ofFloatColor > colorsVMenu;
	//////////////vector< ofVec3f > uniqueNormals;
	//////////////ofVec3f offsetSlicer;
	//////////////ofVec3f rotateSlicer;

	////////////////puzzleToSave->loadPuzzle(myPuzzle);
	////////////puzzleToSave->objectId = objectID; 

	////////////////////////////////////////////////////////////////////////////
	//////no need to have the object IF we are showing puzzles in the middle, not unsliced objects
	////puzzleToSave->loadObjectMP((sgC3DObject*)objectsMP[i+1]->Clone(),i+1);
	////puzzleToSave->setup();
	////puzzleToSave->update();
	////puzzleToSave->colorFacesMenu();//implement this later
	//puzzleToSave->init();//gui

	////we have one puzzle, use this puzzle to 
	//puzzleToSave->offsetSlicer = offsetSlicer;
	//puzzleToSave->rotateSlicer = rotateSlicer;
	////hasSaved = true;
	//return puzzleToSave;
}
//----------------------------------------------------------------------
void game::clearDisplayedObject(){
	if (objectID != -1){
		objectDisplayed->exit();
		delete objectDisplayed;
		objectID = -1;
	}
	if(canvasB){
		//myCanvas->exit();
		//delete myCanvas;
		canvasB = false;
	}
}
//----------------------------------------------------------------------
void game::restart(){
	if(step == 7){
		//myPuzzle->exit();//if we pass this puzzle to the saved games, we cant delete it!!!!!!!!!or we delete the menu puzzle for the future
		//objectDisplayed->exit();

		//have to undo the moves made on the puzzle that was loaded
		//unDoMenuPuzzle();
		step = 0;
		objectID = -1;
		puzzleFinished = false;
		creatingPuzzle = false;
	}else if(step == 6){
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
		step = 0;
		objectID = -1;
	}else if(step==4 || step==5){
		//dont delete the puzzle if it has been saved
		//if(!hasSaved){
		myPuzzle->exit();
		//}
		hasSaved = false;
		myCutter->exit();
		mySlicer->exit();
		objectDisplayed->exit();
		objectID = -1;
		step = 0;
		armID = -1;
		puzzleFinished = false;
		creatingPuzzle = false;
	}else if (step==3){
		objectDisplayed->exit();             //clean displayed object after puzzle is created, so we dont keep it until the exit or restart
		step = 0;
		objectID = -1;
		armID = -1;
	}else if (step==1 || step==2){
		objectDisplayed->exit();
		step = 0;
		objectID = -1;
		if(canvasB){
			//myCanvas->exit();
			//delete myCanvas;
			myCanvas.reset();
			canvasB = false;
		}
	}

	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;

	rotateSlicer.x = 0;
	rotateSlicer.y = 0;
	rotateSlicer.z = 0;

	curRot.set (ofVec4f(0,0,0,0));
	historyV.clear();

	if(extrudedB){
		sgDeleteObject(extrudedObject);
		extrudedB = false;
	}
}
//----------------------------------------------------------------------
void game::exit(){
	//sgDeleteObject(sgBunny);
	////////////have to delete the objects in 
	//map<int, sgCObject*> objects;
	int x = objects.size(); 
	for(int i=0;i<x;i++){
		free(objects[i]);
	}
}
//--------------------------------------------------------------
void game::mouseDragged(int x, int y, int button){
	if(step == 4 || step == 5 || step == 7){
		//ofVec2f mouse(x,y);
		//ofQuaternion yRot(x-lastMouse.x, ofVec3f(0,1,0));
		//ofQuaternion xRot(y-lastMouse.y, ofVec3f(-1,0,0));
		////curRot *= yRot*xRot;
		//curRot.set(curRot*yRot*xRot);
		//lastMouse = mouse;
		//curRot.getRotate(angle, axistb);
		//camPosition.rotate(angle * 0.01, ofVec3f(posP.x, posP.y, posP.z), axistb);

		ofVec3f mouse(x,y);
		ofVec3f r = lastMouse - mouse;
		camPosition.rotate (r.y * 0.01, ofVec3f(posP.x, posP.y, posP.z), ofVec3f(1,0,0));
		camPosition.rotate (r.x * 0.01, ofVec3f(posP.x, posP.y, posP.z), ofVec3f(0,1,0));
	}else if(step == 6){
		myCanvas.mouseDragged(x,y,button);
	} else if(step == 3){
		ofVec3f mouse(x,y);
		ofVec3f r = mouse - lastMouse;
		rotateA(r);
		lastMouse = mouse;
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void game::mousePressed(int x, int y, int button){
	if(step == 3 || step == 4 || step == 5 || step == 7){
		lastMouse = ofVec2f(x,y);
	}else if(step == 6){
		myCanvas.mousePressed(x,y,button);
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void game::mouseReleased(int x, int y, int button){
	if(step == 6){
		myCanvas.mouseReleased(x,y,button);
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
}
//----------------------------------------------------------------------------------------------------------------------
void game::startMove(ofVec3f _pnt){
	if (myPuzzle->isMoving()){
		return;
	}
	if(myPuzzle->activeCubie < 0){
		return;
	}

	else{ 
		// get the id number of the cubie and keep track of it
		int id = myPuzzle->activeCubie;

		// keep track of the mouse/touch position as well

		ofVec3f twistStartPoint = ofVec3f((float)_pnt.x / (float)ofGetWidth(),(float)_pnt.y / (float)ofGetHeight());
		//twistStartPoint = new Point(e.GetPosition(view1).X, e.GetPosition(view1).Y);
		double dist = 300;

		// translate the six axes into 2D screen/camera space:

		cp0 = picker.project(rotateToArmature(ofVec3f (0,0,0)), &viewport);
		cp1 = picker.project(rotateToArmature(ofVec3f (dist,0,0)), &viewport);
		cp2 = picker.project(rotateToArmature(ofVec3f (-dist,0,0)), &viewport);
		cp3 = picker.project(rotateToArmature(ofVec3f (0,dist,0)), &viewport);
		cp4 = picker.project(rotateToArmature(ofVec3f (0,-dist,0)), &viewport);
		cp5 = picker.project(rotateToArmature(ofVec3f (0,0,dist)), &viewport);
		cp6 = picker.project(rotateToArmature(ofVec3f (0,0,-dist)), &viewport);

		double diffx=cp1.distance(cp2);
		double diffy=cp3.distance(cp4);
		double diffz=cp5.distance(cp6);

		if (diffx<diffz && diffx<diffy) {
			normalAng=0;
		} else if (diffy<diffz) {
			normalAng=1;
		} else {
			normalAng=2;
		}
		//Point cp0 = Viewport3DHelper.Point3DtoPoint2D(view1.Viewport, new Point3D(0, 0, 0));

		//// and calculate their angles (in radians) from the center of the axes:
		gestureAngles[0] = atan2(cp1.y - cp0.y, cp1.x - cp0.x);
		gestureAngles[1] = atan2(cp2.y - cp0.y, cp2.x - cp0.x);
		gestureAngles[2] = atan2(cp3.y - cp0.y, cp3.x - cp0.x);
		gestureAngles[3] = atan2(cp4.y - cp0.y, cp4.x - cp0.x);
		gestureAngles[4] = atan2(cp5.y - cp0.y, cp5.x - cp0.x);
		gestureAngles[5] = atan2(cp6.y - cp0.y, cp6.x - cp0.x);
	}
}
//----------------------------------------------------------------------------------------------------
ofVec3f game::rotateToArmature(ofVec3f ax) {
	ofVec3f temp=ax.getRotated(rotateSlicer.z,ofVec3f(0,0,1));
	temp=temp.getRotated(rotateSlicer.x,ofVec3f(0,1,0));
	temp=temp.getRotated(-rotateSlicer.y,ofVec3f(1,0,0));
	return temp;
}
//----------------------------------------------------------------------------------------------------
void game::drawPoints(){
	ofFill();
	ofSetColor(255,0,0);
	ofCircle(cp1,10);
	ofCircle(cp2,5);
	ofSetColor(0,255,0);
	ofCircle(cp3,10);
	ofCircle(cp4,5);
	ofSetColor(0,0,255);
	ofCircle(cp5,10);
	ofCircle(cp6,5);
	ofSetColor(0,0,0);
	ofCircle(cp0,10);
}
//--------------------------------------------------------------------------------------------------------------------------
void game::makeMove(ofVec3f _pnt){
	//SG_POINT axis;
	// get angle of gesture
	double dragDist=sqrt((_pnt.y*_pnt.y)+(_pnt.x*_pnt.x));
	if(bHaveAxis == false){
		if(dragDist > 75){

			double dragAngle = atan2(_pnt.y,_pnt.x);//atan2(newPt.Y - twistStartPoint.Y, newPt.X - twistStartPoint.X);

			double offset = 100000;
			int closest=-1;
			for (int i = 0; i < 6; i++)
			{
				if (i!=normalAng*2 && i!=normalAng*2+1) {
					double dist= abs(dragAngle-gestureAngles[i]);
					double dist2= abs(dragAngle-gestureAngles[i]+(2*3.14159));
					double dist3= abs(dragAngle-gestureAngles[i]-(2*3.14159));

					if (dist < offset){
						offset = dist;
						closest = i;
					} 
					if (dist2 < offset){
						offset = dist2;
						closest = i;
					} 
					if (dist3 < offset){
						offset = dist3;
						closest = i;
					} 
				}
			}


			cubiePos=myPuzzle->getCubieInfo(myPuzzle->activeCubie);

			//	cout << "CAMERA NORMAL IS " << normalAng << endl; 

			if (closest==1 || closest==0) {
				// x axis
				bHaveAxis = true;
				if (normalAng==1) {
					// rotate on z
					axis.x = 0;
					axis.y = 0;
					axis.z = 1;
					dir=true;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).y >= armID/2) {
						dir = false;
					}
					if (closest==1) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);

				}else{
					// rotate on y
					axis.x = 0;
					axis.y = 1;
					axis.z = 0;

					dir=true;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).z < armID/2) {
						dir = false;
					}
					if (closest==1) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);
				}
			}
			if (closest==3 || closest==2) {
				// y axis
				bHaveAxis = true;
				if (normalAng==0) {
					// rotate on z
					axis.x = 0;
					axis.y = 0;
					axis.z = 1;
					dir=true;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).x < armID/2) {
						dir = false;
					}
					if (closest==3) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);

				} else {
					// rotate on x
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;

					dir=false;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).z < armID/2) {
						dir = true;
					}
					if (closest==3) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);
				}
			}
			if (closest==4 || closest==5) {
				// z axis
				bHaveAxis = true;

				if(normalAng==0){
					// rotate on y
					axis.x = 0;
					axis.y = 1;
					axis.z = 0;
					dir=false;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).x < armID/2) {
						dir = true;
					}
					if (closest==5) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);
				} else {
					// rotate on x
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;

					dir=true;
					if(myPuzzle->getCubieInfo(myPuzzle->activeCubie).y < armID/2) {
						dir = false;
					}
					if (closest==5) {
						dir=!dir;
					}
					rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,10);

				}
			}
		}
	}else{
		if (((dragDist-lastDragDistance)/300)>.5) {
			rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,5);
		} else if (((dragDist-lastDragDistance)/300)<-.5) {
			rotateByIDandAxis(myPuzzle->activeCubie,axis,dir,-5);
		}
	}
	lastDragDistance=dragDist;
}
//-----------------------------------------------------------------------------------------------------------------------
float game::getMainComponent(ofVec3f _pnt){
	ofVec3f x(_pnt.x, 0, 0);
	ofVec3f y(0, _pnt.y, 0);
	ofVec3f z(0, 0, _pnt.z);
	float magX = x.lengthSquared();
	float magY = y.lengthSquared();
	float magZ = z.lengthSquared();
	if(magX > magY && magX > magZ){
		return _pnt.x;
	}
	if(magY > magX && magY > magZ){
		return _pnt.y;
	}
	if(magZ > magX && magZ > magY){
		return _pnt.z;
	}
}

//----------------------------------------------------------------------------------------------------------------------------

void game::task1(string msg){
	cout << "task1 says: " << msg << endl;
}