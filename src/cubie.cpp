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
	//each cubie draws its own sgCGroup *objects;

	sgCGroup *result2 = getObjects();  //DO I have to make a copy of this sgCGroup *objects???? so I don't ever loose the objects?
	//yessss!!!!!
	if(result2 != NULL){
		const int ChCnt = result2->GetChildrenList()->GetCount();
		sgCObject** allChilds3a = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		result2->BreakGroup(allChilds3a);// ->BreakGroup(allChilds3);
		sgCObject::DeleteObject(result2);
		for (int j=0; j < ChCnt; j++){
			SG_VECTOR rotD = {0,1,0};
			SG_POINT rot = {0,0,0};
			allChilds3a[j]->InitTempMatrix()->Rotate(rot,rotD,1.0);
			SG_VECTOR transBox11 = {500,0,0}; 
			allChilds3a[j]->GetTempMatrix()->Translate(transBox11);
			SG_VECTOR transBox121 = {0,500,0}; 
			allChilds3a[j]->GetTempMatrix()->Translate(transBox121);
			allChilds3a[j]->ApplyTempMatrix();  
			allChilds3a[j]->DestroyTempMatrix();
			allChilds3a[j]->SetAttribute(SG_OA_COLOR,rand()%50);
			sgGetScene()->AttachObject(allChilds3a[j]);
			break;
		}
		free(allChilds3a);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
sgCGroup* cubie::getObjects(){
	//make a copy of *objects send outside, so originals dont get messed up, and cubies can draw every time without making a the boolean first
	sgCGroup* aux;
	sgCObject *objcts[500];  
	int objctr = 0;

	if(objects != NULL){
		const int ChCnt = objects->GetChildrenList()->GetCount();
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objects->BreakGroup(allParts);
		//sgCObject::DeleteObject(objects);
		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgCObject *temp = allParts[j]->Clone(); ///HERE IS THE PROBLEM!!!!!
			//put clone on *[] tomake new group
			objcts[objctr] = temp;
			objctr ++;
		}
		free(allParts);
		//put thatnew group inside aux**[]
		objects = sgCGroup::CreateGroup(objcts,objctr); //so pieces[] has the data again, and keeps it for future requests
		aux = sgCGroup::CreateGroup(objcts,objctr);  
	}else{
		return NULL;
	}

	/////important!!!!!!!!!!!!!!!!important!!!!!!!!!!!///////////important/////////////////////////////
	//sgCObject::DeleteObject(*objcts); //why if I delete the objects everything goes KAPUT?????
	///////////////important////////////////////important//////////////important//////////////////////
	return aux;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
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
