#include "testApp.h"
#include "game.h"
///////////////////////////////////////////

#define displayX 500
#define displayY 400
#define displayZ -800
#define iddleTime 120
#define puzzleItems 2

std::map<int,gwc::Point> active_points;

//--------------------------------------------------------------
void testApp::setup(){

	/*
	if(loadGestureWorks("GestureworksCore32.dll")) { 
		std::cout << "Error loading gestureworks dll" << std::endl; 
		exit(); 
	}

	//string gmlFile = "basic_manipulation.gml";

	if(!loadGML("C:\\Users\\pp\\dev\\of_v0.8.0_vs_release\\apps\\myApps\\rubiks\\bin\\basic_manipulation.gml")) { 
		std::cout << "Could not find gml file" << std::endl; 
		exit(); 
	}

	initializeGestureWorks(1920,1080);

	if(!registerWindowForTouchByName("rubiksWindow")) { 
		std::cout << "Could not register target window for touch." << std::endl; 
		exit(); 
	}
	registerTouchObject("touchReceiver");

	addGesture("touchReceiver","n-drag");
	addGesture("touchReceiver","n-scale");
	addGesture("touchReceiver","n-rotate");
	*/

	/////////////////////////////initialize sgCore library
	sgInitKernel();

	/////////////////////////////initialize openFrameworks rendering
	initOFRender();

	/////////////////////////////load obj files into sgCore objects
	loadOBJfiles();
	//////////////////////////////create games
	//////////////////////////////one game for now
	SG_VECTOR gamePos = {0,0,0}; //one game created at the origin // this will have to change for a game creating function when more stations are anabled
	//SG_VECTOR displayPos = {ofGetWidth() / 2, ofGetHeight() / 2,displayZ};
	SG_VECTOR displayPos = {550, 1150,displayZ};
	game *tempGame = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos,iddleTime);
	myGames.push_back(tempGame);
	currentGame = 1;
	//create a second game
	//game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos,iddleTime);
	//myGames.push_back(tempGame2);
	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->setup(sgBunny->Clone(),sgTetrahedron->Clone(),sgDodecahedron->Clone(),sgIcosahedron->Clone(),sgOctahedron->Clone());//,sgTeapot->Clone());
	}

	//////setup GUI/////////////
	//ofSetFullscreen(true);

	//gm = new GestureManager();
	gm.loadGMLFile("basic_manipulation.gml");
	gm.init("rubiksWindow", ofGetWidth(), ofGetHeight());
	//gm->start();

	string guiFile = "sheets.xml";
	GuiConfigurator::Instance()->addFile(guiFile);
	GuiConfigurator::Instance()->loadGui();

	SubObMediator::Instance()->addObserver("button", this);
	SubObMediator::Instance()->addObserver("object-selected", this);
	SubObMediator::Instance()->addObserver("armature-selected", this);
	SubObMediator::Instance()->addObserver("next-step", this);
	SubObMediator::Instance()->addObserver("cut-object", this);
	SubObMediator::Instance()->addObserver("goto-step5", this);
	SubObMediator::Instance()->addObserver("reset", this);
	SubObMediator::Instance()->addObserver("touch-point", this);
	SubObMediator::Instance()->addObserver("gesture", this);

	ev = new SubObEvent();

	active_points = std::map<int,gwc::Point>();
	//rotate = true;

	touchId = 0;
	touchIdOffset = 1000;

	//////////////////////////////generate puzzles for middle area
	////////////////////////////////create puzzle///////////////////////////////////////
	puzzleCounter =1000;
	////////////////////////////////create cutter
	ofVec3f offsetSlicer = ofVec3f(0,0,0);
	myCutter = new cutter(0.01,1000,100,1,offsetSlicer);		
	myCutter->setup();
	//////////////////////////////////create slicer
	mySlicer = new slicer(myCutter);
	mySlicer->setup();
	SG_POINT slicingPos = {0,0,0};
	SG_VECTOR middlePuzzlePos = {200,100,0};
	ofVec3f rotateSlicer = ofVec3f (0,0,0);

	objectDisplayed = new myobject3D(gamePos,displayPos);

	for(int i=0; i < puzzleItems; i++){
		objectDisplayed->loadObject(sgCreateBox(300,300,300),2);
		mySlicer->intersectCubes((sgCObject*)objectDisplayed->getObject());

		middlePuzzlePos.x = 200 + (i*300) + (i*100);
		myPuzzle = new puzzle(middlePuzzlePos, offsetSlicer); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
		myPuzzle->setup();

		myPuzzle->loadPieces(mySlicer->getPieces(),101,rotateSlicer);//selected object id is used for coloring
		myPuzzle->colorFaces(101);
		middlePuzzles.push_back(new menuPuzzle(myPuzzle,puzzleCounter));
		//puzzleCounter ++;
	}
	ofEnableAntiAliasing();
}
//--------------------------------------------------------------
void testApp::update(){
	//////////////////////////////open frameworks lights 
	updateOFLights();

	gm.update();

	///////////////////////////////////////////update games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->update();
	}

	/*

	if(GetTickCount() - last_tick_count < 16) {
		return;
	} else { last_tick_count = GetTickCount(); }

	processFrame();

	std::vector<gwc::PointEvent> point_events = consumePointEvents();

	for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++) {
		//cout << "Touch Point = " << event_it->point_id << endl;
		switch(event_it->status) {
			case gwc::TOUCHADDED:
				assignTouchPoint("touchReceiver", event_it->point_id);
			case gwc::TOUCHUPDATE:
				//If the point is being added, this will place it in our point map; the same line of code will update the point's
				//position if it's already present, so we can use this command to handle new points as well as point updates
				active_points[event_it->point_id] = gwc::Point(event_it->position.getX(),event_it->position.getY());
				break;
			case gwc::TOUCHREMOVED:
				//Remove the point from the map
				active_points.erase(event_it->point_id);
				break;
		}
	}

	std::vector<gwc::GestureEvent> gesture_events = consumeGestureEvents();

	for(std::vector<gwc::GestureEvent>::iterator gesture_it = gesture_events.begin(); gesture_it != gesture_events.end(); gesture_it++) {
		cout << gesture_it->gesture_type << " - " << gesture_it->x << ", " << gesture_it->y << " - " << gesture_it->phase << endl;
	}
	*/
}
//--------------------------------------------------------------
void testApp::draw(){
	//ofEnableDepthTest();
	ofEnableAlphaBlending();
	ofDisableDepthTest();
	SceneManager::Instance()->draw();
	ofEnableDepthTest();
	ofDisableAlphaBlending();
	///////////////////START OF RENDERING////////////////////
	startOFLights();


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  DUMMY GUI ///  DUMMY GUI  ///  DUMMY GUI
	///to keep track of what menus and buttons to show, and when
	for(int i = 0; i < myGames.size(); i++){
		int gStep=0;
		//get current step of game
		gStep = myGames[i]->getCurrentStep();

		if(gStep == -1){
			//standby mode
			//showing attract / inactive banner
			ofDrawBitmapString("Press anything to begin" + ofToString("") +"\n",20, 20);
			//waiting for any touch event to begin
			//to go to step 0
		}
		else if(gStep == 0){
			//waiting for object to be selected from menu //drag behavior
			//show object menu
			ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 20);
			//show select object or drag gui graphic
			ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 80);
		}else if(gStep == 1){
			//an object has been selected
			//show object menu
			ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 20);
			//show select object or drag gui graphic
			ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 80);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 100);
		}else if(gStep ==2){
			//waiting for armature to be selected from menu //drag behavior
			//show armature menu
			ofDrawBitmapString("SELECT AN ARMATURE:" + ofToString("") +"\n" + "armature 1:  "+ofToString(1)+"\n",20, 20);
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 3){
			//armature was selected // show armature...manupulate armature
			ofDrawBitmapString("ARMATURE SELECTED:" + ofToString("") +"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 60); //this makes the puzzle
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 4){
			//puzzle has been created
			ofDrawBitmapString("ROTATE PUZZLE:  mouse DRAG" +ofToString("")+"\n",20, 20);
			//show color palette menu
			ofDrawBitmapString("SELECT a COLOR:" + ofToString("") +"\n" + "color 1:  "+ofToString(1)+"\n",20, 40);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 80); //this enables the puzzle to have face rotations
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 100);
		}else if(gStep == 5){
			//puzzle has been created
			//rotations can happen
			ofDrawBitmapString("ROTATE PUZZLE:  mouse DRAG" +ofToString("")+"\n",20, 20);
			ofDrawBitmapString("X rotations: 'q' 'a'"+ofToString(" ")+"\n",20, 40);
			ofDrawBitmapString("Y rotations: 'w' 's'"+ofToString(" ")+"\n",20, 60);
			ofDrawBitmapString("Z rotations: 'e' 'd'"+ofToString(" ")+"\n",20, 80);
			//undo move
			ofDrawBitmapString("UNDO: press 'u' " + ofToString("") +"\n" ,20, 120);
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 140);
		}else if(gStep == 6){
			//create object by extrusion
			ofDrawBitmapString("sSHOW DRAWING AREA!" +ofToString("")+"\n",20, 20);
			//testing
			ofDrawBitmapString("TEST: press 'e' to extrude: " + ofToString("") +"\n" ,20, 40);
			//show object menu
			ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 60);
			//show select object or drag gui graphic
			ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 100);
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 120);//
		}
	}


	////////////////////////PUZZLE //////////////////////
	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[0]->draw();
	}

	//middle puzzles
	//for(int i=0; i < middlePuzzles.size();i++){
	//	middlePuzzles[i]->draw();
	//}

	///////////////////END OF RENDERING////////////////////
	stopOFLights();

	for(std::map<int,gwc::Point>::iterator points_it = active_points.begin(); points_it != active_points.end(); points_it++)
	{
		float x = points_it->second.getX();
		float y = points_it->second.getY();

		//Generate a circle with a 50-pixel radius at this point's location
		ofFill();
		ofCircle(ofPoint(x, y, 0), 20);
		ofNoFill();
		ofSetLineWidth(2);
		ofCircle(ofPoint(x, y, 0), 30);


		//Generate a stringstream for each value with which we're concerned
		std::stringstream xvals; xvals << (int)x;
		std::stringstream yvals; yvals << (int)y;
		std::stringstream ids; ids << points_it->first;

		//Annotate the circle we just drew with the id, x and y values for the corresponding point
		ofDrawBitmapString("ID: " + ids.str() + "\nX: " + xvals.str() + " Y: " + yvals.str(), x + 40, y - 40, 0);
	}
	//ofDisableDepthTest();
}

