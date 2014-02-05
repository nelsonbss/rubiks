#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"

puzzle::puzzle(){
	numPieces = 27;
	myCubies = (cubie**)malloc(numPieces*sizeof(cubie*));
}
//--------------------------------------------------------------
void puzzle::setup(){
	//create cubies each time loadPieces is called
	//because thats whe a new boolean occured
}
//--------------------------------------------------------------
void puzzle::update(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->update();
		}
	}
}
//--------------------------------------------------------------
void puzzle::draw(){  
	//puzzle tells every cubie to attach objects to scene
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->draw();
		}
	}
}
//--------------------------------------------------------------
void puzzle::unDraw(){  
	//puzzle tells every cubie to detach its objects from scene
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->unDraw();
		}
	}
}
//--------------------------------------------------------------
void puzzle::rotateUp(){  
	//puzzle tells every cubie to rotate
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotateUp();
	}
}
//-------------------------------------------------------------------------------------------------
int puzzle::giveNumCubies(){
	//tell how many cubies we have with objects inside
	int aux=0;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i]->objects!= NULL){
			aux++;
		}
	}
	return aux;
}
///////////////////////////////////////////////////////////////
void puzzle::loadPieces(sgCGroup **pcs){
	//it loads the pieces that the slicer made, the pieces are in a sgCGroup** pieces[], 
	//this function receives a copy of that sgCGroup** made by mySlicer->getPieces()
	//it loads them into its own cubies

	//create cubies
	//so each time there is a new boolean operation, whole new cubies get created with variables in zero or blank
	for(int i =0;i<numPieces;i++){
		cubie *auxCubie = new cubie();// is this really creating independent instances of cubie??
		auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}

	for(int i=0;i<numPieces;i++){
		//get group from pieces[] copy
		sgCGroup *part = pcs[i]; //pcs[i] will get destroyed!!!!!!!!!!!!!!!!!

		if(part != NULL){
			const int ChCnt = part->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			part->BreakGroup(allParts);
			sgCObject::DeleteObject(part);

			sgCObject **obj = (sgCObject**)malloc(50*sizeof(sgCObject*));
			int realNumPieces=0;

			for (int j=0; j < ChCnt; j++){
				//clone each part
				sgCObject *tempOBJ = allParts[j]; //each is an object that goes in one cubie
				obj[j] = tempOBJ;
				realNumPieces ++;
			}
			//make them a group
			sgCGroup* cubieGroup = sgCGroup::CreateGroup(obj,realNumPieces);  
			//put that gorup inside temp cubie
			myCubies[i]->setObjects(cubieGroup);//here goes the group of clones from the iriginal slicing pieces[]
			//put that cubie on the cubies[]
			//myCubies[i] = auxCubie; //am I loosing whtas inside auxCubie here?

			free(obj);
			free(allParts);
		}else{
			myCubies[i]->setObjects(NULL);
		}

	}
}
//--------------------------------------------------------------
void puzzle::moveRight(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveRight();
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveLeft(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveLeft();
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveUp(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveUp();
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveDown(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveDown();
		}
	}
}

