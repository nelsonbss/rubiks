#include "testApp.h"
#include <vector>
#include "game.h"
///////////////////////////////////////////
#include "sgCore.h"

///////////////////////////////////////////
#include <math.h>
#define _USE_MATH_DEFINES

#define displayX 480
#define displayY 270
#define displayZ 100

//std::map<int,gwc::Point> active_points;

//--------------------------------------------------------------
void testApp::setup(){
	/////////////////////////////initialize sgCore library
	sgInitKernel();

	/////////////////////////////initialize openFrameworks rendering
	initOFRender();

	////////////////////////////////create games
	////////////////////////////////one game for now
	SG_VECTOR gamePos = {0,0,0}; //one game created at the origin // this will have to change for a game creating function when more stations are anabled
	SG_VECTOR displayPos = {displayX,displayY,displayZ};
	game *tempGame = new game(gamePos, 1024, 768,displayPos);
	myGames.push_back(tempGame);

	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->setup();
	}
	//////setup GUI/////////////
	//ofSetFullscreen(true);

	string guiFile = "sheets.xml";
	GuiConfigurator::Instance()->addFile(guiFile);
	GuiConfigurator::Instance()->getTags();
	GuiConfigurator::Instance()->makeGUI();

	SubObMediator::Instance()->addObserver("button", this);
	SubObMediator::Instance()->addObserver("object-selected", this);
	SubObMediator::Instance()->addObserver("armature-selected", this);
	SubObMediator::Instance()->addObserver("cut-object", this);
	SubObMediator::Instance()->addObserver("reset", this);

	last_tick_count = GetTickCount();

	/*
	active_points = std::map<int,gwc::Point>();

	if(loadGestureWorks("GestureworksCore32.dll")) { 
		std::cout << "Error loading gestureworks dll" << std::endl; 
		exit(); 
	}
	
	if( !registerWindowForTouchByName("Hello Multitouch!") ) { 
		std::cout << "Could not register target window for touch." << std::endl; 
		exit(); 
	}

	initializeGestureWorks(1920,1080);

	if(!registerWindowForTouchByName("rubiksWindow")) { 
		std::cout << "Could not register target window for touch." << std::endl; 
		exit(); 
	}
	*/

	rotate = true;
}
//--------------------------------------------------------------
void testApp::update(){
	//////////////////////////////open frameworks lights 
	updateOFLights();

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
		//cout << event_it->status << endl;
		switch(event_it->status) {
			case gwc::TOUCHADDED:
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

	//cout << "have " << gesture_events.size() << " gesture events" << endl;

	for(std::vector<gwc::GestureEvent>::iterator gesture_it = gesture_events.begin(); gesture_it != gesture_events.end(); gesture_it++) {
		cout << gesture_it->gesture_type << endl;
	}

	*/
}
//--------------------------------------------------------------
void testApp::draw(){
	///////////////////START OF RENDERING////////////////////
	startOFLights();


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  DUMMY GUI ///  DUMMY GUI  ///  DUMMY GUI
	///to keep track of what menus and buttons to show, and when
	for(int i = 0; i < myGames.size(); i++){
		int gStep=0;
		//get current step of game
		gStep = myGames[i]->getCurrentStep();
		if(gStep == 0){
			//waiting for object to be selected from menu //drag behavior
			//show object menu
			ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
		}else if(gStep == 1){
			//an object has been selected
			ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 80);
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
			//show color palette menu
			ofDrawBitmapString("SELECT a COLOR:" + ofToString("") +"\n" + "color 1:  "+ofToString(1)+"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 60); //this enables the puzzle to have face rotations
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 5){
			//puzzle has been created
			//rotations can happen

			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}
	}


	////////////////////////PUZZLE //////////////////////
	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->draw();
	}

	///////////////////END OF RENDERING////////////////////
	stopOFLights();

	ofEnableDepthTest();
	SceneManager::Instance()->draw();

	/*
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
	*/
	ofDisableDepthTest();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	//right now we only have one game, ideal: the master gui will tell a game wich object to load on drag and drop gestures

	int gStep=0;
	//verify in which step the game is, so that "input" action works well
	gStep = myGames[0]->getCurrentStep();

	////////////////////////////////////////////step 0 inputs
	if(gStep == 0){
		SG_VECTOR objectPos = {0,0,0};  //where it gets sliced
		SG_VECTOR tempPos = {displayX,displayY,displayZ}; // where the temp object will be showed to user
		//waiting for shape to be selected
		if(key == '1') {
			//load object recieves (object id, boolean position, display position) 
			myGames[0]->loadObject(1,objectPos,tempPos); //pos.z its the torus radious
		}
		if(key == '2') {
			myGames[0]->loadObject(2,objectPos,tempPos);
		}
		if(key == '3') {
			myGames[0]->loadObject(3,objectPos,tempPos);
		}
		if(key == '4') {
			myGames[0]->loadObject(4,objectPos,tempPos);
		}
		if(key == '5') {
			myGames[0]->loadObject(5,objectPos,tempPos);
		}
		if(key == '6') {
			myGames[0]->loadObject(6,objectPos,tempPos);
		}
		if(key == '7') { 
			myGames[0]->loadObject(7,objectPos,tempPos);
		}
		if(key == '8') { 
			myGames[0]->loadObject(8,objectPos,tempPos);
		}
	}
	if(key == 's'){
		ofToggleFullscreen();
	}
	////////////////////////////////////////////step 1 inputs
	if(gStep == 1){
		//is showing object with flat color
		//selected an object
		if(key == 'n') {
			//go to step 2
			myGames[0]->setCurrentStep(2);
			//show armature
		} 
		//user can change the selected object
		SG_VECTOR objectPos = {0,0,0}; 
		SG_VECTOR tempPos = {displayX,displayY,displayZ};
		//waiting for shape to be selected
		if(key == '1') {
			//load object recieves (object id, boolean position, display position) 
			myGames[0]->loadObject(1,objectPos,tempPos); //pos.z its the torus radious
		}
		if(key == '2') {
			myGames[0]->loadObject(2,objectPos,tempPos);
		}
		if(key == '3') {
			myGames[0]->loadObject(3,objectPos,tempPos);
		}
		if(key == '4') {
			myGames[0]->loadObject(4,objectPos,tempPos);
		}
		if(key == '5') {
			myGames[0]->loadObject(5,objectPos,tempPos);
		}
		if(key == '6') {
			myGames[0]->loadObject(6,objectPos,tempPos);
		}
		if(key == '7') {
			myGames[0]->loadObject(7,objectPos,tempPos);
		}
		if(key == '8') { 
			myGames[0]->loadObject(8,objectPos,tempPos);
		}
	}
	////////////////////////////////////////////step 2 inputs
	if(gStep == 2){
		//waiting for armature to be selected
		if(key == '1') {
			//select armature 1
			//myGames[0]->loadArmature(1);
			//for now .. go to step 3
			myGames[0]->setCurrentStep(3);
		}
		if(key == '2') {
			//select armature 2
			//for now .. go to step 3
			myGames[0]->setCurrentStep(3);
		}
	}
	////////////////////////////////////////////step 3 inputs
	if(gStep == 3){
		//armature was selected
		//showing armature
		//////////////////////////////move all armature
		if(key == 'l') {
			ofVec3f p = ofVec3f (5,0,0);
			myGames[0]->moveA(p);
		}
		if(key == 'j') {
			ofVec3f p = ofVec3f (-5,0,0);
			myGames[0]->moveA(p);
		}
		if(key == 'i') {
			ofVec3f p = ofVec3f (0,-5,0);
			myGames[0]->moveA(p);
		}
		if(key == 'k') {
			ofVec3f p = ofVec3f (0,5,0);
			myGames[0]->moveA(p);
		}
		//a puzzle can be made
		if(key == 'n') {
			//do slicing
			SG_VECTOR v = {displayX,displayY,displayZ};
			myGames[0]->createPuzzle(v);//create Puzzle goes to step4 inside the game to show the puzzle
		}
	}
	////////////////////////////////////////////step 4 inputs
	if(gStep == 4){
		//showing puzzle with colors
		//waiting for color change
		if(key == '1') {
			//call color change funtion
			ofFloatColor sc = pickColorMouse();
			ofFloatColor menuColor = ofFloatColor (1, 0, 1); //this color comes from the GUI
			myGames[0]->changeColorToColor(sc,menuColor);
		}
		//pressed NEXT
		if(key == 'n') {
			//go to step 5
			myGames[0]->setCurrentStep(5);
		}
	}
	////////////////////////////////////////////step 5 inputs
	if(gStep == 5){
		//selected color (or not)
		//pressed next on color palette step
		//showing puzzle
		//now the puzzle can be played with
		if(key == 'u'){
			//undo last move 
			myGames[0]->unDo();
		}
		///////////////////// FACE ROTATIONS //////////////////////////////
		////////  x axis  ////  x axis
		int idcubie = 11;//to follow this cubie for now //this will be decided upon touch, or click on top of puzzle
		int randcubie=0;
		if(key == 'q') {
			if(rotate == true) {//c
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {1,0,0};
				myGames[0]->rotateByIDandAxis(randcubie,axis,true);
				rotate = false;
			}
		}
		if(key == 'a') {
			if(rotate == true) {//cc
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {1,0,0};
				myGames[0]->rotateByIDandAxis(randcubie,axis,false);
				rotate = false;
			}
		}
		////////  y axis  ////  y axis
		if(key == 'w') {
			if(rotate == true) {
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {0,1,0};
				myGames[0]->rotateByIDandAxis(randcubie,axis,true);
				rotate = false;
			}
		}if(key == 's') {
			//counter clockwise
			if(rotate == true) {
				randcubie = 11;//rand()%26;
				SG_VECTOR axis = {0,1,0};
				myGames[0]->rotateByIDandAxis(randcubie,axis,false);
				rotate = false;
			}
		}
		////////  z axis  ////  z axis
		if(key == 'e') {
			if(rotate == true) {
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {0,0,1};
				myGames[0]->rotateByIDandAxis(randcubie,axis,true);
				rotate = false;
			}
		}if(key == 'd') {
			if(rotate == true) {
				//counter clockwise
				randcubie = 11;//rand()%26;
				SG_VECTOR axis = {0,0,1};
				myGames[0]->rotateByIDandAxis(randcubie,axis,false);
				rotate = false;
			}
		}
		//////////////////////////////move all puzzle
		if(key == 'l') {
			SG_VECTOR p = {10,0,0};
			myGames[0]->moveP(p);
		}
		if(key == 'j') {
			SG_VECTOR p = {-10,0,0};
			myGames[0]->moveP(p);
		}
		if(key == 'i') {
			SG_VECTOR p = {0,-10,0};
			myGames[0]->moveP(p);
		}
		if(key == 'k') {
			SG_VECTOR p = {0,10,0};
			myGames[0]->moveP(p);
		}
		/////////////////////////////rotate all puzzle  // two finger swipe gesture
		if(key == 'm') {//rotate right
			SG_VECTOR r = {0,0.1,0};
			myGames[0]->rotateP(r);
		}
		if(key == 'n') {//rotate left
			SG_VECTOR r = {0,-0.1,0};
			myGames[0]->rotateP(r);
		}
		if(key == 'y') {//rotate up
			SG_VECTOR r = {0.1,0,0};
			myGames[0]->rotateP(r);
		}
		if(key == 'h') {//rotate down
			SG_VECTOR r = {-0.1,0,0};
			ofVec3f v;
			myGames[0]->rotateP(r);
		}
	}

	/////////////////////////////////////////////////////////////////
	////////////////RESART button can work on any step, its not showed on step 1 (object selected)
	if(key == 'r'){
		//tell a game to restart 
		myGames[0]->restart();
	}
	//if(puzzleExists == true){
	//	if(key == 'f') {
	//		cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
	//	}
	//	if(key == 'g') {
	//		cout << "nu pieces " << mySlicer->countPieces() << endl;
	//	}
	//}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	/////////////////////////////move all puzzle
	if(key == 'l') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->moveP(p);
	}
	if(key == 'j') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->moveP(p);
	}
	if(key == 'i') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->moveP(p);
	}
	if(key == 'k') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->moveP(p);
	}
	/////////////////////////////rotate all puzzle
	if(key == 'm') {//rotate 
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotateP(r);
	}
	if(key == 'n') {//rotate left
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotateP(r);
	}
	if(key == 'y') {//rotate up
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotateP(r);
	}
	if(key == 'h') {//rotate down
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotateP(r);
	}
	/////////////////////face rotations//no rotations comming in from gui
	//send: SG_VECTOR axis = {0,0,0};
	if(key == 'q') {
		if(rotate == false) {//c
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,true);
			rotate = true;
		}
	}
	if(key == 'a') {
		if(rotate == false) {//cc
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,false);
			rotate = true;
		}
	}
	//y axis
	if(key == 'w') {
		if(rotate == false) {
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,true);
			rotate = true;
		}
	}if(key == 's') {
		//counter clockwise
		if(rotate == false) {
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,false);
			rotate = true;
		}
	}
	//z axis
	if(key == 'e') {
		if(rotate == false) {
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,true);
			rotate = true;
		}
	}if(key == 'd') {
		if(rotate == false) {
			SG_VECTOR axis = {0,0,0};
			myGames[0]->rotateByIDandAxis(0,axis,false);
			rotate = true;
		}
	}
}
//--------------------------------------------------------------
ofFloatColor testApp::pickColorMouse(){
	//temporary way of picking a color to change
	ofFloatColor tc = ofFloatColor(1,1,0);
	//ofImage pix;
	//ofImage image;
	//image.grabScreen(mouseX,mouseY,1,1);
	//PixelType *p;
	//ofPixels *pixels;
	//pixels = image.getPixels();
	////pix = image.getPixels();
	//ofFloatColor c = pix.getColor(mouseX, mouseY);


	return tc;
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
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
		cout << gStep << endl;
		if(gStep == 4){
			float rotationMultiplier = 5.0;
			ofVec2f dragNow(x,y);
			ofVec2f dragDelta = dragNow - lastRightDragPos;
			//cout << "drag change = " << dragDelta.x << ", " << dragDelta.y << endl;
			SG_VECTOR axis = {(dragDelta.y / ofGetHeight()) * rotationMultiplier,0, (dragDelta.x / ofGetWidth()) * rotationMultiplier};
			myGames[0]->rotateP(axis);
			rotate = false;
			lastRightDragPos = dragNow;
		}
	}
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	cout << button << endl;
	if(button == 2){
		lastRightDragPos.set(x,y);
	}
	if(button == 0){
		if(ofGetElapsedTimef() - timeOfLastInput > inputDelayTime){
		    updateMouseState("down", x, y, button);
			timeOfLastInteraction = ofGetElapsedTimef();
			timeOfLastInput = ofGetElapsedTimef();
		}
	}
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	updateMouseState("up", x, y, button);
    timeOfLastInteraction = ofGetElapsedTimef();
}

