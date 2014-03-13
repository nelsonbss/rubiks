#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"
#include "drawingCanvas.h"

#include <vector>

#define planeThicknes 0.001
#define tamCutter 1000


game::game(SG_VECTOR gamePos, float w, float h, SG_VECTOR displayPos, float iddleTime){
	posGame = gamePos;
	slicingPos = posGame;
	width = w;
	height = h;

	iddleTimer = iddleTime;

	posP.x = displayPos.x; //for the puzzle & sample object
	posP.y = displayPos.y;
	posP.z = displayPos.z;

	tamSideArmature = 300;
	posA.x = displayPos.x; //for the armature
	posA.y = displayPos.y;
	posA.z = displayPos.z;

	//drawing canvas
	posCanvas.x = displayPos.x;
	posCanvas.y = displayPos.y;
	posCanvas.z = displayPos.z;
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

	extrudedB = false;
}
//--------------------------------------------------------------
void game::setup(sgCObject *sgBunnyi,sgCObject *sgTetrahedroni,sgCObject *sgDodecahedroni,sgCObject *sgIcosahedroni,sgCObject *sgOctahedroni){//,sgCObject *sgTeapoti){
	//gets the .obj files loaded and converted into sgC3DObject
	sgBunny = sgBunnyi;
	sgTetrahedron = sgTetrahedroni;
	sgDodecahedron = sgDodecahedroni;
	sgIcosahedron = sgIcosahedroni;
	sgOctahedron = sgOctahedroni;
	//sgTeapot = sgTeapoti;

	step = -1;
	idcubie=0;

	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	faceRotate = false;
	faceRotateB = false;//used in the 2 id rotation function
}
//----------------------------------------------------------------------
void game::update(){

	if(step != -1){
		/////take time sample to see if game has to go to standBy mode
		goToAttractStepS = ofGetElapsedTimef();
		if(goToAttractStepS - goToAttractStepI >= iddleTimer){
			restart();
			step = -1;
		}
	}

	if(step == 1 || step == 2 || step == 3){
		//if there is an object selected
		objectDisplayed->update();
	}

	//if(step == 3){
	//	//myArmature->update();//now its rotating onits own
	//}

	///////////////////////////////////////update cubies
	if(updatePuzzle){
		if(step == 4 || step == 5){
			myPuzzle->update();
			////////////////////////////////////////////////////move all puzzle
			//myPuzzle->move(posP);
			////////////////////////////////////////////rotate all puzzle
			//myPuzzle->rotate(rotP);
			//////////////////////////////////////////make face rotation
			if(faceRotate == true) {
				myPuzzle->rotateByIDandAxis(idcubie,axis,dir);

				//put this move on the game history vector
				//undo will look for the other 9 cubies involved and do a pop x2 on their history
				historyV.push_back(history(idcubie,axis,!dir)); //save inverse move (!), to do it at undo, and do 2 pop 
				faceRotate = false;
			}
			//updatePuzzle = false;
			if(faceRotateB == true) {
				int ans = myPuzzle->rotateTwoIds(idcubieA,idcubieB,dir);

				//put this move on the game history vector
				//undo will look for the other 9 cubies involved and do a pop x2 on their history
				if(ans/10 == 1){
					//1
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;
				}else if(ans/10 == 2){
					//2
					axis.x = 0;
					axis.y = 1;
					axis.z = 0;
				}else{
					//3
					axis.x = 0;
					axis.y = 0;
					axis.z = 1;
				}
				bool d = ans%2;//this is 0 or 1
				historyV.push_back(history(idcubieA,axis,!d)); //save inverse move (!), to do it at undo, and do 2 pop 
				faceRotateB = false;
			}
		}
	}
}
//----------------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw game steps////////////////////////////////////
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
		myArmature->draw();
		//show selected object
		objectDisplayed->draw();
	}
	if(step == 4 ){
		//made the cuts
		//show color palette
		//show puzzle

		curRot.getRotate(angle, axistb);

		ofPushMatrix();
		ofTranslate(posP.x,posP.y,posP.z);
		//new trackball
		ofRotate(angle, axistb.x, axistb.y, axistb.z);
		ofTranslate(-posP.x,-posP.y,-posP.z);
		myPuzzle->draw();
		ofPopMatrix();
	}
	if(step == 5){
		//show puzzle
		curRot.getRotate(angle, axistb);

		ofPushMatrix();
		ofTranslate(posP.x,posP.y,posP.z);
		//new trackball
		ofRotate(angle, axistb.x, axistb.y, axistb.z);


		ofTranslate(-posP.x,-posP.y,-posP.z);
		myPuzzle->draw();
		ofPopMatrix();
	}
	if(step == 6){
		//show drawing area

		myCanvas->draw();


		//////////////////////ofPolyline *draw =  new ofPolyline();

		//////////////////////ofPushMatrix();
		//////////////////////ofTranslate(posP.x,posP.y,posP.z);
		////////////////////////pentagon
		////////////////////////draw->addVertex(ofVec2f(-40,50));
		////////////////////////draw->addVertex(ofVec2f(40,50));
		////////////////////////draw->addVertex(ofVec2f(50,0));
		////////////////////////draw->addVertex(ofVec2f(0,-50));
		////////////////////////draw->addVertex(ofVec2f(-50,0));
		////////////////////////draw->addVertex(ofVec2f(-40,50));

		////////////////////////star
		//////////////////////draw->addVertex(ofVec2f(-5,-210));
		//////////////////////draw->addVertex(ofVec2f(60,-60));
		//////////////////////draw->addVertex(ofVec2f(210,-45));
		//////////////////////draw->addVertex(ofVec2f(105,60));
		//////////////////////draw->addVertex(ofVec2f(150,210));
		//////////////////////draw->addVertex(ofVec2f(-5,135));
		//////////////////////draw->addVertex(ofVec2f(-150,210));
		//////////////////////draw->addVertex(ofVec2f(-105,60));
		//////////////////////draw->addVertex(ofVec2f(-210,-45));
		//////////////////////draw->addVertex(ofVec2f(-60,-60));
		//////////////////////draw->addVertex(ofVec2f(-5,-210));

		////////////////////////draw->close(); // close the shape
		//////////////////////draw->draw();
		//////////////////////ofPopMatrix();

		//show build button to get drawing data
	}
}
//----------------------------------------------------------------------
void game::rotateByIDandAxis(int id, SG_VECTOR axs, bool d){
	if(axs.x==0 && axs.y==0 && axs.z==0){
		//stop any rotation
		faceRotate = false;
	}else{
		//allow rotation
		idcubie = id;
		dir = d;
		axis = axs;
		//updatePuzzle = true;
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
void game::loadPuzzle(int puzzleMenuObject){
} //load a puzzle from the puzzle menu on the center
//----------------------------------------------------------------------
void game::loadObject(int objID, SG_VECTOR p, SG_VECTOR t){
	if (objectID == -1){
		objectDisplayed = new myobject3D(p,t);
	}else{
		objectDisplayed->exit();
		delete objectDisplayed;
		objectDisplayed = new myobject3D(p,t);
	}
	objectID = objID;
	if(step == 0 || step==1 || step == 6){
		if(objID == 1){
			//torus
			objectDisplayed->loadObject(sgCreateTorus(100,70,50,50),1);//(radius,thickness,meridiansDonut,meridiansDonutCut)
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}
		if(objID == 2){
			//cube
			objectDisplayed->loadObject(sgCreateBox(300,300,300),2);//(tamX,tamY,tamZ)
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}if(objID == 3){
			//cone
			//objectDisplayed->loadObject(sgCreateCone(250,1,250,3),3);
			objectDisplayed->loadObject((sgC3DObject *)sgTetrahedron->Clone(),3);
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}
		if(objID == 4){
			//try to load the bunny
			objectDisplayed->loadObject((sgC3DObject *)sgBunny->Clone(),4);
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}
		if(objID == 5){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgDodecahedron->Clone(),5);
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}
		if(objID == 6){
			//try to load the Icosahedron
			objectDisplayed->loadObject((sgC3DObject *)sgIcosahedron->Clone(),6);
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}
		}
		if(objID == 7){
			//try to load the Octahedron
			objectDisplayed->loadObject((sgC3DObject *)sgOctahedron->Clone(),7);
			if(extrudedB){
				sgDeleteObject(extrudedObject);
				extrudedB = false;
			}

		}
		//if(objID == 8){
		//	//try to load the Teapot
		//	objectDisplayed->loadObject((sgC3DObject *)sgTeapot->Clone(),8);
		//}
		if(objID == 200){
			//load extruded object
			objectDisplayed->loadObject((sgC3DObject *)extrudedObject->Clone(),200);
		}
		objectDisplayed->setup();
		step = 1;
	}
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
}
//----------------------------------------------------------------------
void game::loadArmature(int type){
	//loads armature and creates cutter and slicer
	//have to clear myArmature, to load the new one, like load objects!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	rotateSlicer.x=0;
	rotateSlicer.y=0;
	rotateSlicer.z=0;
	if (armID == -1){
		//first time
		if(type == 1){
			tamCubie = 100;
			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
		}else if(type == 2){
			tamCubie = 50;
			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
		}
		armID = type;
	}else{
		//free (myArmature);
		//delete myArmature;
		if(type == 1){
			tamCubie = 100;
			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
		}else if(type == 2){
			tamCubie = 50;
			myArmature = new armature (ofVec3f(posA.x,posA.y,posA.z),tamSideArmature,tamSideArmature,10,tamCubie);
		}
		armID = type;
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
	myCutter = new cutter(planeThicknes,tamCutter,tamCubie,1,offsetSlicer);		
	myCutter->setup();
	//////////////////////////////////create slicer
	mySlicer = new slicer(myCutter);
	mySlicer->setup();
}
//-----------------------------------------------------------------------------------------
void game::createPuzzle(SG_VECTOR p){
	if(step == 3){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(p, offsetSlicer); // it receives the position to be displayed AND the offset of the armature/cutter to adapt slicing
		myPuzzle->setup();

		////boolean substraction//////////////////////////////////////////////////////////
		//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
		///////////////  BOOLEAN INTERSECTION          ///////////////////////////////////
		mySlicer->intersectCubes((sgCObject*)objectDisplayed->getObject()); 

		//now slicer has all the parts inside sgCGroup ** pieces[]
		//it recieves the armature rotations to undo them and show the puzzle in an original possition
		myPuzzle->loadPieces(mySlicer->getPieces(),objectID,rotateSlicer);
		////////////////////////////////end create puzzle/////////////////////////////////

		///////////////////////////////  color puzzle   ////////////////////////////////// 
		//color all the faces for platonic solids!! colors outside for most objects(not bunny), black on the insides
		myPuzzle->colorFaces(objectID);

		updatePuzzle = true;

		step = 4;
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
void game::changeColorToColor(ofFloatColor sc, ofFloatColor tc){
	myPuzzle->changeColorToColor(sc,tc);
}
//----------------------------------------------------------------------
void game::moveA (ofVec3f input){
	myArmature->moveA(input);
	//move the offset vector of the cutter at the same time as the armature
	offsetSlicer.x += input.x;
	offsetSlicer.y += input.y;
	offsetSlicer.z += input.z;
}
//----------------------------------------------------------------------
void game::rotateA (ofVec3f input){
	myArmature->rotateA(input);
	//move the offset vector of the cutter at the same time as the armature
	rotateSlicer.x += input.x;
	rotateSlicer.y += input.y;
	rotateSlicer.z += input.z;
}
//----------------------------------------------------------------------
ofVec3f game::giveOffset(){
	return offsetSlicer;
}
//----------------------------------------------------------------------
void game::unDo(){
	//new aproach
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
void game::guiLoad(int _obj){
	/*
	SG_VECTOR objectPos = {0,0,0};
	loadObject(_obj, objectPos, posP);
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
		///////////from puzzles in the center
		if(in == 'p'){
			loadPuzzle(100);
		}
		if(in == 'o'){
			loadPuzzle(101);
		}
		if(in == 'i'){
			loadPuzzle(102);
		}

		////////////////////////////////////////////////////////
		//////////////////object menu on the side
		//waiting for shape to be selected
		if(in == '1') {
			//load object recieves (object id, boolean position, display position) 
			loadObject(1,slicingPos,posP);
		}
		if(in == '2') {
			loadObject(2,slicingPos,posP);
		}
		if(in == '3') {
			loadObject(3,slicingPos,posP);
		}
		if(in == '4') {
			loadObject(4,slicingPos,posP);
		}
		if(in == '5') {
			loadObject(5,slicingPos,posP);
		}
		if(in == '6') {
			loadObject(6,slicingPos,posP);
		}
		if(in == '7') { 
			loadObject(7,slicingPos,posP);
		}
		//if(in == '8') { 
		//	loadObject(8,objectPos,posP);
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
				loadObject(1,slicingPos,posP); //pos.z its the torus radious
			}
			if(in == '2') {
				clearDisplayedObject();
				loadObject(2,slicingPos,posP);
			}
			if(in == '3') {
				clearDisplayedObject();
				loadObject(3,slicingPos,posP);
			}
			if(in == '4') {
				clearDisplayedObject();
				loadObject(4,slicingPos,posP);
			}
			if(in == '5') {
				clearDisplayedObject();
				loadObject(5,slicingPos,posP);
			}
			if(in == '6') {
				//user can change the selected object
				clearDisplayedObject();
				loadObject(6,slicingPos,posP);
			}
			if(in == '7') {
				//user can change the selected object
				clearDisplayedObject();

				loadObject(7,slicingPos,posP);
			}
			/*if(in == '8') { 
			loadObject(8,objectPos,posP);
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
			//select armature 1
			loadArmature(1); 
		}
		if(in == '2') {
			//select armature 2
			loadArmature(2);
		}
	}
	////////////////////////////////////////////step 3 inputs
	////////////////////////////////////////////step 3 inputs
	////////////////////////////////////////////step 3 inputs
	else if(step == 3){
		//armature was selected
		////showing armature    another armature can be selected
		if(in == '1') {
			//select armature 1
			loadArmature(1); 
		}
		if(in == '2') {
			//select armature 2
			loadArmature(2);
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
			//send the armature rotations to the 3dObject
			applyArmRotations();
			//now we know the offset position from the armature to create-> cutter & slicer
			createCutterSlicer();
			//do slicing
			createPuzzle(posP);//create Puzzle goes to step 4 to show the puzzle
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
			ofFloatColor sc = ofFloatColor (1, 1, 0); //yellow
			ofFloatColor menuColor = ofFloatColor (1, 0, 1); //this color comes from the GUI
			changeColorToColor(sc,menuColor);
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
		int randcubie=13;//rand()%26;//to follow this cubie for now //this will be decided upon touch, or click on top of puzzle
		if(myPuzzle->isMoving() == false){ //this is to prevent from reading events while puzzle is moving
			if(in == 'u'){
				//undo last move 
				unDo();
			}
			////////////////////////////////////////////// FACE ROTATIONS //////////////////////////////
			if(in == 'z') {
				//do rotationbased ontwo cubies id
				int cubieA = 11;
				int cubieB = 10;
				rotateTwoIds(cubieA,cubieB,true);
			}
			////////////////////////////////////////////// FACE ROTATIONS //////////////////////////////
			////////  x axis  ////  x axis
			if(in == 'q') {
				//clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,true);
			}
			if(in == 'a') {
				//counter clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,false);
			}
			////////  y axis  ////  y axis
			if(in == 'w') {
				//clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,true);
			}if(in == 's') {
				//counter clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,false);
			}
			////////  z axis  ////  z axis
			if(in == 'e') {
				//clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,true);
			}if(in == 'd') {
				//counter clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,false);
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
			//make extruded object
			extrudeObject(myCanvas->getPolyline());
		}else if(in == 's'){
			ofPolyline *draw =  new ofPolyline();
			//pentagon
			//draw->addVertex(ofVec2f(-40,50));
			//draw->addVertex(ofVec2f(40,50));
			//draw->addVertex(ofVec2f(50,0));
			//draw->addVertex(ofVec2f(0,-50));
			//draw->addVertex(ofVec2f(-50,0));
			//draw->addVertex(ofVec2f(-40,50));

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
			loadObject(1,slicingPos,posP);
		}
		else if(in == '2') {
			clearDisplayedObject();
			loadObject(2,slicingPos,posP);
		}
		else if(in == '3') {
			clearDisplayedObject();
			loadObject(3,slicingPos,posP);
		}
		else if(in == '4') {
			clearDisplayedObject();
			loadObject(4,slicingPos,posP);
		}
		else if(in == '5') {
			clearDisplayedObject();
			loadObject(5,slicingPos,posP);
		}
		else if(in == '6') {
			clearDisplayedObject();
			loadObject(6,slicingPos,posP);
		}
		else if(in == '7') { 
			clearDisplayedObject();
			loadObject(7,slicingPos,posP);
		}
		//if(in == '8') { 
		//	loadObject(8,objectPos,posP);
		//}
		else if(in == '9') { 
			clearDisplayedObject();
			prepareDrawing();
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
void game::extrudeObject(ofPolyline *drawing){
	//this functino extrudes the input ofPolylne

	vector< ofPoint > points = drawing->getVertices();

	sgCObject*  cont_objcts[2000];
	for (int i =0; i < points.size() ; i ++)  {
		if(i!= points.size()-1){
			cont_objcts[i] = sgCreateLine(points[i].x, 0, points[i].y,points[i+1].x, 0, points[i+1].y);
		}else{
			//its the last point.. make the line with the first point
			cont_objcts[i] = sgCreateLine(points[i].x, 0, points[i].y,points[0].x, 0, points[0].y);
		}
	}  

	sgCContour* win_cont = sgCContour::CreateContour(cont_objcts, points.size()-1);
	////extrude along vector
	SG_VECTOR extVec = {0,-300,0};  
	if (objectID == -1){
		extrudedObject = (sgC3DObject*)sgKinematic::Extrude(*win_cont,NULL,0,extVec,true);
	}else{
		free(extrudedObject);
		extrudedObject = (sgC3DObject*)sgKinematic::Extrude(*win_cont,NULL,0,extVec,true);
	}

	extrudedB = true;
	sgDeleteObject(win_cont);
	free(drawing);
	////////free(cont_objcts);

	//we  have the sg3DObjcect to load
	loadObject(200,slicingPos,posP);//using id=200
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
	loadObject(200,slicingPos,posP);//using id=200

}
//----------------------------------------------------------------------
void game::prepareDrawing(){
	if(canvasB == false){
		//create canvas object
		myCanvas = new drawingCanvas(posCanvas,500,500);
		canvasB = true;
	}
	else{
		myCanvas->exit();
		delete myCanvas;
		//create canvas object
		myCanvas = new drawingCanvas(posCanvas,500,500);
	}
	//extrusion
	step = 6;
}
//----------------------------------------------------------------------
void game::clearDisplayedObject(){
	if (objectID != -1){
		objectDisplayed->exit();
		delete objectDisplayed;
		objectID = -1;
	}
	if(canvasB){
		myCanvas->exit();
		delete myCanvas;
		canvasB = false;
	}
}
//----------------------------------------------------------------------
void game::restart(){
	if(step == 6){
		myCanvas->exit();
		if(canvasB){
			delete myCanvas;
			canvasB = false;
		}
		step = 0;
		objectID = -1;
	}else if(step==4 || step==5){
		myPuzzle->exit();
		myCutter->exit();
		mySlicer->exit();
		objectDisplayed->exit();
		objectID = -1;
		step = 0;
		armID = -1;
	}else if (step==3){
		objectDisplayed->exit();             //clean displayed object after puzzle is created, so we dont keep it until the exit or restart
		step = 0;
		objectID = -1;
		armID = -1;
	}else if (step==1 || step==2){
		objectDisplayed->exit();
		step = 0;
		objectID = -1;
	}

	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;

	rotateSlicer.x = 0;
	rotateSlicer.y = 0;
	rotateSlicer.z = 0;

	curRot.set (ofVec4f(0,0,0,0));

	if(extrudedB){
		sgDeleteObject(extrudedObject);
		extrudedB = false;
	}


}
//----------------------------------------------------------------------
void game::exit(){
	sgDeleteObject(sgBunny);
	sgDeleteObject(sgTetrahedron);
	sgDeleteObject(sgDodecahedron);
	sgDeleteObject(sgIcosahedron);
	sgDeleteObject(sgOctahedron);
	//sgDeleteObject(sgTeapot);
}
//--------------------------------------------------------------
void game::mouseDragged(int x, int y, int button){
	if(step == 4 || step == 5){
		ofVec2f mouse(x,y);
		ofQuaternion yRot(x-lastMouse.x, ofVec3f(0,1,0));
		ofQuaternion xRot(y-lastMouse.y, ofVec3f(-1,0,0));
		//curRot *= yRot*xRot;
		curRot.set(curRot*yRot*xRot);
		lastMouse = mouse;
	}else if(step == 6){
		myCanvas->mouseDragged(x,y,button);
	}
}
//--------------------------------------------------------------
void game::mousePressed(int x, int y, int button){
	if(step == 4 || step == 5){
		lastMouse = ofVec2f(x,y);
	}else if(step == 6){
		myCanvas->mousePressed(x,y,button);
	}
}
//--------------------------------------------------------------
void game::mouseReleased(int x, int y, int button){
	if(step == 6){
		myCanvas->mouseReleased(x,y,button);
	}
}