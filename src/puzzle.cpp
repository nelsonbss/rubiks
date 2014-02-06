#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"

puzzle::puzzle(float x, float y){
	numPieces = 27;
	myCubies = (cubie**)malloc(numPieces*sizeof(cubie*));
	posX = x;
	posY = y;
	rotH = 0;
	rotV = 0;
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
//--------------------------------------------------------------
void puzzle::loadPieces(sgCGroup **pcs){
	//it loads the pieces that the slicer made, the pieces are in a sgCGroup** pieces[], 
	//this function receives a copy of that sgCGroup** made by mySlicer->getPieces()
	//it loads them into its own cubies

	//create cubies
	//so each time there is a new boolean operation, whole new cubies get created with variables in zero or blank
	for(int i =0;i<numPieces;i++){
		cubie *auxCubie = new cubie(posX,posY);// is this really creating independent instances of cubie??
		auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}

	for(int i=0;i<numPieces;i++){ //this is creating some 2400 ish objects not cleared!
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
	//sgCObject::DeleteObject(*pcs);
}
////////////////////////////////////////////////////////////////
void puzzle::rotateHright(){  
	//puzzle tells every cubie to rotate
	rotH += 0.1;
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotateH(rotH);
	}
}
//--------------------------------------------------------------
void puzzle::rotateHleft(){  
	//puzzle tells every cubie to rotate
	rotH -= 0.1;
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotateH(rotH);
	}
}
//--------------------------------------------------------------
void puzzle::rotateVup(){  
	//puzzle tells every cubie to rotate
	rotV += 0.1;
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotateV(rotV);
	}
}
//--------------------------------------------------------------
void puzzle::rotateVdown(){  
	//puzzle tells every cubie to rotate
	rotV -= 0.1;
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotateV(rotV);
	}
}
////////////////////////////////////////////////////////////////
void puzzle::moveRight(){
	//iterate through cubies
	posX += 10;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveH(posX);
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveLeft(){
	//iterate through cubies
	posX -= 10;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveH(posX);
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveUp(){
	//iterate through cubies
	posY -= 10;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveV(posY);
		}
	}
}
//--------------------------------------------------------------
void puzzle::moveDown(){
	//iterate through cubies
	posY += 10;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->moveV(posY);
		}
	}
}
////////////////////////////////////////////////////////////////
void puzzle::faceRotate(SG_POINT point, SG_VECTOR axis, float deg){
	//this function is to handle a face rotation for a cubbie
	//this function is invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	//16-17-18
	//7-8-9
	//25-26-27
	//all numbers -1 because array starts in zero 0
	//rotation point: 3D-center of cubie 8 [7]
	myCubies[15]->faceRotate(point,axis,deg);
	myCubies[16]->faceRotate(point,axis,deg);
	myCubies[17]->faceRotate(point,axis,deg);
	///
	myCubies[6]->faceRotate(point,axis,deg);
	myCubies[7]->faceRotate(point,axis,deg);
	myCubies[8]->faceRotate(point,axis,deg);
	///
	myCubies[24]->faceRotate(point,axis,deg);
	myCubies[25]->faceRotate(point,axis,deg);
	myCubies[26]->faceRotate(point,axis,deg);

}
//----------------------------------------------------------------
void puzzle::exit(){
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			//myCubies[i]->exit();
		}
	}
	free(myCubies);
}