#include "cubie.h"
#include "sgCore.h"

cubie::cubie(){
	objects = NULL;
	moveH=200;
	moveV=500;
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){

}
//--------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	// it attaches to scene
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			SG_VECTOR rotD = {0,0,0};
			SG_POINT rot = {0,0,0};
			objectList[j]->InitTempMatrix()->Rotate(rot,rotD,0.0);
			SG_VECTOR transBox11 = {moveH,0,0}; 
			objectList[j]->GetTempMatrix()->Translate(transBox11);
			SG_VECTOR transBox121 = {0,moveV,0}; 
			objectList[j]->GetTempMatrix()->Translate(transBox121);
			objectList[j]->ApplyTempMatrix();
			objectList[j]->DestroyTempMatrix();
			objectList[j]->SetAttribute(SG_OA_COLOR,rand()%27);
			sgGetScene()->AttachObject(objectList[j]);
		}
	}else{
		cout << "null at draw" << endl;
	}
}
//--------------------------------------------------------------
void cubie::unDraw(){  
	//sgGetScene()->Clear();

	//detach from scene
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			sgGetScene()->DetachObject(objectList[j]);
		}
	}else{
		cout << "null at undraw" << endl;
	}

	////////////////////////////////////IMPORTANT!!!!!!!!///////////////////////////////////
	////remake objectList for next drawing
	if(objects != NULL){
		sgCGroup *objects2 = copyObjects(); 
		const int ChCnt = objects2->GetChildrenList()->GetCount();
		numObjs = ChCnt;
		//give this cubies list some memory
		objectList = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));

		//start breaking incoming group
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objects2->BreakGroup(allParts);
		sgCObject::DeleteObject(objects2);
		for (int j=0; j < ChCnt; j++){
			objectList[j] = allParts[j];
		}
		free(allParts);
	}else{
		cout << "null at undraw2" << endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
sgCGroup* cubie::copyObjects(){
	//make a copy of *objects send outside cubie
	//so originals dont get messed up, and cubies can draw every time without making a the boolean first

	sgCGroup* aux;
	sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	sgCObject **objcts1 = (sgCObject**)malloc(50*sizeof(sgCObject*));
	int objctr = 0;

	if(objects != NULL){
		const int ChCnt = objects->GetChildrenList()->GetCount();
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objects->BreakGroup(allParts);
		//sgCObject::DeleteObject(objects);
		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgCObject *temp = allParts[j];
			//put clone on *[] tomake new group
			objcts[objctr] = temp->Clone();
			objcts1[objctr] = temp->Clone();
			objctr ++;
			sgCObject::DeleteObject(temp);
		}
		free(allParts);
		//put that new group inside aux**[]
		objects = sgCGroup::CreateGroup(objcts,objctr); //so objects[] has the data again, and keeps it for future requests
		aux = sgCGroup::CreateGroup(objcts1,objctr);  
	}else{
		return NULL;
	}
	free(objcts);
	free(objcts1);
	return aux;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs){
	//it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	//it takes the input group and breaks it, to put parts on cubie group "objects"

	if(objs != NULL){

		sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
		int objctr = 0;

		//break incomming group
		const int ChCnt = objs->GetChildrenList()->GetCount();
		numObjs = ChCnt;
		//give this cubies list some memory
		objectList = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		//start breaking incoming group
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objs->BreakGroup(allParts);
		sgCObject::DeleteObject(objs);

		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgCObject *temp = allParts[j];
			//put clone on *[] tomake new group
			objectList[j] = temp->Clone();
			objcts[objctr] = temp->Clone();
			objctr ++;
		}
		free(allParts);
		//put that new group inside *objects of this class, of every cubie
		objects = sgCGroup::CreateGroup(objcts,objctr);
		free(objcts);
	}else{
		objects = NULL;
	}

}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::rotateUp(){
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
//--------------------------------------------------------------
void cubie::moveRight(){
	moveH += 10;
}
//--------------------------------------------------------------
void cubie::moveLeft(){
	moveH -= 10;
}
//--------------------------------------------------------------
void cubie::moveUp(){
	moveV -= 10;
}
//--------------------------------------------------------------
void cubie::moveDown(){
	moveV += 10;
}