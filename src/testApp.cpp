#include "testApp.h"
#include "game.h"
///////////////////////////////////////////

#define displayXBlue 550
#define displayYBlue 1150
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

	//gm = new GestureManager();
	gm.loadGMLFile("basic_manipulation.gml");
	gm.init("rubiksWindow", ofGetWidth(), ofGetHeight());
	//gm->start();

	string guiFile = "main.xml";
	GuiConfigurator::Instance()->addFile(guiFile);
	GuiConfigurator::Instance()->loadGui(false, false);
	
	map<string, string> patterns;

	gameIds["bl"] = 0;
	gameIds["br"] = 1;
	gameIds["tl"] = 2;
	gameIds["tr"] = 3;

	gameTags[0] = "bl";
	gameTags[1] = "br";
	gameTags[2] = "tl";
	gameTags[3] = "tr";

	patterns["%COLOR%"] = "Blue";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", false, false, "bl:", patterns);
	patterns["%COLOR%"] = "Red";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", true, false, "br:", patterns);
	patterns["%COLOR%"] = "Orange";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", false, true, "tl:", patterns);
	patterns["%COLOR%"] = "Green";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", true, true, "tr:", patterns);

	GuiConfigurator::Instance()->loadText("assets.xml");

	/*SubObMediator::Instance()->addObserver("object-selected", this);
	SubObMediator::Instance()->addObserver("armature-selected", this);
	SubObMediator::Instance()->addObserver("next-step", this);
	SubObMediator::Instance()->addObserver("reset", this);
	SubObMediator::Instance()->addObserver("touch-point", this);
	SubObMediator::Instance()->addObserver("gesture", this);
	SubObMediator::Instance()->addObserver("extrude", this);
	SubObMediator::Instance()->addObserver("extrusion-success", this);*/

	SubObMediator::Instance()->addObserver("menupuzzle-selected", this);

	/////////////////////////////initialize sgCore library
	sgInitKernel();

	/////////////////////////////initialize openFrameworks rendering
	initOFRender();

	/////////////////////////////load obj files into sgCore objects
	cout << "loading obj files " << endl;
	loadOBJfiles();
	////////////////////////////create middle objects (puzzles with no twisting == normal objects with faces colores
	//this objects are rendering of the sgCore obects just created.
	//there are 7 objects to be created
	ofToggleFullscreen();

	cout << "creating puzzle menu items" << endl;
	middlePuzzlePos.x = 0;
	middlePuzzlePos.y = (ofGetWindowHeight()/2)-90;
	middlePuzzlePos.z = 0;
	//////////////////////////////////create cutter
	//ofVec3f offsetSlicer = ofVec3f(0,0,0);
	//myCutter = new cutter(0.01,1000,100,1,offsetSlicer,3);// number of slices == same number in slicer
	//myCutter->setup();
	////////////////////////////////////create slicer
	//mySlicer = new slicer(myCutter,3);
	//mySlicer->setup();

	slicingPos.x = 0;
	slicingPos.y = 0;
	slicingPos.z = 0;

	//ofVec3f rotateSlicer = ofVec3f (0,0,0);

	for(int i=0; i < puzzleItems; i++){
		middlePuzzlePos.x = 10 + (i * 10) + (i*180);
		/*
		if(i > 6){
			middlePuzzlePos.x = middlePuzzlePos.x - 60  + ((i-5)*10);
		}
		*/
		puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, i);
		if(i==0){
			puzzleDisplayed->loadObject(sgCreateTorus(100,70,50,50),1);
		}else if(i == 1){
			puzzleDisplayed->loadObject(sgCreateBox(300,300,300),2);
		}else if(i == 2){
			puzzleDisplayed->loadObject((sgC3DObject *)sgTetrahedron->Clone(),3);
		}else if(i == 3){
			puzzleDisplayed->loadObject((sgC3DObject *)sgBunny->Clone(),4);
		}else if(i == 4){
			puzzleDisplayed->loadObject((sgC3DObject *)sgDodecahedron->Clone(),5);
		}else if(i == 5){
			puzzleDisplayed->loadObject((sgC3DObject *)sgIcosahedron->Clone(),6);
		}else if(i == 6){
			puzzleDisplayed->loadObject((sgC3DObject *)sgOctahedron->Clone(),7);
		}else if(i == 7){
			//puzzleDisplayed->loadObject((sgC3DObject *)sgOctahedron->Clone(),8);
			puzzleDisplayed->loadObject((sgC3DObject *)sgBunny->Clone(),4);
		}else{
			//slots for other user created puzzles
			puzzleDisplayed->loadObject(sgCreateBox(300,300,300),2);
		}
		cout << "created puzzle menu object: " << i <<endl;
		puzzleDisplayed->setup();
		puzzleDisplayed->update();
		puzzleDisplayed->colorFacesMenu();
		puzzleDisplayed->init();

		//mySlicer->intersectCubes((sgCObject*)puzzleDisplayed->getObject());
		//myPuzzle = new puzzle(middlePuzzlePos, offsetSlicer,3); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
		//myPuzzle->setup();
		//myPuzzle->loadPieces(mySlicer->getPieces(),puzzleDisplayed->objectId,rotateSlicer);//selected object id is used for coloring

		////this is a special function that takes, the unique normals, and unique colors of the menuPuzzle, to repeat those colors on the real puzzle of the menuPuzzle
		//myPuzzle->colorFacesMenuPuzzle(puzzleDisplayed->objectId,puzzleDisplayed->uniqueNormals,puzzleDisplayed->colorsVMenu);


		//cout << "created puzzle menu puzzle: " << i <<endl;
		//puzzleDisplayed->loadPuzzle(myPuzzle);

		//cout << "created puzzle menu item: " << i << endl;
		//
		middlePuzzles.push_back(puzzleDisplayed);
	}
	cout << "puzzles menu created" << endl;
	puzzleCounter = 0;

	///////////////////////////////////////////////////create games
	//////////////////////////////one game for now
	SG_VECTOR gamePos = {0,0,0}; //one game created at the origin // this will have to change for a game creating function when more stations are anabled
	//SG_VECTOR displayPos = {ofGetWidth() / 2, ofGetHeight() / 2,displayZ};
	SG_VECTOR displayPos = {displayXBlue,displayYBlue,displayZ};
	ofRectangle tr;
	tr.x = 300;
	tr.y = 700;
	tr.width = 660;
	tr.height = 320;
	game *tempGame = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime);
	myGames.push_back(tempGame);
	tr.x = 960;
	game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime);
	myGames.push_back(tempGame2);
	tr.x = 300;
	tr.y = 60;
	game *tempGame3 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime);
	myGames.push_back(tempGame3);
	tr.x = 960;
	game *tempGame4 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime);
	myGames.push_back(tempGame4);
	currentGame = 1;
	//create a second game
	//game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos,iddleTime);
	//myGames.push_back(tempGame2);
	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->setup(sgBunny->Clone(),sgTetrahedron->Clone(),sgDodecahedron->Clone(),sgIcosahedron->Clone(),sgOctahedron->Clone(), gameTags[i]);//,sgTeapot->Clone());
	}

	//////setup GUI/////////////
	//ofSetFullscreen(true);

	//active_points = std::map<int,gwc::Point>();
	//rotate = true;

	touchId = nextTouchId = 0;
	touchIdOffset = 1000;

	//ofSetupScreenOrtho(ofGetWidth(), ofGetHeight());
	//ofSetupScreenPerspective(ofGetWidth(), ofGetHeight(), 0.0);
	//ofEnableAntiAliasing();
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
	
	/////////////////////////////////////////update middle puzzles
	for(int i=0; i < middlePuzzles.size();i++){
		middlePuzzles[i]->update();
	}

	/////////////////////////////////////////watch for new puzzles being saved
	if(myGames[0]->savePuzzleB == true){
		middlePuzzlePos.x = 100 + ((puzzleCounter+7)*180);
		menuPuzzle * tempMidPuzzle = myGames[0]->savePuzzle(slicingPos,middlePuzzlePos);
		middlePuzzles[puzzleCounter+7] = tempMidPuzzle;
		puzzleCounter ++;
		if(puzzleCounter == 3){
			puzzleCounter=0;
		}
		myGames[0]->savePuzzleB = false;
		//reset game
		myGames[0]->restart();
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
	SceneManager::Instance()->draw("back");
	ofEnableDepthTest();
	ofDisableAlphaBlending();
	///////////////////START OF RENDERING////////////////////
	//startOFLights();

	ofEnableAntiAliasing();
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  DUMMY GUI ///  DUMMY GUI  ///  DUMMY GUI
	///to keep track of what menus and buttons to show, and when
	//for(int i = 0; i < myGames.size(); i++){
	//	int gStep=0;
	//	//get current step of game
	//	gStep = myGames[i]->getCurrentStep();
	//	
	//	if(gStep == -1){
	//		//standby mode
	//		//showing attract / inactive banner
	//		ofDrawBitmapString("Press anything to begin" + ofToString("") +"\n",20, 20);
	//		//waiting for any touch event to begin
	//		//to go to step 0
	//	}
	//	else if(gStep == 0){
	//		//waiting for object to be selected from menu //drag behavior
	//		//show object menu
	//		ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 20);
	//		//show select object or drag gui graphic
	//		ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 80);
	//	}else if(gStep == 1){
	//		//an object has been selected
	//		//show object menu
	//		ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 20);
	//		//show select object or drag gui graphic
	//		ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 80);
	//		//show next button.
	//		ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 100);
	//	}else if(gStep ==2){
	//		//waiting for armature to be selected from menu //drag behavior
	//		//show armature menu
	//		ofDrawBitmapString("SELECT AN ARMATURE:" + ofToString("") +"\n" + "armature 1:  "+ofToString(1)+"\n",20, 20);
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
	//	}else if(gStep == 3){
	//		//armature was selected // show armature...manupulate armature
	//		ofDrawBitmapString("ARMATURE SELECTED:" + ofToString("") +"\n",20, 20);
	//		//show next button.
	//		ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 60); //this makes the puzzle
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
	//	}else if(gStep == 4){
	//		//puzzle has been created
	//		ofDrawBitmapString("ROTATE PUZZLE:  mouse DRAG" +ofToString("")+"\n",20, 20);
	//		//show color palette menu
	//		ofDrawBitmapString("SELECT a COLOR:" + ofToString("") +"\n" + "color 1:  "+ofToString(1)+"\n",20, 40);
	//		//show next button.
	//		ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 80); //this enables the puzzle to have face rotations
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 100);
	//	}else if(gStep == 5){
	//		//puzzle has been created
	//		//rotations can happen
	//		ofDrawBitmapString("ROTATE PUZZLE:  mouse DRAG" +ofToString("")+"\n",20, 20);
	//		ofDrawBitmapString("X rotations: 'q' 'a'"+ofToString(" ")+"\n",20, 40);
	//		ofDrawBitmapString("Y rotations: 'w' 's'"+ofToString(" ")+"\n",20, 60);
	//		ofDrawBitmapString("Z rotations: 'e' 'd'"+ofToString(" ")+"\n",20, 80);
	//		//undo move
	//		ofDrawBitmapString("UNDO: press 'u' " + ofToString("") +"\n" ,20, 120);
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 140);
	//		//show save button.
	//		ofDrawBitmapString("SAVE: press 's' " + ofToString("") +"\n" ,20, 160);
	//	}else if(gStep == 6){
	//		//create object by extrusion
	//		ofDrawBitmapString("sSHOW DRAWING AREA!" +ofToString("")+"\n",20, 20);
	//		//testing

	//		ofDrawBitmapString("TEST: press 'e' to extrude: " + ofToString("") +"\n" ,20, 40);
	//		//show object menu
	//		ofDrawBitmapString("SELECT AN OBJECT from MENU:" + ofToString("") +"\n" + "1 - 7: objects"+ofToString("")+"\n"+ "9 : extrusion"+ofToString("")+"\n",20, 60);
	//		//show select object or drag gui graphic
	//		ofDrawBitmapString("or drag a shape from above:" + ofToString("") +"\n",20, 100);
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 120);//
	//	}else if(gStep == 7){
	//		//loaded a puzzle form the middle
	//		//rotations can happen
	//		ofDrawBitmapString("ROTATE PUZZLE:  mouse DRAG" +ofToString("")+"\n",20, 20);
	//		ofDrawBitmapString("X rotations: 'q' 'a'"+ofToString(" ")+"\n",20, 40);
	//		ofDrawBitmapString("Y rotations: 'w' 's'"+ofToString(" ")+"\n",20, 60);
	//		ofDrawBitmapString("Z rotations: 'e' 'd'"+ofToString(" ")+"\n",20, 80);
	//		//undo move
	//		ofDrawBitmapString("UNDO: press 'u' " + ofToString("") +"\n" ,20, 120);
	//		//show restart button.
	//		ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 140);
	//		//show save button.
	//		ofDrawBitmapString("make one: press 'n' " + ofToString("") +"\n" ,20, 160);
	//	}
	//}


	////////////////////////PUZZLE //////////////////////
	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->draw();
	}

	//middle puzzles
	for(int i=0; i < middlePuzzles.size();i++){
		middlePuzzles[i]->draw();
	}

	///////////////////END OF RENDERING////////////////////
	//stopOFLights();
	//ofDisableDepthTest();

	ofEnableAlphaBlending();
	ofDisableDepthTest();
	SceneManager::Instance()->draw("front");
	ofEnableDepthTest();
	ofDisableAlphaBlending();
}

