#include "testApp.h"
#include "game.h"
#include "ofxAssimpModelLoader.h"
#include "hiddenButton.h"
///////////////////////////////////////////

#define displayXBlue 550
#define displayYBlue 1150
#define displayZ -800
#define iddleTime 120
#define puzzleItems 10
#define USE_MOUSE 1

std::map<int,gwc::Point> active_points;

testApp::testApp() : puzzleDisplayed(){
}
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
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", false, false, "bl", patterns);
	patterns["%COLOR%"] = "Red";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", true, false, "br", patterns);
	patterns["%COLOR%"] = "Green";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", false, true, "tl", patterns);
	patterns["%COLOR%"] = "Orange";
	GuiConfigurator::Instance()->extendGui("main", "sheets.xml", true, true, "tr", patterns);

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
	//cout << "loading obj files " << endl;
	//loadOBJfiles();
	////////////////////////////create middle objects (puzzles with no twisting == normal objects with faces colores
	//this objects are rendering of the sgCore obects just created.
	//there are 7 objects to be created
	if(USE_MOUSE != 0){
		ofToggleFullscreen();
	}


	//light.setPointLight();
	//light.setSpotlight(60.0);
	//light.setDirectional();
	//light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	//light.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	numObjects = 0;
	loadObjDirG("obj/");
	numObjects = puzzleItems;//for now we over write with a selected number, it will load all the 3d objects available

	offsetSlicer = ofVec3f(0,0,0);
	rotateSlicer = ofVec3f (0,0,0);

	int gridSize = 3;//this will be a shuffled int for diferent shapes, and predetermined for predetermined shapes

	if(numObjects !=0){
		middlePuzzlePos.x = ofGetWindowWidth();
		middlePuzzlePos.y = (ofGetWindowHeight()/2)-90;
		middlePuzzlePos.z = 0;
		//////////////////////////////////create cutter
		slicingPos.x = 0;
		slicingPos.y = 0;
		slicingPos.z = 0;
		/////////////////////////////////////Pre create puzzles to be loaded
		cout << "creating puzzle menu items" << endl;
		middlePuzzlePos.x = 0;
		middlePuzzlePos.y = (ofGetWindowHeight()/2)-90;
		middlePuzzlePos.z = -100;

		//ask for 50 puzzle memory, 5 for each menu puzzle displayed
		//1 for the middle
		//4 for the stations, one for each station
		myPuzzles = (puzzle**)malloc((numObjects*5)*sizeof(puzzle*));

		myCutter = new cutter(0.01,1000,100,1,offsetSlicer,gridSize);// number of slices == same number in slicer /// where to get the grid value??
		myCutter->setup();
		mySlicer = new slicer(myCutter,gridSize);
		mySlicer->setup();

		for(int i=0; i < numObjects; i++){
			cout << "creating puzzle menu object: " << i <<endl;
			//cout << "menu puzzle IN: " << ofGetElapsedTimeMillis() << endl;
			middlePuzzlePos.x = 10 + (i * 10) + (i*180);
			objectToMakePuzzle = new myobject3D (slicingPos, middlePuzzlePos,"main");//all on 0,0,0
			objectToMakePuzzle->loadObjectOD((sgC3DObject*)objectsMP[i+1]->Clone(),i+1);

			puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, i);
			puzzleDisplayed->loadObjectMP((sgC3DObject*)objectsMP[i+1]->Clone(),i+1,objectToMakePuzzle->ObjectUniqueNormals);

			objectToMakePuzzle->setup();
			objectToMakePuzzle->update();
			//objectDisplayed->colorFacesMenu();//implement this later
			//objectDisplayed->init();//gui//no need for this, the objecs displayed will be created later "puzzles on demand"
			puzzleDisplayed->setup();
			puzzleDisplayed->update();
			puzzleDisplayed->colorFacesMenu();
			puzzleDisplayed->init();//gui

			//loop to make 5 puzzles for each menuPuzzle (puzzleDisplayed)
			//1 for the middle
			//4 for the stations, one for each station

			//only need to make boolean operation, slicer will save a copy of the pieces, we use those pieces copies, mySlicer->getPieces()
			mySlicer->intersectCubes((sgCObject*)objectToMakePuzzle->getObject()); 
			for(int j=0;j<5;j++){
				//mySlicer->intersectCubes((sgCObject*)objectToMakePuzzle->getObject());
				myPuzzles[j+(i*5)] = new puzzle(middlePuzzlePos, offsetSlicer,gridSize,objectToMakePuzzle->ObjectUniqueNormals); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
				//myPuzzles[j+(i*5)]->setup();

				myPuzzles[j+(i*5)]->loadPieces(mySlicer->getPieces(),objectToMakePuzzle->objectId,rotateSlicer);//selected object id is used for coloring

				if(objectToMakePuzzle->objectId < 8 ){
					myPuzzles[j+(i*5)]->colorFaces(objectToMakePuzzle->objectId);
				}else{
					myPuzzles[j+(i*5)]->colorTorus();
				}

				//myPuzzles[j+(i*5)]->colorFacesMenuPuzzle(puzzleDisplayed->objectId,puzzleDisplayed->uniqueNormals,puzzleDisplayed->colorsVMenu);
				//myPuzzle->colorFaces(i+1);
				myPuzzles[j+(i*5)]->colorCubiesBlackSides();
				puzzleDisplayed->loadPuzzle(myPuzzles[j+(i*5)],j);
			}

			middlePuzzles.push_back(puzzleDisplayed);
			cout << "end puzzle menu object: " << i <<endl;
			//cout << "menu puzzle OUT: " << ofGetElapsedTimeMillis() << endl;
		}

		//this is to control puzzles that are being saved
		puzzleCounter = 0;

		newPuzzleCounterBL = 0;
		currentCubieBL = 0;

		newPuzzleCOunterBR = 0;
		currentCubieBR = 0;

		newPuzzleCOunterTL = 0;
		currentCubieTL = 0;

		newPuzzleCOunterTR = 0;
		currentCubieTR = 0;

		////////////////////////////////Creating puzzles when menupuzzle is dragged to station, not previously created, this approach takes longer////////////////////////
		////////middlePuzzlePos.x = 0;
		////////middlePuzzlePos.y = (ofGetWindowHeight()/2)-90;
		////////middlePuzzlePos.z = 0;
		////////for(int i=0; i < numObjects; i++){
		////////	middlePuzzlePos.x = 10 + (i * 10) + (i*180);
		////////	puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, i);
		////////	puzzleDisplayed->loadObjectMP((sgC3DObject*)objectsMP[i+1]->Clone(),i+1);
		////////	cout << "created puzzle menu object: " << i <<endl;
		////////	puzzleDisplayed->setup();
		////////	puzzleDisplayed->update();
		////////	puzzleDisplayed->colorFacesMenu();
		////////	puzzleDisplayed->init();//gui

		////////	//mySlicer->intersectCubes((sgCObject*)puzzleDisplayed->getObject());
		////////	//myPuzzle = new puzzle(middlePuzzlePos, offsetSlicer,3); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
		////////	//myPuzzle->setup();
		////////	//myPuzzle->loadPieces(mySlicer->getPieces(),puzzleDisplayed->objectId,rotateSlicer);//selected object id is used for coloring

		////////	////this is a special function that takes, the unique normals, and unique colors of the menuPuzzle, to repeat those colors on the real puzzle of the menuPuzzle
		////////	//myPuzzle->colorFacesMenuPuzzle(puzzleDisplayed->objectId,puzzleDisplayed->uniqueNormals,puzzleDisplayed->colorsVMenu);

		////////	//cout << "created puzzle menu puzzle: " << i <<endl;
		////////	//puzzleDisplayed->loadPuzzle(myPuzzle);

		////////	//cout << "created puzzle menu item: " << i << endl;
		////////	middlePuzzles.push_back(puzzleDisplayed);
		////////}

	}


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
	//myGames.erase (myGames.begin(),myGames.end());
	game *tempGame = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime,"bl");
	myGames.push_back(tempGame);
	tr.x = 960;
	game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime,"br");
	myGames.push_back(tempGame2);
	tr.x = 300;
	tr.y = 60;
	game *tempGame3 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime,"tl");
	myGames.push_back(tempGame3);
	tr.x = 960;
	game *tempGame4 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos, tr, iddleTime,"tr");
	myGames.push_back(tempGame4);
	//create a second game
	//game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos,iddleTime);
	//myGames.push_back(tempGame2);
	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		//myGames[i]->setup(sgBunny->Clone(),sgTetrahedron->Clone(),sgDodecahedron->Clone(),sgIcosahedron->Clone(),sgOctahedron->Clone(), gameTags[i]);//,sgTeapot->Clone());
		myGames[i]->loadObjDir("obj/");
		myGames[i]->setup(gameTags[i]);
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

	//create hidden buttons
	ofVec2f sizeHB = ofVec2f(80,80);

	ofVec2f posHB1 = ofVec2f(0,(ofGetWindowHeight()/2)+100);
	ofVec2f posHB2 = ofVec2f(ofGetWindowWidth()-80,(ofGetWindowHeight()/2)+100);

	hb1 = new hiddenButton(posHB1,sizeHB);
	hb2 = new hiddenButton(posHB2,sizeHB);

	hb1Flag = false;
	hb1Flag2 = false;
	hb2Flag = false;
	shutDownTime =0;
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

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////SAVE PUZZLES ///////////////////////////////////
	/////////////////////////////////////////watch for new puzzles being saved
	for(int i = 0; i < myGames.size(); i++){
		if(myGames[i]->savePuzzleB == true){

			/////////////////////////////////////////////////////////////////////////////
			///make middle puzzles by building all puzzles from zero
			//////////////////////////////////////////////////////////////////////////////
			////////middlePuzzlePos.x = 10 + (numObjects * 10) + (numObjects*180);
			////////objectToMakePuzzle = new myobject3D (slicingPos, middlePuzzlePos,"main");//all on 0,0,0
			////////objectToMakePuzzle->loadObjectOD((sgC3DObject*)objectsMP[numObjects+1]->Clone(),numObjects+1);

			////////puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, numObjects);
			////////puzzleDisplayed->loadObjectMP((sgC3DObject*)objectsMP[numObjects+1]->Clone(),numObjects+1,objectToMakePuzzle->ObjectUniqueNormals);

			////////objectToMakePuzzle->setup();
			////////objectToMakePuzzle->update();
			//////////objectDisplayed->colorFacesMenu();//implement this later
			////////puzzleDisplayed->setup();
			////////puzzleDisplayed->update();
			////////puzzleDisplayed->colorFacesMenu();
			////////puzzleDisplayed->init();//gui

			//////////only need to make boolean operation, slicer will save a copy of the pieces, we use those pieces copies, mySlicer->getPieces()
			////////mySlicer->intersectCubes((sgCObject*)objectToMakePuzzle->getObject()); 

			////////for(int j=0;j<5;j++){
			////////	myPuzzles[j+(numObjects*5)] = new puzzle(middlePuzzlePos, offsetSlicer,3,objectToMakePuzzle->ObjectUniqueNormals); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
			////////	//myPuzzles[j+(i*5)]->setup();
			////////	myPuzzles[j+(numObjects*5)]->loadPieces(mySlicer->getPieces(),objectToMakePuzzle->objectId,rotateSlicer);//selected object id is used for coloring
			////////	myPuzzles[j+(numObjects*5)]->colorFacesMenuPuzzle(puzzleDisplayed->objectId,puzzleDisplayed->uniqueNormals,puzzleDisplayed->colorsVMenu);
			////////	//myPuzzle->colorFaces(i+1);
			////////	myPuzzles[j+(numObjects*5)]->colorCubiesBlackSides();
			////////	puzzleDisplayed->loadPuzzle(myPuzzles[j+(numObjects*5)],j);
			////////}
			////////middlePuzzles.push_back(puzzleDisplayed);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////make middle puzzles by making 5 clones
			//////////middlePuzzlePos.x = 10 + ((puzzleCounter+7) * 10) + ((puzzleCounter+7)*180);
			//////////objectToMakePuzzle = new myobject3D (slicingPos, middlePuzzlePos,"main");//all on 0,0,0
			//////////objectToMakePuzzle->loadObjectOD((sgC3DObject*)objectsMP[1]->Clone(),1);
			//////////objectToMakePuzzle->setup();
			//////////objectToMakePuzzle->update();

			//////////////////////////////////////////////////////////////
			////////////VERY IMPORTANT TO DO THIS HERE so the new middle puzzle gets registered as a subOb  guiNode properly
			//////////free(middlePuzzles.at((puzzleCounter +7)));
			///////////////////////////////////////////////////////////////

			//////////puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, (puzzleCounter+7));
			//////////puzzleDisplayed->loadObjectMP((sgC3DObject*)objectsMP[1]->Clone(),1,objectToMakePuzzle->ObjectUniqueNormals);

			//////////////objectDisplayed->colorFacesMenu();//implement this later
			//////////puzzleDisplayed->setup();
			//////////puzzleDisplayed->update();
			////////////puzzleDisplayed->colorFacesMenu();
			//////////puzzleDisplayed->init();//gui
			//////////puzzleDisplayed->offsetSlicer =  myGames[i]->offsetSlicer;
			//////////puzzleDisplayed->rotateSlicer =  myGames[i]->rotateSlicer;


			////////////put up a flag on the game that its saving the puzzle//used on restart()
			//////////myGames[i]->savePuzzle();

			//////////for(int j=0;j<5;j++){
			//////////	//have to clone the puzzle to make th eother 4 *puzzle
			//////////	//myPuzzles[j+((puzzleCounter+7)*5)] = myGames[i]->myPuzzle;
			//////////	myPuzzles[j+((puzzleCounter+7)*5)] = puzzleDisplayed->cloneMyPuzzle(myGames[i]->myPuzzle,myGames[i]->objectID);
			//////////	puzzleDisplayed->loadPuzzle(myPuzzles[j+((puzzleCounter+7)*5)],j);

			//////////	//myPuzzles[j+((puzzleCounter+7)*5)]->setup();
			//////////	////////myPuzzles[j+((numObjects-1)*5)]->loadPieces(myGames[i]->mySlicer->getPieces(),myGames[i]->objectID,tempMidPuzzle->rotateSlicer);//selected object id is used for coloring
			//////////	//copy color from original puzle
			//////////	//the original puzzle is tempMidPuzzle->myMenuPuzzle
			//////////	//myPuzzles[j+((puzzleCounter+7)*5)]->clonePuzzleColors(myGames[i]->myPuzzle);

			//////////	////myPuzzles[j+((puzzleCounter+7)*5)]->colorFacesMenuPuzzle(puzzleDisplayed->objectId,puzzleDisplayed->uniqueNormals,puzzleDisplayed->colorsVMenu);
			//////////	//myPuzzle->colorFaces(i+1);
			//////////	////myPuzzles[j+((puzzleCounter+7)*5)]->colorCubiesBlackSides();
			//////////	//myPuzzles[j+((numObjects-1)*5)],j);
			//////////}

			////////////replace current position 8,9,10 on the middelPuzzle Vector
			////////////because we are only showing 10 puzzles on the middle
			////////////we will replace the last 3

			////////////middlePuzzles.erase(middlePuzzles.begin() + (puzzleCounter+7));
			//////////middlePuzzles.at((puzzleCounter +7)) = puzzleDisplayed;
			//////////////keep count of the saved puzzles
			//////////puzzleCounter ++;
			//////////if(puzzleCounter == 3){
			//////////	puzzleCounter=0;
			//////////}
			////////////////////////reset save puzzle boolean on the game
			//////////myGames[i]->savePuzzleB = false;
			////////////////////////reset game
			//////////myGames[i]->guiReset();
			//////////myGames[i]->restart();
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



			//////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////make middle puzzles by making 5 clones ONE BY ONE
			/////ONE BY ONE BY ONE BY ONE BY ONE BY ONE BY ONE BY ONE BY ONE
			if(newPuzzleCounterBL < 6){
				if(newPuzzleCounterBL ==0){
					//we only do this once, for the first puzzle
					middlePuzzlePos.x = 10 + ((puzzleCounter+7) * 10) + ((puzzleCounter+7)*180);
					objectToMakePuzzle = new myobject3D (slicingPos, middlePuzzlePos,"main");//all on 0,0,0
					objectToMakePuzzle->loadObjectOD((sgC3DObject*)objectsMP[1]->Clone(),1);
					objectToMakePuzzle->setup();
					objectToMakePuzzle->update();

					////////////////////////////////////////////////////
					//VERY IMPORTANT TO DO THIS HERE so the new middle puzzle gets registered as a subOb  guiNode properly
					free(middlePuzzles.at((puzzleCounter +7)));
					/////////////////////////////////////////////////////

					puzzleDisplayed = new menuPuzzle(slicingPos, middlePuzzlePos, (puzzleCounter+7));
					puzzleDisplayed->loadObjectMP((sgC3DObject*)objectsMP[1]->Clone(),1,objectToMakePuzzle->ObjectUniqueNormals);

					////objectDisplayed->colorFacesMenu();//implement this later
					puzzleDisplayed->setup();
					puzzleDisplayed->update();
					//puzzleDisplayed->colorFacesMenu();
					puzzleDisplayed->init();//gui
					puzzleDisplayed->offsetSlicer =  myGames[i]->offsetSlicer;
					puzzleDisplayed->rotateSlicer =  myGames[i]->rotateSlicer;

					//put up a flag on the game that its saving the puzzle//used on restart()
					myGames[i]->savePuzzle();
				}
				//for(int j=0;j<5;j++){
				myPuzzles[j+((puzzleCounter+7)*5)] = puzzleDisplayed->cloneMyPuzzle(myGames[i]->myPuzzle,myGames[i]->objectID);
				puzzleDisplayed->loadPuzzle(myPuzzles[j+((puzzleCounter+7)*5)],j);

				//now we can count a puzzle being made
				newPuzzleCounterBL ++;

				//replace current position 8,9,10 on the middelPuzzle Vector
				//because we are only showing 10 puzzles on the middle
				//we will replace the last 3

				middlePuzzles.at((puzzleCounter +7)) = puzzleDisplayed;

				//////////////reset save puzzle boolean on the game
				myGames[i]->savePuzzleB = false;
				//////////////reset game
				myGames[i]->guiReset();
				myGames[i]->restart();
				//////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

		}
		////keep count of the saved puzzles
		puzzleCounter ++;
		if(puzzleCounter == 3){
			puzzleCounter=0;
		}
	}
}
////////END saving puzzle


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
////////////////////////////////////////////////////////hidden buttons program shut down
if((hb1Flag == true) && (hb2Flag == true) && (hb1Flag2==true)){
	exit();
}
}
//--------------------------------------------------------------
void testApp::draw(){
	//ofEnableDepthTest();
	ofEnableAlphaBlending();
	ofDisableDepthTest();
	//SceneManager::Instance()->draw("back");
	GuiConfigurator::Instance()->draw("back");
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


	////////////////////////PUZZLE  //////////////////////

	//////////glPushMatrix();
	//////////glTranslatef(ofGetWindowWidth()/2,ofGetWindowHeight()/2,-30);
	//////////myPuzzle->draw();
	//////////glPopMatrix();


	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->draw();
	}

	///////////////////END OF RENDERING     ////////////////////
	//stopOFLights();
	//ofDisableDepthTest();

	ofEnableAlphaBlending();
	ofDisableDepthTest();
	//SceneManager::Instance()->draw("front");
	GuiConfigurator::Instance()->draw("front");

	///////////////////////////////middle puzzles  ///////////////////////////////
	//ofEnableDepthTest();
	ofDisableAlphaBlending();
	////ofEnableLighting();
	////light.enable();
	////light.setPosition(ofGetWindowWidth()/ 2, ofGetWindowHeight()/ 2, 400);
	////light.lookAt(ofVec3f(ofGetWindowWidth()/ 2, ofGetWindowHeight()/ 2, 0));


	for(int i=0; i < middlePuzzles.size();i++){
		middlePuzzles[i]->draw();
	}
	//ofEnableDepthTest();
	//////light.disable();
	//////ofDisableLighting();
	//////ofEnableAlphaBlending();
	//ofDisableDepthTest();
	///////////////////////////////hidde buttons ///////////////////////////////

	hb1->draw();
	hb2->draw();


	ofEnableDepthTest();
	ofEnableAlphaBlending();
}

