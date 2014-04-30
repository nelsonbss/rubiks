#include "slicer.h"
#include "sgCore.h"
#include "cutter.h"

slicer::slicer(cutter *c, int gSize){
	myCutter = c;
	////
	numPieces=gSize*gSize*gSize;
	//create & initialize the pieces of the slicer as NULL
	pieces = (sgCGroup**)malloc(numPieces*sizeof(sgCObject*));
	for(int i =0; i<numPieces; i ++){
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
	cout << "in get pieces: " << ofGetElapsedTimeMillis() << endl;
	//make a copy of the group** to send outside pieces[]
	aux = (sgCGroup**)malloc(numPieces*sizeof(sgCGroup*));
	for(int i =0; i<numPieces; i ++){
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
	cout << "out get pieces: " << ofGetElapsedTimeMillis() << endl;
	return aux;

	//return pieces;
	//return NULL; //here for memory leaks testing

}
//--------------------------------------------------------------
sgCGroup* slicer::getPiecesOneByOne(int cubieToGet){
	//cout << "in get pieces: " << ofGetElapsedTimeMillis() << endl;
	//make a copy of a single pieces[] to send outside

	aux2 = (sgCGroup*)malloc(1*sizeof(sgCGroup*));

	sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	sgCObject **objcts1 = (sgCObject**)malloc(50*sizeof(sgCObject*));
	int objctr = 0;
	//break pieces[cubieToGet], there might be more than one mesh inside that cubie

	if(pieces[cubieToGet] != NULL){
		const int ChCnt = pieces[cubieToGet]->GetChildrenList()->GetCount();
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		pieces[cubieToGet]->BreakGroup(allParts);
		sgDeleteObject(pieces[cubieToGet]); //break group and delete each object?
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
		//put that new group inside aux2->sgCGroup
		pieces[cubieToGet] = sgCGroup::CreateGroup(objcts,objctr); //so pieces[] has the data again, and keeps it for future requests
		aux2 = sgCGroup::CreateGroup(objcts1,objctr);  
	}else{
		pieces[cubieToGet] = NULL;
		aux2 = NULL; 
	}
	free(objcts);
	free(objcts1);

	//cout << "out get pieces: " << ofGetElapsedTimeMillis() << endl;
	return aux2;

	//return pieces;
	//return NULL; //here for memory leaks testing

}
//--------------------------------------------------------------
int slicer::countPieces(){
	//it tells me how many pieces have info
	int count =0;
	for(int i =0; i<numPieces; i ++){
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
	cout << "enter intersect: " << ofGetElapsedTimeMillis() << endl;
	//it uses intersection of numPieces cubes, on the object, to get all the pieces for each cubie in one oeration
	//all pieces are left in pieces[]
	for(int i =0; i<numPieces; i ++){
		sgCObject *tempObj = obj->Clone();
		sgCObject *tempCutter = myCutter->cubes[i]->Clone();
		//do intersecton at origin
		pieces[i] = sgBoolean::Intersection(*(sgC3DObject*)tempObj,*(sgC3DObject*)tempCutter); 
		//now we have the whole piece that goes into a cubie for that slicning cube
		//clean up
		sgDeleteObject(tempObj);
		sgDeleteObject(tempCutter);
	}
	cout << "out intersect: " << ofGetElapsedTimeMillis() << endl;
}
//---------------------------------------------------------------
void slicer::intersectCubesOneByOne(sgCObject *obj, int cubieToCut){
	//cout << "enter intersect: " << ofGetElapsedTimeMillis() << endl;
	//it uses intersection of numPieces cubes, on the object, to get the piece of that cubie, one by one
	//all pieces are left in pieces[]

	sgCObject *tempObj = obj->Clone();
	sgCObject *tempCutter = myCutter->cubes[cubieToCut]->Clone();
	//do intersecton at origin
	pieces[cubieToCut] = sgBoolean::Intersection(*(sgC3DObject*)tempObj,*(sgC3DObject*)tempCutter); 
	//now we have the whole piece that goes into a cubie for that slicning cube
	//clean up
	sgDeleteObject(tempObj);
	sgDeleteObject(tempCutter);

	//cout << "out intersect: " << ofGetElapsedTimeMillis() << endl;
}
//---------------------------------------------------------------
void slicer::undoArmRotations(ofVec3f v){
	//for(int i =0; i<numPieces; i ++){
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
	//for (int j=0; j < numPieces; j++){
	//	if(pieces[j]!= NULL){
	//		sgCObject::DeleteObject(pieces[j]);
	//	}
	//}
	free(pieces);
	//free(aux);//this is generating conflict on adding group to scene on cutter.cpp
}



































/////////////////////////////////////////first algorithms developed///////////////////////////////////////////////////////
///these algorithms use boolean substraction to do "slicing" with 6 planes
///Y slicing still needs to group pieces that are going to Ycut again with turn++
///Z slicing has basic slicing, still needs to look for all cases and recursive calls
///nice programming, but it was getting too delicate, lots of micromanaging pieces made it difficult to concieve and implement
///a new approach was take with boolean intersection, and the algorithm became more simple
///NOTE: the planes created are of use when defining which cubies are going to be rotating

//////////////////////////////////X SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////X SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////X SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void slicer::xSlicing(cutter &icutter, sgCObject *obj, int turn, int cubePart){
	//this is the implementation of the algorithm to produce the numPieces pieces of the "cube"
	//output: fills sgCGroup  **pieces  ,   with the pieces obtained 
	sgCGroup* sub1;

	float comparePos = 0;

	int ChCnt;
	sgCObject** allChilds;

	sgCGroup *tempGroup; //this isfirst going to be cut by Y
	sgCGroup *tempGroupToX;//tosend to be cut by X again

	//xslicing
	sgCObject **wantedObjects;
	int realNumberofObjects;
	sgCObject **toNextXSlicing;
	int realNumberofObjectstoNextXSlicing;



	//top layer, far left corner
	//make cut with x1, y1, z1 planes from cutter *cut

	if(turn == 1){
		//cut1: with x1
		sub1 = sgBoolean::Sub(*(sgC3DObject*)obj->Clone(),*(sgC3DObject*)icutter.x1->Clone());
		// make sure we have the elements to the left (-x) of the cut
		comparePos = icutter.transPlaneX1.x;
	}else if(turn == 2){
		//cut1: with x2
		sub1 = sgBoolean::Sub(*(sgC3DObject*)obj->Clone(),*(sgC3DObject*)icutter.x2->Clone());
		// make sure we have the elements to the left (-x) of the cut
		comparePos = icutter.transPlaneX2.x;
	}else if(turn == 3){
		//no cut left with planes X!!!
		//sub1 = sgBoolean::Sub(*(sgC3DObject*)obj->Clone(),*(sgC3DObject*)icutter.x2->Clone());
		//// make sure we have the elements to the left (-x) of the cut
		//comparePos = icutter.transPlaneX2.x;
	}

	if(sub1){//there was someting cut by X plane!!!!!!!!!!!!!!!!!!!!!!!!
		//know how many pieces we have after the sub operation
		ChCnt = sub1->GetChildrenList()->GetCount();
		allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		//allChilds[] will have all the objects in the sub1 group
		sub1->BreakGroup(allChilds);
		sgCObject::DeleteObject(sub1);

		//to save the objects that we want from this first cut
		//sgCObject **wantedObjects;
		wantedObjects = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		realNumberofObjects = 0;

		//to save object on the left of the planeX
		//this objects ge sent to XSlicing with the next x plane, turn ++
		toNextXSlicing = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		realNumberofObjectstoNextXSlicing = 0;

		for (int j=0; j < ChCnt; j++){
			//get triangles to check their position relative to plane x1
			sgC3DObject *aux = (sgC3DObject*)allChilds[j]->Clone();
			aux->Triangulate(SG_DELAUNAY_TRIANGULATION);
			const SG_ALL_TRIANGLES* trngls = aux->GetTriangles();
			SG_POINT *points =  trngls->allVertex;

			//now I have all the vertices of all the triangles of 1 object of the substraction
			for (int i=0; i < sizeof(*points) ; i ++){
				if (points[i].x < comparePos){
					//this is to the left of the plane x1
					//we want this object!!!

					//fill the array wantedObjects[] to build a sgCGroup with it 
					//the group  to be cut the second time by Y1 plane
					wantedObjects[realNumberofObjects] = aux->Clone(); 
					realNumberofObjects ++;

					// we just need one of the vertices to know on which side of the plain it is
					//because the objects will not be on boths sides or in the middle
					i = sizeof(*points);
				}else{
					//its on the right... so we send it to cutting turn 2 or 3
					//fill the array toNextXSlicing[] to build a sgCGroup with it 
					//the group  to be sent to cut by X2, on turn 2
					toNextXSlicing[realNumberofObjectstoNextXSlicing] = aux->Clone(); 
					realNumberofObjectstoNextXSlicing ++;

					i = sizeof(*points);
				}
			}
			//clean up objects
			sgCObject::DeleteObject(aux);
		}
		free(allChilds);

		///////////////////////////////////////////////////////////////// UNCOMENT THIS!! WHEN ALL IS DONE!! to make use of the code again
		//to save the group that is going for new cut with X, turn ++
		//tempGroupToX = sgCGroup::CreateGroup(toNextXSlicing,realNumberofObjectstoNextXSlicing);
		//free(toNextXSlicing);
		//realNumberofObjectstoNextXSlicing = 0;
		////send to new X slicing function with turn ++
		////pieces[0] = tempGroupToX;//this was to test it, to draw it.
		//xSlicing(*myCutter,tempGroupToX,turn++);
		//////////////////////////////////////////////////IT IS NOT WORKING RIGHT NOW!!!!! problems with copies of elements????

		///////////////////////////////////////////////////////////////////
		//to save the group that is going for the second cut with plane Y
		tempGroup = sgCGroup::CreateGroup(wantedObjects,realNumberofObjects);
		free(wantedObjects);
		realNumberofObjects = 0;
		//pieces[0] = tempGroup;//this was to test it, to draw it.
		ySlicing(*myCutter,tempGroup,1,turn);

	}
	else{
		//nothing was cut by x so piece is smaller and doesnt have parts on the left of x
		//this 3rd of the puzzle will be empty
		if(turn==1 && cubePart==1){
			pieces[0] = NULL;
			pieces[1] = NULL;
			pieces[2] = NULL;
			pieces[3] = NULL;
			pieces[4] = NULL;
			pieces[5] = NULL;
			pieces[6] = NULL;
			pieces[7] = NULL;
			pieces[8] = NULL;
			//send piece to be cut by x2
			//this same function but with turn ++
			xSlicing(*myCutter, obj, turn++,2);
		}else if (turn == 2 && cubePart==2){
			pieces[9] = NULL;
			pieces[10] = NULL;
			pieces[11] = NULL;
			pieces[12] = NULL;
			pieces[13] = NULL;
			pieces[14] = NULL;
			pieces[15] = NULL;
			pieces[16] = NULL;
			pieces[17] = NULL;
			//send piece to be cut by x3!! no x3
			//this same function but with turn ++
			xSlicing(*myCutter, obj, turn++,3);
		}else if (turn == 3 && cubePart==3){
			//if turn 3 there was no cut with X plane
			//there is nothing to cut!!
			pieces[18] = NULL;
			pieces[19] = NULL;
			pieces[20] = NULL;
			pieces[21] = NULL;
			pieces[22] = NULL;
			pieces[23] = NULL;
			pieces[24] = NULL;
			pieces[25] = NULL;
			pieces[26] = NULL;
		}
	}
}

//////////////////////////////////Y SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////Y SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////Y SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void slicer::ySlicing(cutter &icutter, sgCGroup *grp, int turn,int cubePart){

	sgCGroup* sub1;
	float comparePos = 0;

	int ChCnt=0;
	sgCObject** allChilds;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//to send to another ySlice function call
	//yes cut
	sgCObject **toNextYSlicing; 
	int realNumberofObjectstoNextYSlicing;
	sgCGroup **tempGroupToY;//to send to be cut by Y again
	int realNumObjectsToY = 0;
	//no cut
	sgCObject **toNextYSlicingnocut;
	int realNumberofObjectstoNextYSlicingnocut;
	sgCGroup **tempGroupToYnocut; //to send to be cut by Y again
	int realNumObjectsToYnocut =0;

	sgCGroup *realGroupToY; //this is the real group going to be cut by Y turn ++
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//wanted objects to send to Z cutting
	//yes cut
	sgCObject **wantedObjectsY; // wanted objects to be sent to be cut by Z
	int realNumberofObjectsY;
	sgCGroup **tempGroupToZ1; // to gather the pieces that will go to Z cutting in the YES cut section
	int realNumObjectsToZ1 = 0;
	//no cut
	sgCObject **wantedObjectsYnocut;
	int realNumberofObjectsYnocut;
	sgCGroup **tempGroupToZ1nocut; // to gather the pieces that will go to Z cutting in the NO CUT seccion
	int realNumObjectsToZ1nocut = 0;

	sgCGroup *realGroupToZ1; //this is the real group going to Z cut, it gathers all the tempGroupToZ1[]
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//memory allocaton!! for tempgroups
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//////take this group and cut it with Y(turn)
	//have to iterate through all the pieces of the group left of the previous cutting plzne X
	int ChCntCutX = grp->GetChildrenList()->GetCount();
	sgCObject**  allChildsCutX = (sgCObject**)malloc(ChCntCutX*sizeof(sgCObject*));
	grp->BreakGroup(allChildsCutX);
	//sgCObject::DeleteObject(grp); this has to be deleted??????

	////////////////////////////////////////////////////////////////////////////////////////////////
	//tempGroupToY to send to ySlicing turn ++
	tempGroupToY = (sgCGroup**)malloc(ChCntCutX*sizeof(sgCGroup*));
	tempGroupToYnocut = (sgCGroup**)malloc(ChCntCutX*sizeof(sgCGroup*));
	////////////////////////////////////////////////////////////////////////////////////////////////
	//tempGroupToZ1 will have maximum this number of pieces??
	tempGroupToZ1 = (sgCGroup**)malloc(ChCntCutX*sizeof(sgCGroup*));
	tempGroupToZ1nocut = (sgCGroup**)malloc(ChCntCutX*sizeof(sgCGroup*));

	////go through pieces on the left of X cut
	for (int i=0;i<ChCntCutX;i++){
		//////cut2: with Y planes
		if(turn == 1){
			sub1 = sgBoolean::Sub(*(sgC3DObject*)allChildsCutX[i]->Clone(),*(sgC3DObject*)icutter.y1->Clone());
			// make sure we have the elements to the left (-x) of the cut
			comparePos = icutter.transPlaneY1.y;
		}else if(turn == 2){
			sub1 = sgBoolean::Sub(*(sgC3DObject*)allChildsCutX[i]->Clone(),*(sgC3DObject*)icutter.y2->Clone());
			// make sure we have the elements to the left (-x) of the cut
			comparePos = icutter.transPlaneY2.y;
		}

		if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//know how many pieces we have after the sub operation
			ChCnt = sub1->GetChildrenList()->GetCount();
			allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			//allChilds[] will have all the objects in the sub1 group
			sub1->BreakGroup(allChilds);
			sgCObject::DeleteObject(sub1);

			//to save the objects that we want from this Y cut
			wantedObjectsY = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			realNumberofObjectsY = 0;

			//to save objects below the planeY
			//this objects get sent to ySlicing with the next y plane, turn ++
			toNextYSlicing = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			realNumberofObjectstoNextYSlicing = 0;

			//going through all the pieces of Y1 cut
			for (int j=0; j < ChCnt; j++){
				//get triangles to check their position relative to plane y1
				sgC3DObject *aux = (sgC3DObject*)allChilds[j]->Clone();
				aux->Triangulate(SG_DELAUNAY_TRIANGULATION);
				const SG_ALL_TRIANGLES* trngls = aux->GetTriangles();
				SG_POINT *points =  trngls->allVertex;

				//now I have all the vertices of all the triangles of 1 object of the substraction
				for (int i=0; i < sizeof(*points) ; i ++){
					if (points[i].y < comparePos){
						//this is above of the plane y1, because y increases down vertically on the screen
						//we want this object!!!

						//fill the array wantedObjects[] to build a sgCGroup with it 
						//the group  to be cut by plane Z
						wantedObjectsY[realNumberofObjectsY] = aux->Clone(); 
						realNumberofObjectsY ++;

						// we just need one of the vertices to know on which side of the plain it is
						//because the objects will not be on boths sides or in the middle
						i = sizeof(*points);
					}else{
						//its below the plane Yturn
						//group these pieces to be sent to be cut by Y turn++

						//to save object below of the plane Y
						//this objects get sent to ySlicing with the next y plane, turn ++
						toNextYSlicing[realNumberofObjectstoNextYSlicing] = aux->Clone(); 
						realNumberofObjectstoNextYSlicing ++;

						i = sizeof(*points);
					}
				}
				//clean up objects
				sgCObject::DeleteObject(aux);
			}
			free(allChilds);
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			//to save group that is going for another cut with Y plane
			tempGroupToY[i] = sgCGroup::CreateGroup(toNextYSlicing,realNumberofObjectstoNextYSlicing);
			free(toNextYSlicing);
			realNumObjectsToY += realNumberofObjectstoNextYSlicing;
			realNumberofObjectstoNextYSlicing = 0;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			//to save the group that is going for the cut with plane Z
			//if(realNumberofObjects > 0){
			//if we have objects above y1
			tempGroupToZ1[i] = sgCGroup::CreateGroup(wantedObjectsY,realNumberofObjectsY);
			//}else{
			//what if we dont have objects above y1??-> this means there was no cut// thisonly happens if the cutter thickness eats part of the object and the rest is on the goto Y zone!!
			//tempGroupToZ1[i] = NULL
			//}
			free(wantedObjectsY);
			realNumObjectsToZ1 += realNumberofObjectsY;
			realNumberofObjectsY = 0;
		}
		else{
			////////////////////////////////////////////////////////////////////////////////////////////////
			///allocate memory for possible wanted&nextY pieces if there are no cuts by Y plane
			wantedObjectsYnocut = (sgCObject**)malloc(ChCntCutX*sizeof(sgCObject*));
			realNumberofObjectsYnocut =0 ;
			toNextYSlicingnocut = (sgCObject**)malloc(ChCntCutX*sizeof(sgCObject*));
			realNumberofObjectstoNextYSlicingnocut = 0;

			//nothing was cut by Y PLANE!!!
			//this piece could not be in contact with the Y plane, 
			//but it can still be in the desired area
			//ask for .y  < comparePos 
			sgC3DObject *aux = (sgC3DObject*)allChildsCutX[i]->Clone();
			aux->Triangulate(SG_DELAUNAY_TRIANGULATION);
			const SG_ALL_TRIANGLES* trngls = aux->GetTriangles();
			SG_POINT *points =  trngls->allVertex;

			//now I have all the vertices of all the triangles of 1 object
			for (int i=0; i < sizeof(*points) ; i ++){
				if (points[i].y < comparePos){
					//this is above of the plane y1, because y increases down vertically on the screen
					//we want this object!!!

					//fill the array wantedObjectsY[] to build a sgCGroup with it 
					//the group  to be cut plane Z1
					wantedObjectsYnocut[realNumberofObjectsYnocut] = aux->Clone(); 
					realNumberofObjectsYnocut ++;

					// we just need one of the vertices to know on which side of the plain it is
					//because the objects will not be on boths sides or in the middle
					i = sizeof(*points);
				}else{
					//its below the plane Yturn
					//group these pieces to be sent to be cut by Y turn++

					//to save object below of the plane Y
					//this objects get sent to ySlicing with the next y plane, turn ++
					toNextYSlicingnocut[realNumberofObjectstoNextYSlicingnocut] = aux->Clone(); 
					realNumberofObjectstoNextYSlicingnocut ++;

					i = sizeof(*points);
				}
			}
			//at this point we have pieces that were not cut, but that have to go to next Y cutting or to Z cutting
			//they have to be unified to the groups on the YES CUT side of this funciton
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			//to save group that is going for another cut with Y plane
			tempGroupToYnocut[i] = sgCGroup::CreateGroup(toNextYSlicingnocut,realNumberofObjectstoNextYSlicingnocut);
			free(toNextYSlicingnocut);
			realNumObjectsToYnocut += realNumberofObjectstoNextYSlicingnocut;
			realNumberofObjectstoNextYSlicingnocut = 0;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			//to save the group that is going for the cut with plane Z from NO CUT area
			//if(realNumberofObjects > 0){
			//if we have objects above y1
			tempGroupToZ1nocut[i] = sgCGroup::CreateGroup(wantedObjectsYnocut,realNumberofObjectsYnocut);
			//}else{
			//what if we dont have objects above y1??-> this means there was no cut// thisonly happens if the cutter takes part of the object !!
			//tempGroupToZ1[i] = NULL
			//}
			free(wantedObjectsYnocut);
			realNumObjectsToZ1nocut += realNumberofObjectsYnocut;
			realNumberofObjectsYnocut = 0;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}  
	free(allChildsCutX); 
	//if we have pieces to send to be cut by Z
	if((realNumObjectsToZ1 != 0) || (realNumObjectsToZ1nocut != 0)){
		//////////////////////////////MAKE GROUP TO SEND TO BE CUT BY Z ///////////////////////////////////////////
		//after having iterated all of the pieces from the X cut and having made the cut with Y plane
		//we have all the pieces we want from the Y cut in: tempGroupToZ1[] // we have ChCntCutx of these []'s
		//we have the number of pieces in total in: realNumObjectsToZ1
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//unify temp groups[] of pieces to be sent to be cut by Z
		sgCObject  *objcts[50]; //50 to be on the safe side
		int z=0;
		//break each tempGroupToZ1[]
		//ask if tempGroupToZ1[] was ever used
		if(realNumObjectsToZ1 != 0){
			for (int o=0;o<ChCntCutX;o++){
				ChCnt = tempGroupToZ1[o]->GetChildrenList()->GetCount();
				allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
				tempGroupToZ1[o]->BreakGroup(allChilds);
				//get each object inside each group, into an [], to make this a new, unified group
				for (int j=0; j < ChCnt; j++){
					objcts[z] = allChilds[j];
					z ++;//to save objcts correctly, and know exactly how many objects is the final group going to have
				}
			}
			free(allChilds);
		}
		//break each tempGroupToZ1nocut[]
		//ask if tempGroupToZ1nocut[] was ever used
		if(realNumObjectsToZ1nocut != 0){
			for (int o=0;o<ChCntCutX;o++){
				ChCnt = tempGroupToZ1nocut[o]->GetChildrenList()->GetCount();
				allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
				tempGroupToZ1nocut[o]->BreakGroup(allChilds);
				//get each object inside each group, into an [], to make this a new, unified group
				for (int j=0; j < ChCnt; j++){
					objcts[z] = allChilds[j];
					z ++;//to save objcts correctly, and know exactly how many objects is the final group going to have
				}
			}
			free(allChilds);
		}

		//create group to be sent to z1 :: every object that has to be sent to be cut by Z is on objcts[]!!!!
		realGroupToZ1  = sgCGroup::CreateGroup(objcts,z);//realNumObjectsToZ1); //z is better since we are unifying temp[]'s z should be = realNumObjectsToZ1 + realNumObjectsToZ1nocut
		//clean objects
		//sgCObject::DeleteObject(*tempGroupToZ1);
		//sgCObject::DeleteObject(*objcts);

		///////////////////////////////////////////////////////////////////////
		////for testing purposes...////////////////////////////////////////////
		//break this group and draw it
		//ChCnt = realGroupToZ1->GetChildrenList()->GetCount();
		//sgCObject** temp = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		//realGroupToZ1->BreakGroup(temp);
		/*pieces[0] = realGroupToZ1;*/
		///////////////////////////////////////////////////////////////////////

		//send to zSlicing
		zSlicing(*myCutter,realGroupToZ1,turn,turn);

	}else{
		//no pieces to be sent to be cut by Z!!!
		//this means the colum to the left of x1 and above y1 is going to be empty, it doesn't have any objects on that volume of space
		if(turn==1 && cubePart==1){
			pieces[0] = NULL;
			pieces[3] = NULL;
			pieces[6] = NULL;
		}else if(turn==2 && cubePart==1){
			pieces[1] = NULL;
			pieces[4] = NULL;
			pieces[7] = NULL;
		}else if(turn==3 && cubePart==1){
			pieces[2] = NULL;
			pieces[5] = NULL;
			pieces[8] = NULL;
		}else if(turn==1 && cubePart==2){
			pieces[9] = NULL;
			pieces[12] = NULL;
			pieces[15] = NULL;
		}else if(turn==2 && cubePart==2){
			pieces[10] = NULL;
			pieces[13] = NULL;
			pieces[16] = NULL;
		}else if(turn==3 && cubePart==2){
			pieces[11] = NULL;
			pieces[14] = NULL;
			pieces[17] = NULL;
		}else if(turn==1 && cubePart==3){
			pieces[18] = NULL;
			pieces[21] = NULL;
			pieces[24] = NULL;
		}else if(turn==2 && cubePart==3){
			pieces[19] = NULL;
			pieces[22] = NULL;
			pieces[25] = NULL;
		}else if(turn==3 && cubePart==3){
			pieces[20] = NULL;
			pieces[23] = NULL;
			pieces[26] = NULL;
		}
	}

}
//////////////////////////////////z SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////z SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////z SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void slicer::zSlicing(cutter &icutter, sgCGroup *grp, int turn,int cubePart){
	pieces[0] = grp;
	int as;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////take this group and cut it with Z1
	////////cut2: with y1
	//sub1 = sgBoolean::Sub(*(sgC3DObject*)obj->Clone(),*(sgC3DObject*)icutter.z1->Clone());
	//// make sure we have the elements to the left (-x) of the cut
	//comparePos = icutter.transPlaneZ1.z;

	//if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!have to know what o do when there is no cut!!!!
	//	//know how many pieces we have after the sub operation
	//	ChCnt = sub1->GetChildrenList()->GetCount();
	//	allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	//	//allChilds[] will have all the objects in the sub1 group
	//	sub1->BreakGroup(allChilds);
	//	sgCObject::DeleteObject(sub1);

	//	//to save the objects that we want from this first cut
	//	//sgCObject **wantedObjects;
	//	wantedObjects = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	//	realNumberofObjects = 0;

	//	for (int j=0; j < ChCnt; j++){
	//		//get triangles to check their position relative to plane y1
	//		sgC3DObject *aux = (sgC3DObject*)allChilds[j]->Clone();
	//		aux->Triangulate(SG_DELAUNAY_TRIANGULATION);
	//		const SG_ALL_TRIANGLES* trngls = aux->GetTriangles();
	//		SG_POINT *points =  trngls->allVertex;

	//		//now I have all the vertices of all the triangles of 1 object of the substraction
	//		for (int i=0; i < sizeof(*points) ; i ++){
	//			if (points[i].z > comparePos){
	//				//this is above of the plane Z1, because it increases outside of the screen
	//				//we want this object!!!

	//				//fill the array wantedObjects[] to build a sgCGroup with it 
	//				//the group  to be cut the third time by plane Z1
	//				wantedObjects[realNumberofObjects] = aux->Clone(); 
	//				realNumberofObjects ++;

	//				// we just need one of the vertices to know on which side of the plain it is
	//				//because the objects will not be on boths sides or in the middle
	//				i = sizeof(*points);
	//			}else{
	//				//its below the plane z1
	//				i = sizeof(*points);
	//				//send this pieces to other cutting functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//			}
	//		}
	//		//clean up objects
	//		sgCObject::DeleteObject(aux);
	//	}
	//	free(allChilds);
	//}
	/////////////////////////////////////////////////////////////////////FIRST CUBIE!!!!!!!!!!!!!!!
	////to save the group that is going TO BE THE FIRST CUBIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//tempGroup = sgCGroup::CreateGroup(wantedObjects,realNumberofObjects);
	//free(wantedObjects);
	//realNumberofObjects = 0;
	////pieces[0]= tempGroup;//this was to test it, to draw it.
}