#include "cubie.h"
#include "sgCore.h"

cubie::cubie(float x, float y){
	objects = NULL;
	posX = x;
	posY = y;
	rotH = 0.0;
	rotV = 0.0;
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
			SG_VECTOR vrotH = {0,1,0}; //rotate H
			SG_POINT protH = {0,0,0};
			objectList[j]->InitTempMatrix()->Rotate(protH,vrotH,rotH);
			SG_VECTOR vrotV = {1,0,0}; //rotate V
			SG_POINT protV = {0,0,0};
			objectList[j]->GetTempMatrix()->Rotate(protV,vrotV,rotV);
			//translations
			SG_VECTOR transBox11 = {posX,0,0}; 
			objectList[j]->GetTempMatrix()->Translate(transBox11);
			SG_VECTOR transBox121 = {0,posY,0}; 
			objectList[j]->GetTempMatrix()->Translate(transBox121);
			objectList[j]->ApplyTempMatrix();
			objectList[j]->DestroyTempMatrix();
			objectList[j]->SetAttribute(SG_OA_COLOR,rand()%27);
			//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_FULL);
			sgGetScene()->AttachObject(objectList[j]);
		}
	}else{
		cout << "null at draw" << endl;
	}
}
//--------------------------------------------------------------
void cubie::unDraw(){  
	//sgGetScene()->Clear();ll

	//detach from scene
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_HIDE);
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
void cubie::rotateH(float rad){
	rotH = rad;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::rotateV(float rad){
	rotV = rad;
}

//--------------------------------------------------------------
void cubie::moveH(float posx){
	posX = posx;
}
//--------------------------------------------------------------
void cubie::moveV(float posy){
	posY = posy;
}
