#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"

puzzle::puzzle(){
	myCubies = (cubie**)malloc(27*sizeof(cubie*));
}
//--------------------------------------------------------------
void puzzle::setup(){
	//create cubies
	for(int i =0;i<27;i++){
		cubie *auxCubie = new cubie();// is this really creating independent instances of cubie??
		auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}
}
//--------------------------------------------------------------
void puzzle::update(){
}
//--------------------------------------------------------------
void puzzle::draw(sgCScene *scn){  
	//puzzle tells every cubie to draw itself
	//iterate through cubies
	for(int i=0;i<1;i++){
		myCubies[i]->draw(scn);
	}
}
void puzzle::draw1(sgCScene *scn){  
	//puzzle tells every cubie to draw itself
	//iterate through cubies
	for(int i=0;i<1;i++){
		myCubies[i]->draw1(scn);
	}
}
void puzzle::rotate(){  
	//puzzle tells every cubie to draw itself
	//iterate through cubies
	for(int i=0;i<1;i++){
		myCubies[i]->rotate();
	}
}
//-------------------------------------------------------------------------------------------------
int puzzle::giveNumCubies(){
	//to test how many cubies we have here
	int aux=0;
	for(int i=0;i<27;i++){
		if(myCubies[i]->objects!= NULL){
			aux++;
		}
	}
	return aux;
}
///////////////////////////////////////////////////////////////
void puzzle::loadPieces(sgCGroup **pcs){
	//it loads the pieces that the slicer made, the pieces are in a sgCGroup**, 
	//this function receives a copy of that sgCGroup** made by mySlicer->getPieces()
	//it loads them into its own cubies
	for(int i=0;i<27;i++){
		//get group from pieces[] copy
		sgCGroup *part = pcs[i]; //pcs[i] will get destroyed!!!!!!!!!!!!!!!!!

		//temp cubie
		//cubie *auxCubie = new cubie();//if I put the group directly on cubie... do I loose the object? do I have to clone it?
		//thisgot commented out because puzzle contructed its cubies on setup()

		if(part != NULL){
			const int ChCnt = part->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			part->BreakGroup(allParts);
			sgCObject::DeleteObject(part);

			sgCObject *obj[500];
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

			///////////////////////////////////important!!!!!!??????????????????????????????//////////////////////////////////////////////////////
			//sgCObject::DeleteObject(cubieGroup); //its getting deleted inside auxCubie->setObjects(cubieGroup);
			///////////////////////////////////important!!!!!!??????????????????????????????//////////////////////////////////////////////////////
			free(allParts);
		}else{
			myCubies[i]->objects = NULL;
			//myCubies[i] = auxCubie; //am I loosing whats inside auxCubie here?
		}

	}
}
