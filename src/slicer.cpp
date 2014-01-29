#include "slicer.h"
#include "sgCore.h"
#include "cutter.h"

slicer::slicer(cutter *c){
	myCutter = c;
	pieces = (sgCGroup**)malloc(27*sizeof(sgCObject*));
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
//////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
sgCGroup** slicer::getPieces(){
	return pieces;
}
//----------------------------------------------------------------
void slicer::xSlicing(cutter &icutter, sgCObject *obj, int turn){
	//this is the implementation of the algorithm to produce the 27 pieces of the "cube"
	//output: fills sgCGroup  **pieces  ,   with the pieces obtained 
	sgCGroup* sub1;

	float comparePos = 0;

	int ChCnt;
	sgCObject** allChilds;

	sgCGroup *tempGroup; //this isfirst going to be cut by Y
	sgCGroup *tempGroupToX;//tosend to be cut by X again


	//it will make three cuts to get each "piece"
	for(int i=1 ; i<2; i++){//this has to go out!!!
		// 1 -> 9 are the top cubies
		if(i==1){
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
				if(turn==1){
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
					xSlicing(*myCutter, obj, turn++);
				}else if (turn == 2){
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
					xSlicing(*myCutter, obj, turn++);
				}else{
					//if turn 3 there was no cut with X plane
					/*pieces[18] = NULL;
					pieces[19] = NULL;
					pieces[20] = NULL;
					pieces[21] = NULL;
					pieces[22] = NULL;
					pieces[23] = NULL;
					pieces[24] = NULL;
					pieces[25] = NULL;
					pieces[26] = NULL;*/
				}

			}





			/////THIS HAS TO BE PUT INSIDE THE FIRST SUCCESFUL CUT WITH X1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


		}else if(i==2){
		}else if(i==3){
		}else if(i==4){
		}else if(i==5){
		}else if(i==6){
		}else if(i==7){
		}else if(i==8){
		}else if(i==9){
		}
		//10 -> 18 are the middle cubies
		else if(i==10){
		}else if(i==11){
		}else if(i==12){
		}else if(i==13){
		}else if(i==14){
		}else if(i==15){
		}else if(i==16){
		}else if(i==17){
		}else if(i==18){
		}
		//19 -> 27 are the botom cubies
		else if(i==19){
		}else if(i==20){
		}else if(i==21){
		}else if(i==22){
		}else if(i==23){
		}else if(i==24){
		}else if(i==25){
		}else if(i==26){
		}else if(i==27){
		}
	}

}

//////////////////////////////////Y SLICING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void slicer::ySlicing(cutter &icutter, sgCGroup *grp, int turn,int cubePart){

	sgCGroup* sub1;
	float comparePos = 0;

	int ChCnt=0;
	sgCObject** allChilds;

	sgCGroup *tempGroup; //this isfirst going to be cut by Y
	//sgCGroup **tempGroupToY;//tosend to be cut by X again
	int realNumberofObjectstoNextYSlicing=0;

	sgCGroup **tempGroupToZ1; // to gather the pieces that will go to Z cutting
	int realNumObjectsToZ1 = 0;
	sgCGroup *realGroupToZ1; //this is the real group going to Z cut, it gathers all the tempGroupToZ1[]

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//////take this group and cut it with Y(turn)
	//have to iterate through all the pieces of the group left of the previous cutting plzne X
	int ChCntCutX = grp->GetChildrenList()->GetCount();
	sgCObject**  allChildsCutX = (sgCObject**)malloc(ChCntCutX*sizeof(sgCObject*));
	grp->BreakGroup(allChildsCutX);
	//sgDeleteObject(grp); this has to be deleted??????

	////////////////////////////////////////////////////////////////////////////////////////////////
	//tempGroupToZ1 will have maximum this number of pieces??
	tempGroupToZ1 = (sgCGroup**)malloc(ChCntCutX*sizeof(sgCGroup*));

	////go through pieces on the left of X cut
	for (int i=0;i<ChCntCutX;i++){
		//////cut2: with Y planes

		//if(turnY ==1){
		sub1 = sgBoolean::Sub(*(sgC3DObject*)allChildsCutX[i]->Clone(),*(sgC3DObject*)icutter.y1->Clone());
		// make sure we have the elements to the left (-x) of the cut
		comparePos = icutter.transPlaneY1.y;
		//}else if(turnY==2){
		//	sub1 = sgBoolean::Sub(*(sgC3DObject*)allChildsCutX[i]->Clone(),*(sgC3DObject*)icutter.y2->Clone());
		//	// make sure we have the elements to the left (-x) of the cut
		//	comparePos = icutter.transPlaneY2.y;
		//}

		if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//know how many pieces we have after the sub operation
			ChCnt = sub1->GetChildrenList()->GetCount();
			allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			//allChilds[] will have all the objects in the sub1 group
			sub1->BreakGroup(allChilds);
			sgCObject::DeleteObject(sub1);

			//to save the objects that we want from this second cut with Y1
			//sgCObject **wantedObjects;
			wantedObjectsY = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			realNumberofObjectsY = 0;

			//to save object on the left of the planeY
			//this objects ge sent to XSlicing with the next y plane, turn ++
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
						//the group  to be cut the third time by plane Z1
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
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			//to save the group that is going for the third cut with plane Z1
			//if(realNumberofObjects > 0){
			//if we have objects above y1
			tempGroupToZ1[i] = sgCGroup::CreateGroup(wantedObjectsY,realNumberofObjectsY);
			//}else{
			//what if we dont have objects above y1??-> this means there was no cut// thisonly happens if the cutter takes part of the object !!
			//tempGroupToZ1[i] = NULL
			//}
			free(wantedObjectsY);
			realNumObjectsToZ1 += realNumberofObjectsY;
			realNumberofObjectsY = 0;
		}
		else{
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
					//send this pieces to other cutting functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
			}
		}
	}  
	free(allChildsCutX); 
	//after having iterated all of the pieces from the X cut
	//we have all the pieces we want from the Y cut in: tempGroupToZ1[] // we have ChCntCutx1 of these
	//we have hte number of pieces in total in: realNumObjectsToZ1

	//add each of this obj to sgCObject*   objcts[realNumObjectsToZ1]; 
	sgCObject  *objcts[500];
	int z=0;
	//break each tempGroupToZ1[]
	for (int o=0;o<ChCntCutX;o++){
		ChCnt = tempGroupToZ1[o]->GetChildrenList()->GetCount();
		allChilds = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		tempGroupToZ1[o]->BreakGroup(allChilds);

		for (int j=0; j < ChCnt; j++){
			objcts[z] = allChilds[j];
			z ++;//to save objcts correctly
		}
	}
	free(allChilds);
	//create group to be sent to z1
	realGroupToZ1  = sgCGroup::CreateGroup(objcts,realNumObjectsToZ1);
	//clean objects
	//sgCObject::DeleteObject(*tempGroupToZ1);
	//sgCObject::DeleteObject(*objcts);


	///////////////////////////////////////////////////////////////////////
	////for testing purposes...////////////////////////////////////////////
	//break this group and draw it
	//ChCnt = realGroupToZ1->GetChildrenList()->GetCount();
	//sgCObject** temp = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	//realGroupToZ1->BreakGroup(temp);
	pieces[0] = realGroupToZ1;
	///////////////////////////////////////////////////////////////////////

}