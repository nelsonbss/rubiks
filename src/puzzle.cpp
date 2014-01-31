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
void puzzle::loadPieces(sgCGroup **pcs){
	//it loads the pieces that the slicer made
	//it loads them into its own cubies
	for(int i=0;i<27;i++){
		//get group from pieces[] copy
		sgCGroup *part = pcs[i]; //pcs[i] will get destroyed!!!!!!!!!!!!!!!!!

		//temp cubie
		cubie *auxCubie = new cubie();//if I put the group directly on cubie... do I loose the object? do I have to clone it?

		if(part != NULL){
			const int ChCnt = part->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			part->BreakGroup(allParts);
			sgCObject::DeleteObject(part);

			sgCObject *obj[500];
			int realNumPieces=0;

			for (int j=0; j < ChCnt; j++){
				//clone each part
				sgCObject *tempOBJ = allParts[j]->Clone(); //each is an object that goes in one cubie
				obj[j] = tempOBJ;
				realNumPieces ++;
			}
			//make them a group
			sgCGroup* cubieGroup = sgCGroup::CreateGroup(obj,realNumPieces);  
			//put that gorup inside temp cubie
			auxCubie->setObjects(cubieGroup);//here goes the group of clones from the iriginal slicing pieces[]
			//put that cubie on the cubies[]
			myCubies[i] = auxCubie; //am I loosing whtas inside auxCubie here?

			///////////////////////////////////important!!!!!!??????????????????????????????//////////////////////////////////////////////////////
			//sgCObject::DeleteObject(cubieGroup); //DOES THIS MESS UP THE DATA ON auxCubie->objects = cubieGroup; ??????????????????????????????????
			//most problable.. yes.. but auxCubie->setObjects() has made its own copy
			///////////////////////////////////important!!!!!!??????????????????????????????//////////////////////////////////////////////////////


			free(allParts);
		}else{
			auxCubie->objects = NULL;
			myCubies[i] = auxCubie; //am I loosing whtas inside auxCubie here?
		}

	}
}
