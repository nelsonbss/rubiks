#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

game::game(SG_VECTOR p, float w, float h, SG_VECTOR puzzlePos){
	posGame = p;
	width = w;
	height = h;

	posP.x = 0; //for the puzzle & sample object
	posP.y = 0;
	posP.z = 0;
	posA = puzzlePos; //for the armature

	rotP.x = 0; //rotation of puzzle
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
	///////////////////////////////
	myArmature = new armature (ofVec3f(posA.x,posA.y,0),300,300,10,3);
	myArmature->setup();
	/////////////////////////////////////////PUZzLE //////////
	updatePuzzle = false;
	//
	faceRotate = false;

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
	////////////////////////////////////////load heavy models
	pot.loadModel("teapot.obj");
	//need to make it an sgCore3DObject to be able to slice it
	ofMesh tempMesh5 = pot.getMesh(0);
	//get vertices from mesh
	vector<ofVec3f> teapot3Vert = tempMesh5.getVertices();
	//make an array[] from this vector
	SG_POINT *vert5 = new SG_POINT[teapot3Vert.size()];
	for(int i=0;i<teapot3Vert.size(); i++){
		vert5[i].x = teapot3Vert[i].x;
		vert5[i].y = teapot3Vert[i].y;
		vert5[i].z = teapot3Vert[i].z;
	}
	//get indices from mesh
	vector<ofIndexType>  teapot3Indices = tempMesh5.getIndices();
	//make an array[] from this vector
	SG_INDEX_TRIANGLE *indexes5 = new SG_INDEX_TRIANGLE[teapot3Indices.size()];
	for(int i=0;i<teapot3Indices.size(); i++){
		indexes5->ver_indexes[i] = teapot3Indices[i];
	}
	//generate sgC3DObject from geometry information
	sgTeapot = sgFileManager::ObjectFromTriangles(vert5,teapot3Vert.size(),indexes5,teapot3Indices.size()/3); 
	delete [] vert5;
	delete [] indexes5;
}
//--------------------------------------------------------------
void game::update(){

	if(step == 1 || step == 2 || step == 3){
		//if there is an object selected
		objectDisplayed->update(); //rotates the selected object...just for show
	}
	if(step == 3){
		//myArmature->update();//now its rotating onits own
		//should be movable like the puzzle
		////////////////////////////////////////////////////move all puzzle
		//myArmature->move(posA);
		//////////////////////////////////////////////rotate all puzzle
		//myArmature->rotate(rotA);
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

		myPuzzle->draw();
		//mySlicer->draw();
	}
	if(step == 5){
		//show puzzle
		//rotations can be made
		myPuzzle->draw();
		//mySlicer->draw();
	}
}
//----------------------------------------------------------------
void game::moveP(SG_VECTOR p){
	posP = p;
}
//---------------------------------------------------------------
void game::rotateP(SG_VECTOR r){
	rotP = r;
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
		if(objID == 4){
			//try to load the bunny
			objectDisplayed->loadObject((sgC3DObject *)sgBunny);
		}
		if(objID == 5){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgDodecahedron);
		}
		if(objID == 6){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgIcosahedron);
		}
		if(objID == 7){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgOctahedron);
		}
		if(objID == 8){
			//try to load the dodecahedron
			objectDisplayed->loadObject((sgC3DObject *)sgTeapot);
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
//-----------------------------------------------------------------
void game::changeColorToColor(ofFloatColor sc, ofFloatColor tc){
	myPuzzle->changeColorToColor(sc,tc);
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