//-------------------------------------------------------------- 
void testApp::keyPressed(int key){
	if(currentGame == 1){
		myGames[0]->guiInput(key);
	}
	if(myGames[0]->getCurrentStep() != -1){
		////////////////////////////////////////////////////////
		///////////from puzzles in the center
		if(key == 'p'){
			myGames[0]->loadPuzzle(middlePuzzles[0]->getPuzzle());
		}
		if(key == 'o'){
			myGames[0]->loadPuzzle(middlePuzzles[1]->getPuzzle());
		}
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	/*
	if(button == 2){
		float rotationMultiplier = 5.0;
		ofVec2f dragNow(x,y);
		ofVec2f dragDelta = dragNow - lastRightDragPos;
		//cout << "drag change = " << dragDelta.x << ", " << dragDelta.y << endl;
		SG_VECTOR axis = {((dragDelta.y / ofGetHeight()) * rotationMultiplier) * -1.0,(dragDelta.x / ofGetWidth()) * rotationMultiplier,0};
		myGames[0]->rotateP(axis);
		rotate = false;
		lastRightDragPos = dragNow;
	}
	if(button == 0){
		if(ofGetElapsedTimef() - timeOfLastInput > inputDelayTime){
		    updateMouseState("drag", x, y, button);
			timeOfLastInteraction = ofGetElapsedTimef();
			timeOfLastInput = ofGetElapsedTimef();
		}
		int gStep = myGames[0]->getCurrentStep();
		//cout << gStep << endl;
		if(gStep == 4){
			float rotationMultiplier = 5.0;
			ofVec2f dragNow(x,y);
			ofVec2f dragDelta = dragNow - lastRightDragPos;
			//cout << "drag change = " << dragDelta.x << ", " << dragDelta.y << endl;
			SG_VECTOR axis = {((dragDelta.y / ofGetHeight()) * rotationMultiplier) * -1.0,(dragDelta.x / ofGetWidth()) * rotationMultiplier,0};
			myGames[0]->rotateP(axis);
			rotate = false;
			lastRightDragPos = dragNow;
		}
	}
	*/
	if(button == 0){
		SubObEvent *ev = new SubObEvent();
		ev->setName("update-touch-point");
		ev->addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev->addArg("touch-id", touchId + touchIdOffset);
		//SubObMediator::Instance()->sendEvent("update-touch-point", ev);
	}
	if(button == 2){
		myGames[0]->mouseDragged(x,y,button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(button == 0){
		touchId = nextTouchId;
		nextTouchId = (nextTouchId + 1) % 16;
		SubObEvent *ev = new SubObEvent();
		ev->setName("add-touch-point");
		ev->addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev->addArg("touch-id", touchId + touchIdOffset);
		//SubObMediator::Instance()->sendEvent("add-touch-point", ev);
	}
	if(button == 2){
		myGames[0]->mousePressed(x,y,button);
	}
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	updateMouseState("up", x, y, button);
    timeOfLastInteraction = ofGetElapsedTimef();
	if(button == 0){
		SubObEvent *ev = new SubObEvent();
		ev->setName("remove-touch-point");
		ev->addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev->addArg("touch-id", touchId + touchIdOffset);
		//SubObMediator::Instance()->sendEvent("remove-touch-point", ev);
	}
	myGames[0]->mouseReleased(x,y,button);
}

void testApp::updateMouseState(const char * _state, int _x, int _y, int _button){
    attrs["mouseX"] = ofToString(_x);
    attrs["mouseY"] = ofToString(_y);
    attrs["mouseButton"] = ofToString(_button);
    attrs["mouseState"] = _state;
    //SubObMediator::Instance()->update("mouse-changed",this);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

void testApp::update(string _subName, Subject *_sub){
	/*
	if(_subName == "button"){
		string msg = _sub->getAttr("message");
		cout << "got message - " << msg << endl;
		if(msg == "cut"){
			makeCut = true;
		}
		if(msg == "rotate-right") {//rotate right
			rotatePHright = true;
		}
		if(msg == "rotate-left") {//rotate left
			rotatePHleft = true;
		}
		if(msg == "rotate-up") {//rotate up
			rotatePVup = true;
		}
		if(msg == "rotate-down") {//rotate down
			rotatePVdown = true;
		}
	}
	*/
}

void testApp::update(string _eventName, SubObEvent* _event){
	//cout << "event named - " << _eventName << endl;
	if(_eventName == "object-selected"){
		if(myGames[0]->getCurrentStep() == 0){
			//SubObEvent *ev = new SubObEvent();
			ev->setName("hide-node");
			ev->addArg("target","next-inactive");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("hide-node");
			ev->addArg("target","start-help-bl");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("unhide-node");
			ev->addArg("target","next-active");
			SubObMediator::Instance()->sendEvent("unhide-node", ev);
		}
		if(myGames[0]->getCurrentStep() == 0  || myGames[0]->getCurrentStep() == 1 || myGames[0]->getCurrentStep() == 6){
			int obj = _event->getArg("object")->getInt();
			SG_VECTOR objectPos = {0,0,0};  //where it gets sliced
			myGames[0]->guiLoad(obj);
		}
	}
	if(_eventName == "next-step"){
		myGames[0]->guiNext();
		int currStep = myGames[0]->getCurrentStep();
		cout << "set game to - " << currStep << endl;
		if(currStep == 1){
			ev->setName("hide-node");
			ev->addArg("target","3d-window");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("unhide-node");
			ev->addArg("target","arm-window");
			SubObMediator::Instance()->sendEvent("unhide-node", ev);
		}
		if(currStep == 3){
			ev->setName("hide-node");
			ev->addArg("target","arm-window");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("unhide-node");
			ev->addArg("target","color-window");
			SubObMediator::Instance()->sendEvent("unhide-node", ev);
		}
		if(currStep == 4){
			ev->setName("hide-node");
			ev->addArg("target","color-window");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("hide-node");
			ev->addArg("target","3d-window-box");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("hide-node");
			ev->addArg("target","object-drop");
			SubObMediator::Instance()->sendEvent("hide-node", ev);
			ev->setName("unhide-node");
			ev->addArg("target","puzzle-help-bl");
			SubObMediator::Instance()->sendEvent("unhide-node", ev);
			ev->setName("unhide-node");
			ev->addArg("target","ibox-bl");
			SubObMediator::Instance()->sendEvent("unhide-node", ev);
		}
	}
	if(_eventName == "armature-selected"){
		string armStr = ofToString(_event->getArg("armature")->getInt());
		//myGames[0]->setCurrentStep(3);
		myGames[0]->guiInput(armStr.c_str()[0]);
	}
	if(_eventName == "cut-object"){
		SG_VECTOR v = {displayX,displayY,displayZ};
		myGames[0]->createPuzzle(v);
	}
	if(_eventName == "goto-step5"){
		myGames[0]->setCurrentStep(5);
	}
	if(_eventName == "reset"){
		myGames[0]->guiReset();
		SceneManager::Instance()->reset();
	}
	if(_eventName == "touch-point"){
		cout << "received touch point" << endl;
		_event->setActive(false);
	}
	if(_eventName == "object-area-drag"){
		ofVec2f delta = _event->getArg("deltaPos")->getVec2();

	}
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void testApp::exit(){
	myGames[0]->restart();
	myGames[0]->exit();

	/*myGames[1]->restart();
	myGames[1]->exit();*/

	sgDeleteObject(sgBunny);
	sgDeleteObject(sgTetrahedron);
	sgDeleteObject(sgDodecahedron);
	sgDeleteObject(sgIcosahedron);
	sgDeleteObject(sgOctahedron);
	//sgDeleteObject(sgTeapot);

	myCutter->exit();
	mySlicer->exit();
	//objectDisplayed->exit();
	//delete menu puzzles correctly


	//sgFreeKernel();
}
//------------------------------------------------------------------------------
void testApp::loadOBJfiles(){
	////////////////////////////////////////load heavy models
	bunny.loadModel("bunny.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh = bunny.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> bunnyVert = tempMesh.getVertices();
	//make an array[] from this vector
	SG_POINT *vert = new SG_POINT[bunnyVert.size()];
	for(int i=0;i<bunnyVert.size(); i++){
		vert[i].x = bunnyVert[i].x;
		vert[i].y = bunnyVert[i].y;
		vert[i].z = bunnyVert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType> bunnyIndices = tempMesh.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes = new SG_INDEX_TRIANGLE[bunnyIndices.size()];
	for(int i=0;i<bunnyIndices.size(); i++){
		indexes->ver_indexes[i] = bunnyIndices[i];
	}
	//generate sgC3DObject from geometry information
	sgBunny = sgFileManager::ObjectFromTriangles(vert,bunnyVert.size(),indexes,bunnyIndices.size()/3); 
	delete [] vert;
	delete [] indexes;
	////////////////////////////////////////load heavy models
	tetrahedron.loadModel("tetrahedron.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh1 = tetrahedron.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> tetrahedronVert = tempMesh1.getVertices();
	//make an array[] from this vector
	SG_POINT *vert1 = new SG_POINT[tetrahedronVert.size()];
	for(int i=0;i<tetrahedronVert.size(); i++){
		vert1[i].x = tetrahedronVert[i].x;
		vert1[i].y = tetrahedronVert[i].y;
		vert1[i].z = tetrahedronVert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType>  tetrahedronIndices = tempMesh1.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes1 = new SG_INDEX_TRIANGLE[tetrahedronIndices.size()];
	for(int i=0;i<tetrahedronIndices.size(); i++){
		indexes1->ver_indexes[i] = tetrahedronIndices[i];
	}
	//generate sgC3DObject from geometry information
	sgTetrahedron = sgFileManager::ObjectFromTriangles(vert1,tetrahedronVert.size(),indexes1,tetrahedronIndices.size()/3); 
	delete [] vert1;
	delete [] indexes1;
	////////////////////////////////////////load heavy models
	dodecahedron.loadModel("dodecahedron.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh2 = dodecahedron.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> dodecahedronVert = tempMesh2.getVertices();
	//make an array[] from this vector
	SG_POINT *vert2 = new SG_POINT[dodecahedronVert.size()];
	for(int i=0;i<dodecahedronVert.size(); i++){
		vert2[i].x = dodecahedronVert[i].x;
		vert2[i].y = dodecahedronVert[i].y;
		vert2[i].z = dodecahedronVert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType>  dodecahedronIndices = tempMesh2.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes2 = new SG_INDEX_TRIANGLE[dodecahedronIndices.size()];
	for(int i=0;i<dodecahedronIndices.size(); i++){
		indexes2->ver_indexes[i] = dodecahedronIndices[i];
	}
	//generate sgC3DObject from geometry information
	sgDodecahedron = sgFileManager::ObjectFromTriangles(vert2,dodecahedronVert.size(),indexes2,dodecahedronIndices.size()/3); 
	delete [] vert2;
	delete [] indexes2;
	////////////////////////////////////////load heavy models
	icosahedron.loadModel("icosahedron.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh3 = icosahedron.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> icosahedronVert = tempMesh3.getVertices();
	//make an array[] from this vector
	SG_POINT *vert3 = new SG_POINT[icosahedronVert.size()];
	for(int i=0;i<icosahedronVert.size(); i++){
		vert3[i].x = icosahedronVert[i].x;
		vert3[i].y = icosahedronVert[i].y;
		vert3[i].z = icosahedronVert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType>  icosahedronIndices = tempMesh3.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes3 = new SG_INDEX_TRIANGLE[icosahedronIndices.size()];
	for(int i=0;i<icosahedronIndices.size(); i++){
		indexes3->ver_indexes[i] = icosahedronIndices[i];
	}
	//generate sgC3DObject from geometry information
	sgIcosahedron = sgFileManager::ObjectFromTriangles(vert3,icosahedronVert.size(),indexes3,icosahedronIndices.size()/3); 
	delete [] vert3;
	delete [] indexes3;
	////////////////////////////////////////load heavy models
	octahedron.loadModel("octahedron.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh4 = octahedron.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> octahedronVert = tempMesh4.getVertices();
	//make an array[] from this vector
	SG_POINT *vert4 = new SG_POINT[octahedronVert.size()];
	for(int i=0;i<octahedronVert.size(); i++){
		vert4[i].x = octahedronVert[i].x;
		vert4[i].y = octahedronVert[i].y;
		vert4[i].z = octahedronVert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType>  octahedronIndices = tempMesh4.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes4 = new SG_INDEX_TRIANGLE[octahedronIndices.size()];
	for(int i=0;i<octahedronIndices.size(); i++){
		indexes4->ver_indexes[i] = octahedronIndices[i];
	}
	//generate sgC3DObject from geometry information
	sgOctahedron = sgFileManager::ObjectFromTriangles(vert4,octahedronVert.size(),indexes4,octahedronIndices.size()/3); 
	delete [] vert4;
	delete [] indexes4;
	//////////////////////////////////////////load heavy models
	//pot.loadModel("teapot.obj");
	////need to make it an sgCore3DObject to be able to slice it
	//ofMesh tempMesh5 = pot.getMesh(0);
	////get vertices from mesh
	//vector<ofVec3f> teapot3Vert = tempMesh5.getVertices();
	////make an array[] from this vector
	//SG_POINT *vert5 = new SG_POINT[teapot3Vert.size()];
	//for(int i=0;i<teapot3Vert.size(); i++){
	//	vert5[i].x = teapot3Vert[i].x;
	//	vert5[i].y = teapot3Vert[i].y;
	//	vert5[i].z = teapot3Vert[i].z;
	//}
	////get indices from mesh
	//vector<ofIndexType>  teapot3Indices = tempMesh5.getIndices();
	////make an array[] from this vector
	//SG_INDEX_TRIANGLE *indexes5 = new SG_INDEX_TRIANGLE[teapot3Indices.size()];
	//for(int i=0;i<teapot3Indices.size(); i++){
	//	indexes5->ver_indexes[i] = teapot3Indices[i];
	//}
	////generate sgC3DObject from geometry information
	//sgTeapot = (sgC3DObject *) sgFileManager::ObjectFromTriangles(vert5,teapot3Vert.size(),indexes5,teapot3Vert.size()/3); 
	//delete [] vert5;
	//delete [] indexes5;
}
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////
// OF rendering
void testApp::initOFRender(){
	ofGLRenderer(true);

	ofSetVerticalSync(true);
	ofEnableDepthTest();
	/////background
	//ofBackground(100, 100, 100, 0);//gray
	//ofBackground(30, 144, 255, 0);//blue
	ofBackground(65, 105, 225, 0);//blue
	// turn on smooth lighting //
	bSmoothLighting = true;
	ofSetSmoothLighting(true);

	// lets make a high-res sphere //
	// default is 20 //
	ofSetSphereResolution(128);

	// radius of the sphere //
	radius = 180.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);

	// Point lights emit light in all directions //
	// set the diffuse color, color reflected from the light source //
	pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

	// specular color, the highlight/shininess color //
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
	pointLight.setPointLight();

	spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
	spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	// turn the light into spotLight, emit a cone of light //
	spotLight.setSpotlight();

	// size of the cone of emitted light, angle between light axis and side of cone //
	// angle range between 0 - 90 in degrees //
	spotLight.setSpotlightCutOff( 50 );

	// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	// range 0 - 128, zero is even illumination, 128 is max falloff //
	spotLight.setSpotConcentration( 45 );

	// Directional Lights emit light based on their orientation, regardless of their position //
	directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 0.f));
	directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	directionalLight.setDirectional();

	// set the direction of the light
	// set it pointing from left to right -> //
	directionalLight.setOrientation( ofVec3f(0, 90, 0) );


	bShiny = true;
	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	bPointLight = bSpotLight = bDirLight = true;

	// tex coords for 3D objects in OF are from 0 -> 1, not 0 -> image.width
	// so we must disable the arb rectangle call to allow 0 -> 1
	ofDisableArbTex();
	//// load an image to use as the texture //
	//ofLogoImage.loadImage("of.png");
	//bUseTexture = true;
}
//------------------------------------------------------------------------------
void testApp::startOFLights(){
	// enable lighting //
	ofEnableLighting();
	// enable the material, so that it applies to all 3D objects before material.end() call //
	material.begin();
	// activate the lights //
	if (bPointLight) pointLight.enable();
	//if (bSpotLight) spotLight.enable();
	if (bDirLight) directionalLight.enable();

	// grab the texture reference and bind it //
	// this will apply the texture to all drawing (vertex) calls before unbind() //
	if(bUseTexture) ofLogoImage.getTextureReference().bind();

	//ofSetColor(255, 255, 255, 255);
	/*ofPushMatrix();
	ofTranslate(center.x, center.y, center.z-300);
	ofRotate(ofGetElapsedTimef() * .8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawSphere( 0,0,0, radius);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(300, 300, cos(ofGetElapsedTimef()*1.4) * 300.f);
	ofRotate(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
	ofRotate(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox(0, 0, 0, 60);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(center.x, center.y, -900);
	ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox( 0, 0, 0, 850);
	ofPopMatrix();*/

	//small test of openFrameworks drawing embeded with sgCore geometry 
	//ofPushMatrix();
	//ofTranslate(300,300);
	//ofSetColor(ofColor(255,0,255));
	//ofCircle(ofPoint(0,0),5);
	////ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	//ofPopMatrix();

	if(bUseTexture) ofLogoImage.getTextureReference().unbind();

	if (!bPointLight) pointLight.disable();
	//if (!bSpotLight) spotLight.disable();
	if (!bDirLight) directionalLight.disable();
}
//------------------------------------------------------------------------------
void testApp::updateOFLights(){
	/*pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
	sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
	-cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);*/

	/*spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);*/
}
//------------------------------------------------------------------------------
void testApp::stopOFLights(){
	material.end();
	// turn off lighting //
	ofDisableLighting();

	ofSetColor( pointLight.getDiffuseColor() );
	//if(bPointLight) pointLight.draw();

	//ofSetColor(255, 255, 255);
	//ofSetColor( spotLight.getDiffuseColor() );
	//if(bSpotLight) spotLight.draw();

	//ofSetColor(255, 255, 255);
	/*ofDrawBitmapString("Point Light On (1) : "+ofToString(bPointLight) +"\n"+
	"Spot Light On (2) : "+ofToString(bSpotLight) +"\n"+
	"Directional Light On (3) : "+ofToString(bDirLight)+"\n"+
	"Shiny Objects On (s) : "+ofToString(bShiny)+"\n"+
	"Spot Light Cutoff (up/down) : "+ofToString(spotLight.getSpotlightCutOff(),0)+"\n"+
	"Spot Light Concentration (right/left) : " + ofToString(spotLight.getSpotConcentration(),0)+"\n"+
	"Smooth Lighting enabled (x) : "+ofToString(bSmoothLighting,0)+"\n"+
	"Textured (t) : "+ofToString(bUseTexture,0),
	20, 20);*/
}
