#include "cubie.h"
#include "sgCore.h"

cubie::cubie(){
	objects = NULL;
	move=600;
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){
	move += 100;
}
//--------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	// it attaches to scene

	//cout << "objects from slicer start:" << objects << endl;
	sgCGroup *result2 = copyObjects();
	sgCObject** allChilds3a = (sgCObject**)malloc(1*sizeof(sgCObject*));
	if(result2 != NULL){
		const int ChCnt = result2->GetChildrenList()->GetCount();
		if(allChilds3a != NULL){
			free(allChilds3a);
		}
		allChilds3a = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		result2->BreakGroup(allChilds3a);
		sgCObject::DeleteObject(result2);
		for (int j=0; j < ChCnt; j++){
			SG_VECTOR rotD = {0,1,0};
			SG_POINT rot = {0,0,0};
			/*sgC3DObject *aux = (sgC3DObject *) allChilds3a[j]; 
			aux->InitTempMatrix()->Rotate(rot,rotD,1.0);
			SG_VECTOR transBox11 = {move,0,0}; 
			aux->GetTempMatrix()->Translate(transBox11);
			SG_VECTOR transBox121 = {0,500,0}; 
			aux->GetTempMatrix()->Translate(transBox121);
			aux->ApplyTempMatrix();  
			aux->DestroyTempMatrix();
			aux->Triangulate(SG_VERTEX_TRIANGULATION);
			aux->SetAttribute(SG_OA_COLOR,rand()%50);
			sgGetScene()->AttachObject(aux);*/
			allChilds3a[j]->InitTempMatrix()->Rotate(rot,rotD,1.0);
			SG_VECTOR transBox11 = {move,0,0}; 
			allChilds3a[j]->GetTempMatrix()->Translate(transBox11);
			SG_VECTOR transBox121 = {0,500,0}; 
			allChilds3a[j]->GetTempMatrix()->Translate(transBox121);
			allChilds3a[j]->ApplyTempMatrix();  
			allChilds3a[j]->DestroyTempMatrix();
			//allChilds3a[j]->Triangulate(SG_VERTEX_TRIANGULATION); // it has to be a sgC3dObject to be able to do "Triangulate"
			allChilds3a[j]->SetAttribute(SG_OA_COLOR,rand()%27);
			sgGetScene()->DetachObject(allChilds3a[j]);
			sgGetScene()->AttachObject(allChilds3a[j]);
		}
		if(allChilds3a != NULL){
			free(allChilds3a);
		}
		
	}
	//cout << "objects from slicer end:" << objects << endl;
}
//--------------------------------------------------------------
void cubie::unDraw(){  
	//detach from scene
	sgCGroup *result2 = copyObjects();
	if(result2 != NULL){
		const int ChCnt = result2->GetChildrenList()->GetCount();
		sgCObject** allpieces = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		result2->BreakGroup(allpieces);
		//sgCObject::DeleteObject(result2);
		for (int j=0; j < ChCnt; j++){
			sgGetScene()->DetachObject(allpieces[j]);
		}
		free(allpieces);
	}
	//cout << "objects from slicer end:" << objects << endl;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
sgCGroup* cubie::copyObjects(){
	//make a copy of *objects send outside cubie
	//so originals dont get messed up, and cubies can draw every time without making a the boolean first

	sgCGroup* aux;
	sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	int objctr = 0;

	if(objects != NULL){
		const int ChCnt = objects->GetChildrenList()->GetCount();
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objects->BreakGroup(allParts);
		//sgCObject::DeleteObject(objects);
		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgCObject *temp = allParts[j]; ///HERE IS THE PROBLEM!!!!!
			//put clone on *[] tomake new group
			objcts[objctr] = temp;
			objctr ++;
		}
		free(allParts);
		//put thatnew group inside aux**[]
		objects = sgCGroup::CreateGroup(objcts,objctr); //so objects[] has the data again, and keeps it for future requests
		aux = sgCGroup::CreateGroup(objcts,objctr);  
	}else{
		return NULL;
	}
	free(objcts);
	return aux;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs){
	//it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	//it takes the input group and breaks it, to put parts on cubie group "objects"

	sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	int objctr = 0;

	const int ChCnt = objs->GetChildrenList()->GetCount();
	sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	objs->BreakGroup(allParts);
	sgCObject::DeleteObject(objs);//after this function ends, objs is messed up , so the caller of this function doesn't have to delete the inputed group

	for (int j=0; j < ChCnt; j++){
		//clone each object
		sgCObject *temp = allParts[j];
		//put clone on *[] tomake new group
		objcts[objctr] = temp;
		objctr ++;
	}
	free(allParts);
	
	//put that new group inside *objects of this class, of every cubie
	objects = sgCGroup::CreateGroup(objcts,objctr);  

	free(objcts);
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::rotate(){
	sgCGroup *result2 = copyObjects();  
	if(result2 != NULL){
		const int ChCnt = result2->GetChildrenList()->GetCount();
		sgCObject** allChilds3a = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		result2->BreakGroup(allChilds3a);
		sgCObject::DeleteObject(result2);
		const int ChCnt2 = result2->GetChildrenList()->GetCount();
		for (int j=0; j < ChCnt; j++){
			SG_VECTOR rotD = {0,1,0};
			SG_POINT rot = {0,0,0};
			allChilds3a[j]->InitTempMatrix()->Rotate(rot,rotD,1.57);
			allChilds3a[j]->ApplyTempMatrix();  
			allChilds3a[j]->DestroyTempMatrix();
			sgGetScene()->AttachObject(allChilds3a[j]);
		}
		free(allChilds3a);
	}
}