//-------------------------------------------------------------- 
void testApp::keyPressed(int key){
	myGames[0]->guiInput(key);

	////////////////////////////////////////////////////////////////////////
	/////////////////////////////loading puzzles from the middle
	////////////////////////////////////////////////////////////////////////
	if(myGames[0]->getCurrentStep() != -1){
		////////////////////////////////////////////////////////
		///////////from puzzles in the center
		//////instead of asking for a "key", with the GUI it should ask for the object ID
		if(key == 'p'){
			//myGames[0]->loadPuzzle(middlePuzzles[3]->getPuzzle());
			//myGames[0]->clearDisplayedObject();
			//myGames[0]->loadMenuObject(middlePuzzles[0]->objectId,myGames[0]->slicingPos,myGames[0]->posP,middlePuzzles[0]->colorsVMenu,middlePuzzles[0]->uniqueNormals);
			//myGames[0]->setCurrentStep(7);
		}
		//if(key == 'o'){
		//	//myGames[0]->loadPuzzle(middlePuzzles[8]->getPuzzle());
		//	myGames[0]->setCurrentStep(7);
		//}
		//if(key == 'i'){
		//	//myGames[0]->loadPuzzle(middlePuzzles[7]->getPuzzle());
		//	myGames[0]->setCurrentStep(7);
		//}
		//if(key == 'u'){
		//	//myGames[0]->loadPuzzle(middlePuzzles[1]->getPuzzle());
		//	myGames[0]->setCurrentStep(7);
		//}
		//if(key == 'y'){
		//	//myGames[0]->loadPuzzle(middlePuzzles[0]->getPuzzle());
		//	myGames[0]->setCurrentStep(7);
		//}
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
		if(USE_MOUSE){
			SubObMediator::Instance()->sendEvent("update-touch-point", ev);
		}
	}
	if(button == 2){
		myGames[0]->mouseDragged(x,y,button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(button == 0){
		//to make hidden buttons work for shutdown
		if(hb1->hitTest(x,y)){
			//first button pressed
			hb1Flag = true;
			shutDownTime = ofGetElapsedTimeMillis();
		}
		if(hb1Flag==true & hb2->hitTest(x,y)){
			//second button pressed, only after first button has been pressed
			int currentTime = ofGetElapsedTimeMillis();
			if(currentTime - shutDownTime > 5000){
				//too much time
				hb1Flag = false; // have to restart shut down sequence 
			}else{
				hb2Flag = true;
			}
		}
		if(hb1->hitTest(x,y) && hb2Flag == true){
			//first button pressed for the second time
			int currentTime = ofGetElapsedTimeMillis();
			if(currentTime - shutDownTime > 5000){
				hb1Flag = false;
				hb2Flag = false;
			}else{
				hb1Flag2 = true;
			}
		}

		//////////////////////////////////////////////

		touchId = nextTouchId;
		nextTouchId = (nextTouchId + 1) % 16;
		SubObEvent ev("add-touch-point");
		//ev.setName("add-touch-point");
		ev.addArg("position",ofVec3f((float)x / (float)ofGetWidth(),(float)y / (float)ofGetHeight(),0));
		ev.addArg("touch-id", touchId + touchIdOffset);
		if(USE_MOUSE){
			SubObMediator::Instance()->sendEvent("add-touch-point", ev);
		}
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
		if(USE_MOUSE){
			SubObMediator::Instance()->sendEvent("remove-touch-point", ev);
		}
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
		myGames[gameIds[gameTag]]->clearDisplayedObject();
		/////////////////////////////////////////////////////////////////////////////
		//loadMenuObject loads an object and builds a puzzle in realtime
		//myGames[gameIds[gameTag]]->loadMenuObject(middlePuzzles[mpId]->objectId,myGames[gameIds[gameTag]]->slicingPos,myGames[gameIds[gameTag]]->posP,middlePuzzles[mpId]->colorsVMenu,middlePuzzles[mpId]->uniqueNormals);
		/////////////////////////////////////////////////////////////////////////////
		//loadPuzzle loads a premade puzzle into a game
		myGames[gameIds[gameTag]]->loadPuzzle(middlePuzzles[mpId]->getPuzzle(gameTag),middlePuzzles[mpId]->objectId,myGames[gameIds[gameTag]]->slicingPos,myGames[gameIds[gameTag]]->posP);

		//myGames[gameIds[gameTag]]->setCurrentStep(7);
		myGames[gameIds[gameTag]]->prefix = gameTag;
		myGames[gameIds[gameTag]]->update(gameTag + ":"+_eventName,_event);
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
	//for(int i = 0; i < myGames.size(); i++){
	//	myGames[i]->restart();
	//	myGames[i]->exit();
	//}
	//sgDeleteObject(sgBunny);

	//myCutter->exit();
	//mySlicer->exit();
	//objectDisplayed->exit();
	//delete menu puzzles correctly
	//free(myPuzzles);
	//for(int i=0; i < middlePuzzles.size();i++){
	//	middlePuzzles[i]->exit();
	//}

	//free(hb1);
	//free(hb2);
	//sgFreeKernel();
	std::exit(0);
}
//----------------------------------------------------------------------------------------
void testApp::loadObjDirG(string _path){
	ofDirectory dir2(_path);
	dir2.allowExt("obj");
	dir2.listDir();
	vector<ofFile> filesG = dir2.getFiles();
	for(auto fIter = filesG.begin(); fIter != filesG.end(); fIter++){
		string name = fIter->getFileName();
		int id = ofToInt(ofSplitString(name, "_")[0]);
		loaderG.loadModel(_path + name);
		ofMesh tempMesh = loaderG.getMesh(0);
		vector<ofVec3f> vertsG = tempMesh.getVertices();
		//make an array[] from this vector
		SG_POINT *vertG = new SG_POINT[vertsG.size()];
		for(int i=0;i<vertsG.size(); i++){
			vertG[i].x = vertsG[i].x;
			vertG[i].y = vertsG[i].y;
			vertG[i].z = vertsG[i].z;
		}
		//get indices from mesh
		vector<ofIndexType> indices = tempMesh.getIndices();
		//make an array[] from this vector
		SG_INDEX_TRIANGLE *indexes2 = new SG_INDEX_TRIANGLE[indices.size()];
		for(int i=0;i<indices.size(); i++){
			indexes2->ver_indexes[i] = indices[i];
		}
		//generate sgC3DObject from geometry information
		sgCObject* tObj2 = sgFileManager::ObjectFromTriangles(vertG,vertsG.size(),indexes2,indices.size()/3); 
		objectsMP[id] = tObj2;
		numObjects ++;
		delete [] vertG;
		delete [] indexes2;
	}
}
//---------------------------------------------------------------------------------------
//void testApp::loadOBJfiles(){
//	////////////////////////////////////////load heavy models
//	bunny.loadModel("bunny.obj");
//	//need to make it an sgCore3DObject to be able to slice it
//	ofMesh tempMesh = bunny.getMesh(0);
//	//get vertices from mesh
//	vector<ofVec3f> bunnyVert = tempMesh.getVertices();
//	//make an array[] from this vector
//	SG_POINT *vert = new SG_POINT[bunnyVert.size()];
//	for(int i=0;i<bunnyVert.size(); i++){
//		vert[i].x = bunnyVert[i].x;
//		vert[i].y = bunnyVert[i].y;
//		vert[i].z = bunnyVert[i].z;
//	}
//	//get indices from mesh
//	vector<ofIndexType> bunnyIndices = tempMesh.getIndices();
//	//make an array[] from this vector
//	SG_INDEX_TRIANGLE *indexes = new SG_INDEX_TRIANGLE[bunnyIndices.size()];
//	for(int i=0;i<bunnyIndices.size(); i++){
//		indexes->ver_indexes[i] = bunnyIndices[i];
//	}
//	//generate sgC3DObject from geometry information
//	sgBunny = sgFileManager::ObjectFromTriangles(vert,bunnyVert.size(),indexes,bunnyIndices.size()/3); 
//	delete [] vert;
//	delete [] indexes;
//	////////////////////////////////////////load heavy models
//	tetrahedron.loadModel("tetrahedronNew.obj");
//	//need to make it an sgCore3DObject to be able to slice it
//	ofMesh tempMesh1 = tetrahedron.getMesh(0);
//	//get vertices from mesh
//	vector<ofVec3f> tetrahedronVert = tempMesh1.getVertices();
//	//make an array[] from this vector
//	SG_POINT *vert1 = new SG_POINT[tetrahedronVert.size()];
//	for(int i=0;i<tetrahedronVert.size(); i++){
//		vert1[i].x = tetrahedronVert[i].x;
//		vert1[i].y = tetrahedronVert[i].y;
//		vert1[i].z = tetrahedronVert[i].z;
//	}
//	//get indices from mesh
//	vector<ofIndexType>  tetrahedronIndices = tempMesh1.getIndices();
//	//make an array[] from this vector
//	SG_INDEX_TRIANGLE *indexes1 = new SG_INDEX_TRIANGLE[tetrahedronIndices.size()];
//	for(int i=0;i<tetrahedronIndices.size(); i++){
//		indexes1->ver_indexes[i] = tetrahedronIndices[i];
//	}
//	//generate sgC3DObject from geometry information
//	sgTetrahedron = sgFileManager::ObjectFromTriangles(vert1,tetrahedronVert.size(),indexes1,tetrahedronIndices.size()/3); 
//	delete [] vert1;
//	delete [] indexes1;
//	////////////////////////////////////////load heavy models
//	dodecahedron.loadModel("dodecahedron.obj");
//	//need to make it an sgCore3DObject to be able to slice it
//	ofMesh tempMesh2 = dodecahedron.getMesh(0);
//	//get vertices from mesh
//	vector<ofVec3f> dodecahedronVert = tempMesh2.getVertices();
//	//make an array[] from this vector
//	SG_POINT *vert2 = new SG_POINT[dodecahedronVert.size()];
//	for(int i=0;i<dodecahedronVert.size(); i++){
//		vert2[i].x = dodecahedronVert[i].x;
//		vert2[i].y = dodecahedronVert[i].y;
//		vert2[i].z = dodecahedronVert[i].z;
//	}
//	//get indices from mesh
//	vector<ofIndexType>  dodecahedronIndices = tempMesh2.getIndices();
//	//make an array[] from this vector
//	SG_INDEX_TRIANGLE *indexes2 = new SG_INDEX_TRIANGLE[dodecahedronIndices.size()];
//	for(int i=0;i<dodecahedronIndices.size(); i++){
//		indexes2->ver_indexes[i] = dodecahedronIndices[i];
//	}
//	//generate sgC3DObject from geometry information
//	sgDodecahedron = sgFileManager::ObjectFromTriangles(vert2,dodecahedronVert.size(),indexes2,dodecahedronIndices.size()/3); 
//	delete [] vert2;
//	delete [] indexes2;
//	////////////////////////////////////////load heavy models
//	icosahedron.loadModel("icosahedron.obj");
//	//need to make it an sgCore3DObject to be able to slice it
//	ofMesh tempMesh3 = icosahedron.getMesh(0);
//	//get vertices from mesh
//	vector<ofVec3f> icosahedronVert = tempMesh3.getVertices();
//	//make an array[] from this vector
//	SG_POINT *vert3 = new SG_POINT[icosahedronVert.size()];
//	for(int i=0;i<icosahedronVert.size(); i++){
//		vert3[i].x = icosahedronVert[i].x;
//		vert3[i].y = icosahedronVert[i].y;
//		vert3[i].z = icosahedronVert[i].z;
//	}
//	//get indices from mesh
//	vector<ofIndexType>  icosahedronIndices = tempMesh3.getIndices();
//	//make an array[] from this vector
//	SG_INDEX_TRIANGLE *indexes3 = new SG_INDEX_TRIANGLE[icosahedronIndices.size()];
//	for(int i=0;i<icosahedronIndices.size(); i++){
//		indexes3->ver_indexes[i] = icosahedronIndices[i];
//	}
//	//generate sgC3DObject from geometry information
//	sgIcosahedron = sgFileManager::ObjectFromTriangles(vert3,icosahedronVert.size(),indexes3,icosahedronIndices.size()/3); 
//	delete [] vert3;
//	delete [] indexes3;
//	////////////////////////////////////////load heavy models
//	octahedron.loadModel("octahedron.obj");
//	//need to make it an sgCore3DObject to be able to slice it
//	ofMesh tempMesh4 = octahedron.getMesh(0);
//	//get vertices from mesh
//	vector<ofVec3f> octahedronVert = tempMesh4.getVertices();
//	//make an array[] from this vector
//	SG_POINT *vert4 = new SG_POINT[octahedronVert.size()];
//	for(int i=0;i<octahedronVert.size(); i++){
//		vert4[i].x = octahedronVert[i].x;
//		vert4[i].y = octahedronVert[i].y;
//		vert4[i].z = octahedronVert[i].z;
//	}
//	//get indices from mesh
//	vector<ofIndexType>  octahedronIndices = tempMesh4.getIndices();
//	//make an array[] from this vector
//	SG_INDEX_TRIANGLE *indexes4 = new SG_INDEX_TRIANGLE[octahedronIndices.size()];
//	for(int i=0;i<octahedronIndices.size(); i++){
//		indexes4->ver_indexes[i] = octahedronIndices[i];
//	}
//	//generate sgC3DObject from geometry information
//	sgOctahedron = sgFileManager::ObjectFromTriangles(vert4,octahedronVert.size(),indexes4,octahedronIndices.size()/3); 
//	delete [] vert4;
//	delete [] indexes4;
//}
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////
// OF rendering
void testApp::initOFRender(){
	ofGLRenderer(true);

	ofSetVerticalSync(true);
	ofEnableDepthTest();

	//////////// turn on smooth lighting //
	//bSmoothLighting = true;
	ofSetSmoothLighting(true);

	//////////// Point lights emit light in all directions //
	//////////// set the diffuse color, color reflected from the light source //
	//////////pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

	//////////// specular color, the highlight/shininess color //
	//////////pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
	//////////pointLight.setPointLight();

	//////////spotLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	//////////spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	//////////// turn the light into spotLight, emit a cone of light //
	//////////spotLight.setSpotlight();

	//////////// size of the cone of emitted light, angle between light axis and side of cone //
	//////////// angle range between 0 - 90 in degrees //
	//////////spotLight.setSpotlightCutOff( 50 );

	//////////// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	//////////// range 0 - 128, zero is even illumination, 128 is max falloff //
	//////////spotLight.setSpotConcentration( 45 );

	//////////// Directional Lights emit light based on their orientation, regardless of their position //
	//////////directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 0.f));
	//////////directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	//////////directionalLight.setDirectional();

	//////////// set the direction of the light
	//////////// set it pointing from left to right -> //
	//////////directionalLight.setOrientation( ofVec3f(0, 90, 0) );


	//////////bShiny = true;
	//////////// shininess is a value between 0 - 128, 128 being the most shiny //
	//////////material.setShininess( 120 );
	//////////// the light highlight of the material //
	//////////material.setSpecularColor(ofColor(255, 255, 255, 1));

	//////////bPointLight = bSpotLight = bDirLight = true;

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
	////////////ofEnableLighting();
	////////////// enable the material, so that it applies to all 3D objects before material.end() call //
	////////////material.begin();
	////////////// activate the lights //
	////////////if (bPointLight) pointLight.enable();
	//////////////if (bSpotLight) spotLight.enable();
	////////////if (bDirLight) directionalLight.enable();

	////////////// grab the texture reference and bind it //
	////////////// this will apply the texture to all drawing (vertex) calls before unbind() //
	//////////////if(bUseTexture) ofLogoImage.getTextureReference().bind();
	//////////////if(bUseTexture) ofLogoImage.getTextureReference().unbind();

	////////////if (!bPointLight) pointLight.disable();
	//////////////if (!bSpotLight) spotLight.disable();
	////////////if (!bDirLight) directionalLight.disable();
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