void testApp::updateMouseState(const char * _state, int _x, int _y, int _button){
    attrs["mouseX"] = ofToString(_x);
    attrs["mouseY"] = ofToString(_y);
    attrs["mouseButton"] = ofToString(_button);
    attrs["mouseState"] = _state;
    SubObMediator::Instance()->update("mouse-changed",this);
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
	cout << "event named - " << _eventName << endl;
	if(_eventName == "object-selected"){
		int obj = _event->getArg("object")->getInt();
		SG_VECTOR objectPos = {0,0,0};  //where it gets sliced
		SG_VECTOR tempPos = {displayX,displayY,displayZ}; // where the temp object will be showed to user
		cout << "dropping object - " << obj << endl;
		myGames[0]->loadObject(obj,objectPos,tempPos);
	}
	if(_eventName == "armature-selected"){
		myGames[0]->setCurrentStep(3);
	}
	if(_eventName == "cut-object"){
		SG_VECTOR v = {displayX,displayY,displayZ};
		myGames[0]->createPuzzle(v);
	}
	if(_eventName == "reset"){
		myGames[0]->restart();
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
	//cleanup games vector!!
	if(myGames[0]->getCurrentStep() != 0){
		myGames[0]->objectDisplayed->exit();
	}
	//sgFreeKernel();
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
