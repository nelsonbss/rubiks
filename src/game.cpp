#include "game.h"
#include "sgCore.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

#define planeThicknes 0.001
#define tamCutter 900


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

	rotP.x = 0; //rotation of puzzle
	rotP.y = 0;
	rotP.z = 0;

	//offset
	tamCubie = 0;  //when creating armature this gets a size
	offsetSlicer.x = 0;
	offsetSlicer.y = 0;
	offsetSlicer.z = 0;   ///this is because of the torus!!!, have to fix this dammit!
	armID = -1;

	objectID = -1; //initialized on -1 because on stage=0 there is no object selected
}
//--------------------------------------------------------------
void game::setup(){
	step = 0;
	idcubie=0;

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
	sgTeapot = (sgC3DObject *) sgFileManager::ObjectFromTriangles(vert5,teapot3Vert.size(),indexes5,teapot3Vert.size()/3); 
	delete [] vert5;
	delete [] indexes5;
	/////////////////
	//loadObjects();
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
				/*historyV.push_back(history(idcubie,axis,dir));*/ //save the play
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
		myArmature->draw();
		//show selected object
		objectDisplayed->draw();
	}
	if(step == 4 ){
		//made the cuts
		//show color palette
		//show puzzle
		myPuzzle->draw();
	}
	if(step == 5){
		//show puzzle
		//rotations can be made
		myPuzzle->draw();
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
			objectDisplayed->loadObject(sgCreateCone(250,1,250,3),3);
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
		if(objID == 8){
			//try to load the Teapot
			objectDisplayed->loadObject((sgC3DObject *)sgTeapot->Clone(),8);
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
	setCurrentStep(3);
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
//----------------------------------------------------------------------
void game::createPuzzle(SG_VECTOR p, ofVec3f offset){
	if(step == 3){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(p, offset); // it receives the position to be displayed AND the offset of the armature/cutter to adapt rotations of cubies
		myPuzzle->setup();

		////boolean substraction//////////////////////////////////////////////////////////
		//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
		///////////////  BOOLEAN INTERSECTION          ///////////////////////////////////
		mySlicer->intersectCubes((sgCObject*)objectDisplayed->getObject()); 
		//now slicer has all the parts inside sgCGroup ** = pieces[]
		myPuzzle->loadPieces(mySlicer->getPieces(),objectID);
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
void game::rotateP(SG_VECTOR r){
	myPuzzle->rotate(r);
}
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
		objectDisplayed->exit();
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
}
//----------------------------------------------------------------------
void game::exit(){
	sgDeleteObject(sgBunny);
	sgDeleteObject(sgDodecahedron);
	sgDeleteObject(sgIcosahedron);
	sgDeleteObject(sgOctahedron);
	sgDeleteObject(sgTeapot);
	//}
}