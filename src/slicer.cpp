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
void slicer::simpleSlicing(cutter &icutter, sgCObject *obj, int turn){
	//this is the implementation of the algorithm to produce the 27 pieces of the "cube"
	//output: fills sgCGroup  **pieces  ,   with the pieces obtained 
	sgCGroup* sub1;

	float comparePos = 0;

	int ChCnt;
	sgCObject** allChilds;

	sgCGroup *tempGroup;

	//it will make three cuts to get each "piece"
	for(int i=1 ; i<28; i++){//this has to go out!!!
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

			if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!have to know what o do when there is no cut!!!!
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
							i = sizeof(*points);
							//send this pieces to other cutting functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							/*if(turn == 1){
								simpleSlicing(*myCutter, aux->Clone(), 2);
							}else if(turn ==2){
								simpleSlicing(*myCutter, aux->Clone(), 3);
							}*/
						}
					}
					//clean up objects
					sgCObject::DeleteObject(aux);
				}
				free(allChilds);
            }
			///////////////////////////////////////////////////////////////////
			//to save the group that is going for the second cut with plane Y1
			tempGroup = sgCGroup::CreateGroup(wantedObjects,realNumberofObjects);
			//free(wantedObjects);
			realNumberofObjects = 0;
			//pieces[0]= tempGroup;//this was to test it, to draw it.

			//////take this group and cut it with y1
			//////cut2: with y1
			sub1 = sgBoolean::Sub(*(sgC3DObject*)wantedObjects[0],*(sgC3DObject*)icutter.y1->Clone());
			// make sure we have the elements to the left (-x) of the cut
			comparePos = icutter.transPlaneY1.y;
			free(wantedObjects);

			if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!have to know what o do when there is no cut!!!!
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
							wantedObjects[realNumberofObjects] = aux->Clone(); 
							realNumberofObjects ++;

							// we just need one of the vertices to know on which side of the plain it is
							//because the objects will not be on boths sides or in the middle
							i = sizeof(*points);
						}else{
							//its below the plane Y1
							i = sizeof(*points);
							//send this pieces to other cutting functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						}
					}
					//clean up objects
					sgCObject::DeleteObject(aux);
				}
				free(allChilds);
			}

			///////////////////////////////////////////////////////////////////
			//to save the group that is going for the third cut with plane Z1
			tempGroup = sgCGroup::CreateGroup(wantedObjects,realNumberofObjects);
			free(wantedObjects);
			realNumberofObjects = 0;
			pieces[0]= tempGroup;//this was to test it, to draw it.

			//////take this group and cut it with Z1
			//////cut2: with y1
			sub1 = sgBoolean::Sub(*(sgC3DObject*)obj->Clone(),*(sgC3DObject*)icutter.z1->Clone());
			// make sure we have the elements to the left (-x) of the cut
			comparePos = icutter.transPlaneZ1.z;

			if(sub1){//there was someting cut !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!have to know what o do when there is no cut!!!!
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

				for (int j=0; j < ChCnt; j++){
					//get triangles to check their position relative to plane y1
					sgC3DObject *aux = (sgC3DObject*)allChilds[j]->Clone();
					aux->Triangulate(SG_DELAUNAY_TRIANGULATION);
					const SG_ALL_TRIANGLES* trngls = aux->GetTriangles();
					SG_POINT *points =  trngls->allVertex;

					//now I have all the vertices of all the triangles of 1 object of the substraction
					for (int i=0; i < sizeof(*points) ; i ++){
						if (points[i].z > comparePos){
							//this is above of the plane Z1, because it increases outside of the screen
							//we want this object!!!

							//fill the array wantedObjects[] to build a sgCGroup with it 
							//the group  to be cut the third time by plane Z1
							wantedObjects[realNumberofObjects] = aux->Clone(); 
							realNumberofObjects ++;

							// we just need one of the vertices to know on which side of the plain it is
							//because the objects will not be on boths sides or in the middle
							i = sizeof(*points);
						}else{
							//its below the plane z1
							i = sizeof(*points);
							//send this pieces to other cutting functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						}
					}
					//clean up objects
					sgCObject::DeleteObject(aux);
				}
				free(allChilds);
			}
			///////////////////////////////////////////////////////////////////FIRST CUBIE!!!!!!!!!!!!!!!
			//to save the group that is going TO BE THE FIRST CUBIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			tempGroup = sgCGroup::CreateGroup(wantedObjects,realNumberofObjects);
			free(wantedObjects);
			realNumberofObjects = 0;
			//pieces[0]= tempGroup;//this was to test it, to draw it.


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