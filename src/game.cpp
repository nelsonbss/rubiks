#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

game::game(SG_VECTOR p, float w, float h){
	pos = p;
	width = w;
	height = h;
}
//--------------------------------------------------------------
#define planeThicknes 0.001
#define planeSize 400
#define tamCubie 100

#define displayX 500
#define displayY 400
#define displayZ 100


void game::setup(){
	step = 0;
	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	movePRight = false;
	movePLeft = false;
	movePUp = false;
	movePDown = false;
	//
	rotatePHright = false;
	rotatePHleft = false;
	rotatePVup = false;
	rotatePVdown = false;
	//
	tempDeg = 0.0;
	faceRotateCx = false;
	faceRotateCCx = false;
	faceRotateCy =false;
	faceRotateCCy = false;
	faceRotateCz = false;
	faceRotateCCz = false;

	createSlicer();
}
//--------------------------------------------------------------
void game::update(){
	////////////////////////////////////////////////////move all puzzle
	if(movePRight){
		myPuzzle->moveRight();
	}else if(movePLeft){
		myPuzzle->moveLeft();
	}else if(movePUp){
		myPuzzle->moveUp();
	}else if(movePDown){
		myPuzzle->moveDown();
	}
	////////////////////////////////////////////rotate all puzzle
	if(rotatePHright == true) {//rotate right
		myPuzzle->rotateHright();
	}
	if(rotatePHleft == true) {//rotate left
		myPuzzle->rotateHleft();
	}
	if(rotatePVup == true) {//rotate up
		myPuzzle->rotateVup();
	}
	if(rotatePVdown == true) {//rotate down
		myPuzzle->rotateVdown();
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////face rotations
	SG_POINT point = {0,0,0};
	SG_VECTOR axis = {1,0,0};
	int idcubie = 11;
	if(faceRotateCx == true) {//c
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axis,true);
		//drawCuts = true;
		faceRotateCx = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCx == true) {//

		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axis,false);
		//drawCuts = true;
		faceRotateCCx = false;
		//drawCuts = true;
	}

	SG_VECTOR axisy = {0,1,0};
	if(faceRotateCy == true) {//c
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axisy,true);
		//drawCuts = true;
		faceRotateCy = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCy == true) {//
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axisy,false);
		//drawCuts = true;
		faceRotateCCy = false;
		//cout << tempDeg << endl;
	}

	SG_VECTOR axisz = {0,0,1};
	if(faceRotateCz == true) {//c
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axisz,true);
		//drawCuts = true;
		faceRotateCz = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCz == true) {//
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axisz,false);
		//drawCuts = true;
		faceRotateCCz = false;
		//cout << tempDeg << endl;
	}


	if(step == 1){
		//if there is an object selected
		objectDisplayed->update(); //rotates the selected object...just for show
	}

	///////////////////////////////////////update cubies
	if(updatePuzzle){
		if(step == 3){
			myPuzzle->update();
			//updatePuzzle = false;
		}
	}
}
//--------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw the pieces////////////////////////////////////
	if((step == 1) || (step ==2)){
		objectDisplayed->draw();
		//myCutter->draw();
		//myCutter->unDraw();
	}
	if(step == 3){
		//made the cuts
		//mySlicer->draw();
		//drawCuts1 = false;
		myPuzzle->draw();
	}
}
//----------------------------------------------------------------
void game::loadObject (int objID,SG_VECTOR p,SG_VECTOR t){
	objectDisplayed = new myobject3D(p,t);
	if(objID == 1){
		//torus
		objectDisplayed->loadObject(sgCreateTorus(100,80,50,50));//pos.z is radius, thicknes, meridians
	}
	if(objID == 2){
		//cube
		objectDisplayed->loadObject(sgCreateBox(300,300,300));
	}if(objID == 3){
		//cone
		objectDisplayed->loadObject(sgCreateCone(200,1,300.0, 3));
	}

	step = 1;
	objectDisplayed->setup(pos);

	

	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
}
//--------------------------------------------
void game::createSlicer(){
	////////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1,0,0,-100); 
	myCutter->setup();
	//////////////////////////////////end create cutter///////////////////////////////////

	//////////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter,pos.x,pos.y);
	mySlicer->setup();
	///////////////////////////end create slicer /////////////////////////////////////////
}
//---------------------------------------------
void game::createPuzzle(SG_VECTOR p){
	////////////////////////////////create puzzle///////////////////////////////////////
	myPuzzle = new puzzle(p.x,p.y,p.z);
	myPuzzle->setup();
	//////////////////////////////end create puzzle////////////////////////////////////

	////BOOLEAN SUBSTRACTION//////////////////////////////////////////////////////////
	//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
	///boolean INTERSECTION///////////////////////////////////////////////////////////
	mySlicer->intersectCubes(objectDisplayed->getObject()); 
	//now slicer has all the parts inside sgCGroup ** = pieces[]
	//////////////////////////////create puzzle////////////////////////////////////////
	myPuzzle->loadPieces(mySlicer->getPieces());
	////////////////////////////////end create puzzle//////////////////////////////////

	updatePuzzle = true;
	step = 3;
}
