#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"
#include "ofxTrackball.h"

#include <vector>

#define planeThicknes 0.001
#define tamCutter 1000


game::game(SG_VECTOR gamePos, float w, float h, SG_VECTOR displayPos){
	posGame = gamePos;
	width = w;
	height = h;

	posP.x = displayPos.x; //for the puzzle & sample object
	posP.y = displayPos.y;
	posP.z = displayPos.z;

	tamSideArmature = 300;
	posA.x = displayPos.x; //for the armature
	posA.y = displayPos.y;
	posA.z = displayPos.z;

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

	step = 0;
	idcubie=0;

	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	faceRotate = false;


}
//----------------------------------------------------------------------
void game::update(){

	if(step == 1 || step == 2 || step == 3){
		//if there is an object selected
		objectDisplayed->update(); //rotates the selected object...just for show
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
				//it only saves the idcubie. 
				//undo will look for the other 9 cubies involved and do a pop_back on their history
				historyV.push_back(history(idcubie,axis,!dir)); //new approach save inverse move, to do it at undo, and do 2 pop 
				faceRotate = false;
			}
			//updatePuzzle = false;
		}
	}
}
//----------------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(step == 0){
		//is waiting for a shape to be selected
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
		curRot.getRotate(angle, axistb);
		ofPushMatrix();
		ofTranslate(posP.x,posP.y,posP.z);
		//new trackball
		ofRotate(angle, axistb.x, axistb.y, axistb.z);
		ofTranslate(-posP.x,-posP.y,-posP.z);
		myArmature->draw();
		ofPopMatrix();
		//show selected object
		objectDisplayed->draw();
	}
	if(step == 4 ){
		//trackball
		//myTB->draw();

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
		//trackball
		//myTB->draw();

		//glMatrixMode(GL_MODELVIEW);

		//show puzzle
		curRot.getRotate(angle, axistb);

		double pitch = atan2(2*(curRot.y()*curRot.z()+curRot.w()*curRot.x()),curRot.w()*curRot.w()-curRot.x()*curRot.x()-curRot.y()*curRot.y()+curRot.z()*curRot.z());
		double roll = atan2(2*(curRot.x()*curRot.y()+curRot.w()*curRot.z()),curRot.w()*curRot.w()+curRot.x()*curRot.x()-curRot.y()*curRot.y()-curRot.z()*curRot.z());
		double yaw = asin(-2*(curRot.x()*curRot.z()-curRot.w()*curRot.y()));

		cout << "x:  "  << ofRadToDeg(pitch) << "y: " << ofRadToDeg(yaw) << "z: " << ofRadToDeg(roll) << endl;

		GLfloat* m  = new GLfloat[16];
		getMatrix( m, curRot.inverse() );//inverse the quaternion to have puzzle move correctly

		

		ofPushMatrix();
		glTranslatef(posP.x,posP.y,posP.z);

		//new trackball
		glRotated(angle, axistb.x, axistb.y, axistb.z);

		//quaternion to matrix, using the matrix here
		//glMultMatrixf(m);

		
		glGetDoublev(GL_MODELVIEW_MATRIX, model);// here I have the current matrix with the rotations that I need
		//upper 3x3 elements is the rotation matrix
		ofMatrix4x4 s = ofMatrix4x4(model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10],model[11],model[12],model[12],model[14],model[15]);
		//invert matrix
		ofMatrix4x4 inverseModel = ofMatrix4x4::getInverseOf(s);
		//use inverse matrix to apply rotations
		invRot = inverseModel.getRotate();
		invRot.getRotate(angle, axistb);
		
		///apply rotation cancelation by rotting wth the inverse
		//glRotated(angle, axistb.x, axistb.y, axistb.z);
		//it should do nothing. no rotations at the end



		//normal of rotate
		//ofRotateX(ofRadToDeg(pitch));
		//ofRotateY(ofRadToDeg(yaw));
		//ofRotateZ(ofRadToDeg(roll));

		ofTranslate(-posP.x,-posP.y,-posP.z);
		myPuzzle->draw();

		free(m);
		ofPopMatrix();

		
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
void game::loadObject(int objID, SG_VECTOR p, SG_VECTOR t){
	if (objectID == -1){
		objectDisplayed = new myobject3D(p,t);
	}else{
		objectDisplayed->exit();
		delete objectDisplayed;
		objectDisplayed = new myobject3D(p,t);
	}
	objectID = objID;
	if(step == 0 || step==1){

		if(objID == 1){
			//torus
			objectDisplayed->loadObject(sgCreateTorus(100,70,50,50),1);//(radius,thickness,meridiansDonut,meridiansDonutCut)
		}
		if(objID == 2){
			//cube
			objectDisplayed->loadObject(sgCreateBox(300,300,300),2);//(tamX,tamY,tamZ)
		}if(objID == 3){
			//cone
			//objectDisplayed->loadObject(sgCreateCone(250,1,250,3),3);
			objectDisplayed->loadObject((sgC3DObject *)sgTetrahedron->Clone(),3);
		}
		if(objID == 4){
			//try to load the bunny
			objectDisplayed->loadObject((sgC3DObject *)sgBunny->Clone(),4);
		}
		if(objID == 5){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgDodecahedron->Clone(),5);
		}
		if(objID == 6){
			//try to load the Icosahedron
			objectDisplayed->loadObject((sgC3DObject *)sgIcosahedron->Clone(),6);
		}
		if(objID == 7){
			//try to load the Octahedron
			objectDisplayed->loadObject((sgC3DObject *)sgOctahedron->Clone(),7);
		}
		//if(objID == 8){
		//	//try to load the Teapot
		//	objectDisplayed->loadObject((sgC3DObject *)sgTeapot->Clone(),8);
		//}
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
		myPuzzle->loadPieces(mySlicer->getPieces(),objectID,rotateSlicer);
		////////////////////////////////end create puzzle/////////////////////////////////

		///////////////////////////////  color puzzle   ////////////////////////////////// 
		//color all the faces for platonic solids!! colors outside for most objects(not bunny), black on the insides
		myPuzzle->colorFaces(objectID);

		////////////////////////   give puzzle trackball  //////////////////////////////
		//myTB = new ofxTrackball(ofGetWidth()/2, ofGetHeight()/2, 0, 2000, myPuzzle,false);

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
	//add this rotation to armRotations history
	//armRotH.push_back(armRotations(input));
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
	////////////////////////////////////////////step 0 inputs
	////////////////////////////////////////////step 0 inputs
	////////////////////////////////////////////step 0 inputs
	if(step == 0){
		SG_VECTOR objectPos = {0,0,0};  //where it gets sliced
		//SG_VECTOR posP = {ofGetWidth() / 2,ofGetHeight() / 2,displayZ}; // where the temp object will be showed to user
		//waiting for shape to be selected
		if(in == '1') {
			//load object recieves (object id, boolean position, display position) 
			loadObject(1,objectPos,posP);
		}
		if(in == '2') {
			loadObject(2,objectPos,posP);
		}
		if(in == '3') {
			loadObject(3,objectPos,posP);
		}
		if(in == '4') {
			loadObject(4,objectPos,posP);
		}
		if(in == '5') {
			loadObject(5,objectPos,posP);
		}
		if(in == '6') {
			loadObject(6,objectPos,posP);
		}
		if(in == '7') { 
			loadObject(7,objectPos,posP);
		}
		//if(in == '8') { 
		//	loadObject(8,objectPos,posP);
		//}
	}
	if(in == 'b'){
		ofToggleFullscreen();
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
		} 
		//user can change the selected object
		SG_VECTOR objectPos = {0,0,0}; 
		//waiting for shape to be selected
		if(in == '1') {
			//load object recieves (object id, boolean position, display position) 
			loadObject(1,objectPos,posP); //pos.z its the torus radious
		}
		if(in == '2') {
			loadObject(2,objectPos,posP);
		}
		if(in == '3') {
			loadObject(3,objectPos,posP);
		}
		if(in == '4') {
			loadObject(4,objectPos,posP);
		}
		if(in == '5') {
			loadObject(5,objectPos,posP);
		}
		if(in == '6') {
			loadObject(6,objectPos,posP);
		}
		if(in == '7') {
			loadObject(7,objectPos,posP);
		}
		/*if(in == '8') { 
		loadObject(8,objectPos,posP);
		}*/
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


		int idcubie = 11;//to follow this cubie for now //this will be decided upon touch, or click on top of puzzle
		int randcubie=0;
		if(myPuzzle->isMoving() == false){ //this is to prevent from reading events while puzzle is moving
			if(in == 'u'){
				//undo last move 
				unDo();
			}
			////////////////////////////////////////////// FACE ROTATIONS //////////////////////////////
			////////  x axis  ////  x axis
			if(in == 'q') {
				//if(rotateB == true) {//c
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,true);
				/*rotateB = false;
				}*/
			}
			if(in == 'a') {
				//if(rotateB == true) {//cc
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {1,0,0};
				rotateByIDandAxis(randcubie,axis,false);
				/*	rotateB = false;
				}*/
			}
			////////  y axis  ////  y axis
			if(in == 'w') {
				//if(rotateB == true) {
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,true);
				/*rotateB = false;
				}*/
			}if(in == 's') {
				//counter clockwise
				//if(rotateB == true) {
				randcubie = 11;//rand()%26;
				SG_VECTOR axis = {0,1,0};
				rotateByIDandAxis(randcubie,axis,false);
				/*rotateB = false;
				}*/
			}
			////////  z axis  ////  z axis
			if(in == 'e') {
				//if(rotateB == true) {
				randcubie = 11;//rand()%26;
				//clockwise
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,true);
				/*rotateB = false;
				}*/
			}if(in == 'd') {
				//if(rotateB == true) {
				//counter clockwise
				randcubie = 11;//rand()%26;
				SG_VECTOR axis = {0,0,1};
				rotateByIDandAxis(randcubie,axis,false);
				/*rotateB = false;
				}*/
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
	}

	/////////////////////////////////////////////////////////////////
	////////////////RESART button can work on any step, its not showed on step 1 (object selected)
	if(in == 'r'){
		//tell a game to restart 
		restart();
	}
	//if(puzzleExists == true){
	//	if(in == 'f') {
	//		cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
	//	}
	//	if(in == 'g') {
	//		cout << "nu pieces " << mySlicer->countPieces() << endl;
	//	}
	//}
}
//----------------------------------------------------------------------
void game::restart(){
	if(step==4 || step==5){
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
	//myPuzzle->mouseDragged(x,y,button);
	if(step == 4 || step == 5){
		ofVec2f mouse(x,y);
		ofQuaternion yRot(x-lastMouse.x, ofVec3f(0,1,0));
		ofQuaternion xRot(y-lastMouse.y, ofVec3f(-1,0,0));
		//curRot *= yRot*xRot;
		curRot.set(curRot*yRot*xRot);
		lastMouse = mouse;
	}
}

//--------------------------------------------------------------
void game::mousePressed(int x, int y, int button){
	//myPuzzle->mouseDragged(x,y,button);
	if(step == 4 || step == 5){
		lastMouse = ofVec2f(x,y);
	}
}

//--------------------------------------------------------------------------
void game::getMatrix( GLfloat * m, ofQuaternion quat ) {
	float x2 = quat.x() * quat.x();
	float y2 = quat.y() * quat.y();
	float z2 = quat.z() * quat.z();
	float xy = quat.x() * quat.y();
	float xz = quat.x() * quat.z();
	float yz = quat.y() * quat.z();
	float wx = quat.w() * quat.x();
	float wy = quat.w() * quat.y();
	float wz = quat.w() * quat.z();
	m[0] = 1.0f - 2.0f * (y2 + z2);
	m[1] = 2.0f * (xy - wz);
	m[2] = 2.0f * (xz + wy);
	m[3] = 0.0f;
	m[4] = 2.0f * (xy + wz);
	m[5] = 1.0f - 2.0f * (x2 + z2);
	m[6] = 2.0f * (yz - wx);
	m[7] = 0.0f;
	m[8] = 2.0f * (xz - wy);
	m[9] = 2.0f * (yz + wx);
	m[10] = 1.0f - 2.0f * (x2 + y2);
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}