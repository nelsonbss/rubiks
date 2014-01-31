#include "cubie.h"
#include "sgCore.h"

cubie::cubie(){
}
//--------------------------------------------------------------
void cubie::setup(){
	objects = NULL;
}
//--------------------------------------------------------------
void cubie::update(){
}
//--------------------------------------------------------------
void cubie::draw(){  

}

void cubie::setObjects(sgCGroup *objs){
	//this has to make a copy of the group that is receiving, to have its own copy
	//make a copy of the group* to send outside pieces[]
	sgCObject *objcts[500];  
	int objctr = 0;

	const int ChCnt = objs->GetChildrenList()->GetCount();
	sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	objs->BreakGroup(allParts);
	sgCObject::DeleteObject(objs);//after this function ends, objs is messed up , so the caller of this function doesn't have to delete the inputed group

	for (int j=0; j < ChCnt; j++){
		//clone each object
		sgCObject *temp = allParts[j]->Clone();
		//put clone on *[] tomake new group
		objcts[objctr] = temp;
		objctr ++;
	}
	free(allParts);
	//put that new group inside *objects of this class, of every cubie
	objects = sgCGroup::CreateGroup(objcts,objctr);  

	
}
