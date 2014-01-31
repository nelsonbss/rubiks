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
		cubie *auxCubie = new cubie();
		auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}
}
//--------------------------------------------------------------
void puzzle::update(){
}
//--------------------------------------------------------------
void puzzle::draw(){  

}
///////////////////////////////////////////////////////////////
void puzzle::loadPieces(slicer *slcr){

	//it loads the pieces that the slicer made
	//it loads them into its own cubies
	for(int i=0;i<27;i++){
		//get group from pieces[]
		//sgCGroup *parts = (sgCGroup*)pcs[i];//if I put the group directly on cubie... do I loose the object? do I have to clone it?
		sgCGroup *parts = (sgCGroup*)slcr->pieces[i];
		//temp cubie
		cubie *auxCubie = new cubie();
		if(parts != NULL){
			const int ChCnt = parts->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			parts->BreakGroup(allParts);
			sgCObject::DeleteObject(parts);

			sgCObject *obj[50];
			int realNumPieces=0;

			for (int j=0; j < ChCnt; j++){
				//clone each part
				sgCObject *tempOBJ = allParts[j]->Clone(); //each is an object
				obj[j] = tempOBJ;
				realNumPieces ++;
			}
			//make them a group
			sgCGroup* resGroup = sgCGroup::CreateGroup(obj,realNumPieces);  
			//put that gorup inside a cubie
			auxCubie->objects = resGroup;//here goes the new group of clones from the iriginal slicing
			//put that cubie on the cubies[]
			myCubies[i] = auxCubie; //am I loosing whtas inside auxCubie here?
			sgCObject::DeleteObject(resGroup);
			free(allParts);
		}else{
			auxCubie->objects = NULL;
			myCubies[i] = auxCubie; //am I loosing whtas inside auxCubie here?
		}

	}
}
