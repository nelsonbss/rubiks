#include "slicer.h"
#include "sgCore.h"
#include "cutter.h"

slicer::slicer(cutter *c){
	myCutter = c;
	////
	//create & initialize the pieces of the slicer as NULL
	pieces = (sgCGroup**)malloc(27*sizeof(sgCObject*));
	for(int i =0; i<27; i ++){
		pieces[i] = NULL; //initiallize every piece with NULL
	}
}
//--------------------------------------------------------------
void slicer::setup(){
}
//--------------------------------------------------------------
void slicer::update(){
}
//--------------------------------------------------------------
void slicer::draw(){  
}
//--------------------------------------------------------------
sgCGroup** slicer::getPieces(){
	//make a copy of the group** to send outside pieces[]
	aux = (sgCGroup**)malloc(27*sizeof(sgCGroup*));
	for(int i =0; i<27; i ++){
		sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
		sgCObject **objcts1 = (sgCObject**)malloc(50*sizeof(sgCObject*));
		int objctr = 0;
		//break each pieces[i]
		//sgCGroup *parts = pieces[i];
		if(pieces[i] != NULL){
			const int ChCnt = pieces[i]->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			pieces[i]->BreakGroup(allParts);
			sgDeleteObject(pieces[i]); //break group and delete each object?
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
			pieces[i] = sgCGroup::CreateGroup(objcts,objctr); //so pieces[] has the data again, and keeps it for future requests
			aux[i] = sgCGroup::CreateGroup(objcts1,objctr);  
		}else{
			pieces[i] = NULL;
			aux[i] = NULL; 
		}
		free(objcts);
		free(objcts1);
	}
	return aux;
	//return NULL; //here for memory leaks testing
}
//--------------------------------------------------------------
int slicer::countPieces(){
	//it tells me how many pieces have info
	int count =0;
	for(int i =0; i<27; i ++){
		//break each pieces[i]
		sgCGroup *parts = pieces[i];
		if(parts != NULL){
			count ++;
		}
	}
	return count;
}
//--------------------------------------------------------------
//////////////////////////// Intersection///////////////////////
void slicer::intersectCubes(sgCObject *obj){
	//it uses intersection of 27 cubes, on the object, to get all the pieces for each cubie in one oeration
	//all pieces are left in pieces[]
	for(int i =0; i<27; i ++){
		sgCObject *tempObj = obj->Clone();
		sgCObject *tempCutter = myCutter->cubes[i]->Clone();
		//do intersecton at origin
		pieces[i] = sgBoolean::Intersection(*(sgC3DObject*)tempObj,*(sgC3DObject*)tempCutter); 
		//now we have the whole piece that goes into a cubie for that slicning cube
		//clean up
		sgDeleteObject(tempObj);
		sgDeleteObject(tempCutter);
	}
}
//---------------------------------------------------------------
void slicer::undoArmRotations(ofVec3f v){
	//for(int i =0; i<27; i ++){
	//	sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	//	int objctr = 0;
	//	//break each pieces[i]
	//	//sgCGroup *parts = pieces[i];
	//	if(pieces[i] != NULL){
	//		const int ChCnt = pieces[i]->GetChildrenList()->GetCount();
	//		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	//		pieces[i]->BreakGroup(allParts);
	//		sgDeleteObject(pieces[i]); //break group and delete each object?
	//		for (int j=0; j < ChCnt; j++){
	//			//clone each object
	//			sgCObject *temp = allParts[j];
	//			//put clone on *[] tomake new group
	//			objcts[objctr] = temp->Clone();
	//			objctr ++;
	//			sgCObject::DeleteObject(temp);
	//		}
	//		free(allParts);
	//		//put that new group inside aux**[]
	//		pieces[i] = sgCGroup::CreateGroup(objcts,objctr); //so pieces[] has the data again, and keeps it for future requests
	//	}else{
	//		pieces[i] = NULL;
	//	}
	//	free(objcts);
	//}
}
//---------------------------------------------------------------
void slicer::exit(){
	//sgCObject::DeleteObject(*pieces); //break and delete all objects!!
	free(myCutter);
	for (int j=0; j < 27; j++){
		if(pieces[j]!= NULL){
			sgCObject::DeleteObject(pieces[j]);
		}
	}
	free(pieces);
	//free(aux);//this is generating conflict on adding group to scene on cutter.cpp
}