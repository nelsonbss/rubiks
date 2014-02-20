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
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
}
//--------------------------------------------------------------
#define planeThicknes 0.001
#define planeSize 400
#define tamCubie 100

void game::setup(){
	step = 0;
	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	tempDeg = 0.0;
	faceRotateCx = false;
	faceRotateCCx = false;
	faceRotateCy =false;
	faceRotateCCy = false;
	faceRotateCz = false;
	faceRotateCCz = false;


}
//--------------------------------------------------------------
void game::update(){
	if(step==3){
		////////////////////////////////////////////////////move all puzzle
		myPuzzle->move(pos);
		////////////////////////////////////////////rotate all puzzle
		myPuzzle->rotate(rot);
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
	if(step ==0){
		ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
	}
	if((step == 1) || (step ==2)){
		ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
		objectDisplayed->draw();
		//myCutter->draw();
		//myCutter->unDraw();
	}
	if(step == 3){
		//made the cuts
		//mySlicer->draw();
		myPuzzle->draw();
	}
}
//----------------------------------------------------------------
void game::move(SG_VECTOR p){
	pos = p;
}
//---------------------------------------------------------------
void game::rotate(SG_VECTOR r){
	rot = r;
}
//----------------------------------------------------------------
void game::loadObject (int objID,SG_VECTOR p,SG_VECTOR t){
	if(step == 0 || step==1){
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
		objectDisplayed->setup();
		createSlicer();
		step = 1;
	}
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
}
//--------------------------------------------
void game::createSlicer(){
	////////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1,0,0,-100);							//this -100 if because of torus radious!!! have to fix this!! to adapt to other selected shapes!!
	myCutter->setup();
	//////////////////////////////////end create cutter///////////////////////////////////

	//////////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter,pos.x,pos.y);
	mySlicer->setup();
	///////////////////////////end create slicer /////////////////////////////////////////
}
//---------------------------------------------
void game::createPuzzle(SG_VECTOR p){
	if((step == 1) || (step ==2)){
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
}
//----------------------------------------------
void game::restart(){
	//go to step 0, clear puzzle, , clear pieces, clear object
	if(step==3 || step==4){
		myPuzzle->exit();
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
		step = 0;
	}else if (step==1 || step==2){
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
		step = 0;
	}
}