//-------------------------------------------------------------- 
void testApp::keyPressed(int key){
	if(currentGame == 1){
		myGames[0]->guiInput(key);
	}

	////////////////////////////////////////////////////////////////////////
	/////////////////////////////loading puzzles from the middle
	////////////////////////////////////////////////////////////////////////
	if(myGames[0]->getCurrentStep() != -1){
		////////////////////////////////////////////////////////
		///////////from puzzles in the center
		//////instead of asking for a "key", with the GUI it should ask for the object ID
		if(key == 'p'){
			//myGames[0]->loadPuzzle(middlePuzzles[3]->getPuzzle());
			myGames[0]->setCurrentStep(7);
		}
		if(key == 'o'){
			//myGames[0]->loadPuzzle(middlePuzzles[8]->getPuzzle());
			myGames[0]->setCurrentStep(7);
		}
		if(key == 'i'){
			//myGames[0]->loadPuzzle(middlePuzzles[7]->getPuzzle());
			myGames[0]->setCurrentStep(7);
		}
		if(key == 'u'){
			//myGames[0]->loadPuzzle(middlePuzzles[1]->getPuzzle());
			myGames[0]->setCurrentStep(7);
		}
		if(key == 'y'){
			//myGames[0]->loadPuzzle(middlePuzzles[0]->getPuzzle());
			myGames[0]->setCurrentStep(7);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(button == 0){
		SubObEvent ev;
		ev.setName("update-touch-point");
		ev.addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev.addArg("touch-id", touchId + touchIdOffset);
		SubObMediator::Instance()->sendEvent("update-touch-point", ev);
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
		SubObEvent ev("add-touch-point");
		//ev.setName("add-touch-point");
		ev.addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev.addArg("touch-id", touchId + touchIdOffset);
		SubObMediator::Instance()->sendEvent("add-touch-point", ev);
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
		SubObEvent ev;
		ev.setName("remove-touch-point");
		ev.addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev.addArg("touch-id", touchId + touchIdOffset);
		SubObMediator::Instance()->sendEvent("remove-touch-point", ev);
	}
	//myGames[0]->mouseReleased(x,y,button);
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
}

void testApp::update(string _eventName, SubObEvent _event){
	if(_eventName == "menupuzzle-selected"){
		int mpId = _event.getArg("puzzle-id")->getInt();
		string gameTag = _event.getArg("game-tag")->getString();
		////////myGames[gameIds[gameTag]]->loadPuzzle(middlePuzzles[mpId]->getPuzzle());
		//myGames[gameIds[gameTag]]->setCurrentStep(7);
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
	//myGames[0]->restart();
	//myGames[0]->exit();

	/*myGames[1]->restart();
	myGames[1]->exit();*/

	sgDeleteObject(sgBunny);
	sgDeleteObject(sgTetrahedron);
	sgDeleteObject(sgDodecahedron);
	sgDeleteObject(sgIcosahedron);
	sgDeleteObject(sgOctahedron);
	//sgDeleteObject(sgTeapot);

	//myCutter->exit();
	//mySlicer->exit();
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
	tetrahedron.loadModel("tetrahedronNew.obj");
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
	//sphere.loadModel("sphere300_20.obj");
	////need to make it an sgCore3DObject to be able to slice it
	//ofMesh tempMesh5 = sphere.getMesh(0);
	////get vertices from mesh
	//vector<ofVec3f> sphereVert = tempMesh5.getVertices();
	////make an array[] from this vector
	//SG_POINT *vert5 = new SG_POINT[sphereVert.size()];
	//for(int i=0;i<sphereVert.size(); i++){
	//	vert5[i].x = sphereVert[i].x;
	//	vert5[i].y = sphereVert[i].y;
	//	vert5[i].z = sphereVert[i].z;
	//}
	////get indices from mesh
	//vector<ofIndexType>  sphereIndices = tempMesh5.getIndices();
	////make an array[] from this vector
	//SG_INDEX_TRIANGLE *indexes5 = new SG_INDEX_TRIANGLE[sphereIndices.size()];
	//for(int i=0;i<sphereIndices.size(); i++){
	//	indexes5->ver_indexes[i] = sphereIndices[i];
	//}
	////generate sgC3DObject from geometry information
	//sgSphere = sgFileManager::ObjectFromTriangles(vert5,sphereVert.size(),indexes5,sphereIndices.size()/3); 
	//delete [] vert5;
	//delete [] indexes5;
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
	//ofBackground(65, 105, 225, 0);//blue
	// turn on smooth lighting //
	bSmoothLighting = true;
	ofSetSmoothLighting(true);

	// lets make a high-res sphere //
	// default is 20 //
	//ofSetSphereResolution(128);

	// radius of the sphere //
	//radius = 180.f;
	//center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);

	// Point lights emit light in all directions //
	// set the diffuse color, color reflected from the light source //
	pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

	// specular color, the highlight/shininess color //
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
	pointLight.setPointLight();

	spotLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
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
	material.setSpecularColor(ofColor(255, 255, 255, 1));

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
	//if(bUseTexture) ofLogoImage.getTextureReference().bind();

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

	//if(bUseTexture) ofLogoImage.getTextureReference().unbind();

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
