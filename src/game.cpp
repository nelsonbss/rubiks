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
	puzzleExists = false;
	updatePuzzle = false;
	makeCut = false;
	drawCuts = false;
	drawCuts1 = false;
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
	///////////////////////////////////////BUILD PUZZLE ////////////////////////////////////
	if(makeCut==true){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(displayX,displayY,displayZ);
		myPuzzle->setup();
		//////////////////////////////end create puzzle////////////////////////////////////

		////BOOLEAN SUBSTRACTION//////////////////////////////////////////////////////////
		//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
		///boolean INTERSECTION///////////////////////////////////////////////////////////
		mySlicer->intersectCubes(objectDisplayed->getObject()); 
		//now slicer has all the parts inside sgCGroup ** = pieces[]
		//////////////////////////////create puzzle////////////////////////////////////////
		myPuzzle->loadPieces(mySlicer->getPieces());
		puzzleExists = true;
		////////////////////////////////end create puzzle//////////////////////////////////
		drawCuts1 = true;
		updatePuzzle = true;
		makeCut = false;
		///////////////////////////////////////////////////////////////////////////////////
	}
	/////////////////////////////////////BUILD PUZZLE ENDS ////////////////////////////////


	////////////////////////////////////////////////////move all puzzle
	if(movePRight){
		//myPuzzle->unDraw();
		myPuzzle->moveRight();
		drawCuts = true;
	}else if(movePLeft){
		//myPuzzle->unDraw();
		myPuzzle->moveLeft();
		drawCuts = true;
	}else if(movePUp){
		//myPuzzle->unDraw();
		myPuzzle->moveUp();
		drawCuts = true;
	}else if(movePDown){
		//myPuzzle->unDraw();
		myPuzzle->moveDown();
		drawCuts = true;
	}
	////////////////////////////////////////////rotate all puzzle
	if(rotatePHright == true) {//rotate right
		//myPuzzle->unDraw();
		myPuzzle->rotateHright();
		drawCuts = true;
	}
	if(rotatePHleft == true) {//rotate left
		//myPuzzle->unDraw();
		myPuzzle->rotateHleft();
		drawCuts = true;
	}
	if(rotatePVup == true) {//rotate up
		//myPuzzle->unDraw();
		myPuzzle->rotateVup();
		drawCuts = true;
	}
	if(rotatePVdown == true) {//rotate down
		//myPuzzle->unDraw();
		myPuzzle->rotateVdown();
		drawCuts = true;
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
		if(puzzleExists){
			myPuzzle->update();
			//updatePuzzle = false;
		}
	}
}
//--------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(drawCuts1==true){
		//made the cuts
		//mySlicer->draw();
		drawCuts1 = false;
		drawCuts = true;
	}

	if(step == 1){
		objectDisplayed->draw();
		//myCutter->draw();
		//myCutter->unDraw();
	}

	if(drawCuts==true){
		if(puzzleExists == true){
			myPuzzle->draw();
		}
		//drawCuts = false;
	}
}
//----------------------------------------------------------------
void game::loadObject (int objID,SG_VECTOR p){
	SG_VECTOR pos = {p.x,p.y,p.z};
	objectDisplayed = new myobject3D(pos);
	if(objID == 1){
		//torus
		objectDisplayed->loadObject(sgCreateTorus(pos.z,80,50,50));
	}
	if(objID == 2){
		//cube
		objectDisplayed->loadObject(sgCreateBox(300,300,300));
	}if(objID == 3){
		//cone
		objectDisplayed->loadObject(sgCreateCone(200,1,300.0, 3));
	}

	objectDisplayed->setup();

	step = 1;

	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
}
//--------------------------------------------
void game::createSlicer(){
	////////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1,0,0,-pos.z); 
	myCutter->setup();
	//////////////////////////////////end create cutter///////////////////////////////////

	//////////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter,pos.x,pos.y);
	mySlicer->setup();
	///////////////////////////end create slicer /////////////////////////////////////////
}
