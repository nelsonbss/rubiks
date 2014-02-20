#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

game::game(SG_VECTOR p, float w, float h){
	posP = p;
	posA = p;
	width = w;
	height = h;
	rotP.x = 0;
	rotP.y = 0;
	rotP.z = 0;
}
//--------------------------------------------------------------
#define planeThicknes 0.001
#define planeSize 400
#define tamCubie 100

void game::setup(){
	step = 0;
	idcubie=0;
	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	faceRotate = false;
}
//--------------------------------------------------------------
void game::update(){

	if(step == 1){
		//if there is an object selected
		objectDisplayed->update(); //rotates the selected object...just for show
	}

	///////////////////////////////////////update cubies
	if(updatePuzzle){
		if(step == 4 || step == 5){
			myPuzzle->update();
			////////////////////////////////////////////////////move all puzzle
			myPuzzle->move(posP);
			////////////////////////////////////////////rotate all puzzle
			myPuzzle->rotate(rotP);
			//////////////////////////////////////////make face rotation
			if(faceRotate == true) {
				myPuzzle->rotateByIDandAxis(idcubie,axis,dir);
				faceRotate = false;
			}
			//updatePuzzle = false;
		}
	}
}
//--------------------------------------------------------------
void game::draw(){  
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(step == 0){
		//is waiting for a shape to be selected
	}
	if (step == 1){
		//show selected object
		objectDisplayed->draw();
		//myCutter->draw();
		//myCutter->unDraw();
	}
	if(step == 2){
		//waiting for armature to be selected
	}
	if(step == 3){
		//armature has been selected
		//myArmature->draw();
	}
	if(step == 4 || step == 5){
		//made the cuts
		//show puzzle
		myPuzzle->draw();
		//mySlicer->draw();
	}
}
//----------------------------------------------------------------
void game::moveP(SG_VECTOR p){
	posP = p;
	//updatePuzzle = true;
}
//---------------------------------------------------------------
void game::rotateP(SG_VECTOR r){
	rotP = r;
	//updatePuzzle = true;
}
//-------------------------------------------------------------------
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
	mySlicer = new slicer(myCutter,posP.x,posP.y);
	mySlicer->setup();
	///////////////////////////end create slicer /////////////////////////////////////////
}
//---------------------------------------------
void game::createPuzzle(SG_VECTOR p){
	if(step == 3){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(p.x,p.y,p.z); // it receives th position to be displayed
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
		step = 4;
	}
}
//----------------------------------------------
int game::getCurrentStep(){
	return step;
}
//----------------------------------------------
void game::setCurrentStep(int s){
	step = s;
}
//----------------------------------------------
void game::restart(){
	//go to step 0, clear puzzle, , clear pieces, clear object
	if(step==4 || step==5){
		myPuzzle->exit();
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
		step = 0;
	}else if (step==1 || step==2 || step==3){
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
		step = 0;
	}
}
