#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"

cubie::cubie(float x, float y,float z, int idi){
	objects = NULL;
	id = idi;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	rot.x = 0.0;
	rot.y = 0.0;
	rot.z = 0.0;
	color = rand()%27;
	okDraw = true;
	pointRotate.x = 0;
	pointRotate.y = 0;
	pointRotate.z = 0;
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;
	moving = false;
	movingXC = false;
	movingXCC = false;
	movingYC = false;
	movingYCC = false;
	movingZC = false;
	movingZCC = false;
	ct1 = 0.0;
	ct2 = 0.0;
	animTime = 300;
	tempDeg = 0.0;

	sample = true;
	rotXa = 0.0;
	SG_POINT axis = {0,0,0};
	SG_VECTOR vrotFace = {0,0,0};
	myMatrix.push_back(matrix(axis,tempDeg,true));
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){
	//if(numObjs>0){
	if(moving==true){
		for (int j=0; j < numObjs; j++){
			//		//animate rotation xc
			//		if(movingXC == true){
			//			if(rotX < tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotX += (ct2 - ct1)*((1.57)/animTime);
			//				ct1 = ct2;
			//			}else{
			//				rotX = tempDeg;
			//				/*if(rotX >= 360){
			//				rotX = 0.0;
			//				}*/
			//				if(rotX >= 6.28){
			//					rotX = 0.0;
			//				}
			//				moving = false;
			//				movingXC = false;
			//			}
			//		}
			//		//xcc
			//		if(movingXCC == true){
			//			if(rotX > tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotX -= (ct2 - ct1)*((1.57)/animTime);
			//				ct1 = ct2;
			//			}else{
			//				rotX = tempDeg;
			//				/*if(rotX <= -360){
			//				rotX = 0.0;
			//				}*/
			//				if(rotX <= -6.28){
			//					rotX = 0.0;
			//				}
			//				moving = false;
			//				movingXCC = false;
			//			}
			//		}
			//		////////////   Y
			//		//animate rotation yc
			//		if(movingYC == true){
			//			if(rotY < tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotY += (ct2 - ct1)*((1.57)/animTime);
			//				ct1 = ct2;
			//			}else{
			//				rotY = tempDeg;
			//				/*if(rotY >= 360){
			//				rotY = 0.0;
			//				}*/
			//				if(rotY >= 6.28){
			//					rotY = 0.0;
			//				}
			//				moving = false;
			//				movingYC = false;
			//			}
			//		}
			//		//ycc
			//		if(movingYCC == true){
			//			if(rotY > tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotY -= (ct2 - ct1)*((1.57)/animTime);
			//				ct1 = ct2;
			//			}else{
			//				rotY = tempDeg;
			//				/*if(rotY <= -360){
			//				rotY = 0.0;
			//				}*/
			//				if(rotY <= -6.28){
			//					rotY = 0.0;
			//				}
			//				moving = false;
			//				movingYCC = false;
			//			}
			//		}
			//		////////////  Z
			//		//animate rotation zc
			//		if(movingZC == true){
			//			if(rotZ < tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotZ += (ct2 - ct1)*((1.57)/animTime);
			//				ct1 = ct2;
			//			}else{
			//				rotZ = tempDeg;
			//				/*if(rotZ >= 360){
			//				rotZ = 0.0;
			//				}*/
			//				if(rotZ >= 6.28){
			//					rotZ = 0.0;
			//				}
			//				moving = false;
			//				movingZC = false;
			//			}
			//		}
			//		//zcc
			//		if(movingZCC == true){
			//			if(rotZ > tempDeg){
			//				ct2 = ofGetElapsedTimeMillis();
			//				rotZ -= (ct2 - ct1)*((1.57)/animTime); //////////////////////90 on OF rotations
			//				ct1 = ct2;
			//			}else{
			//				rotZ = tempDeg;
			//				/*if(rotZ <= -360){
			//				rotZ = 0.0;
			//				}*/
			//				if(rotZ <= -6.28){
			//					rotZ = 0.0;
			//				}
			//				moving = false;
			//				movingZCC = false;
			//			}
			//		}


			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			if(myMatrix.size()<2){
				for(int i=0; i<myMatrix.size();i++){
					SG_POINT protFace = {0,0,0};										 
					SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
					float d = myMatrix.at(i).deg;
					if(myMatrix.at(i).dir == true){
						//c
						if(myMatrix.at(i).vector.x == 1){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else{
							//rotZ = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}
					}else{
						//cc
						if(myMatrix.at(i).vector.x == 1){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else{
							//rotZ = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}
					}
					//objectList[j]->ApplyTempMatrix();
					//objectList[j]->DestroyTempMatrix();
				}
			}
			if(myMatrix.size()>=2){
				//build rotation matrix for all steps up to the one where it was at the moment of a new movement
				for(int i=0; i<myMatrix.size()-1;i++){
					SG_POINT protFace = {0,0,0};										 
					SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
					float d = myMatrix.at(i).deg;
					if(myMatrix.at(i).dir == true){
						//c
						if(myMatrix.at(i).vector.x == 1){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else{
							//rotZ = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}
					}else{
						//cc
						if(myMatrix.at(i).vector.x == 1){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}else{
							//rotZ = d;
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}
					}
					//objectList[j]->ApplyTempMatrix();
					//objectList[j]->DestroyTempMatrix();
				}
				//we are at the last positon
				SG_POINT protFace = {0,0,0};										 
				SG_VECTOR vrotFace = myMatrix.at(myMatrix.size()-1).vector;//  axis; //rotate to do a face move
				double tempDeg2 = myMatrix.at(myMatrix.size()-1).deg;
				if(sample==false){
					//this should only be sampled once during the animation
					rotXa = myMatrix.at(myMatrix.size()-2).deg;
					sample=true;
				}
				if(myMatrix.at(myMatrix.size()-1).dir == true){
					//animate rotation xc
					if(movingXC == true){
						if(rotXa < tempDeg2){
							ct2 = ofGetElapsedTimeMillis();
							rotXa += 0.1;//(ct2 - ct1)*((1.57)/animTime);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,rotXa);
							ct1 = ct2;
						}else{
							rotX = tempDeg2;
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,rotX);
							/*if(rotX >= 360){
							rotX = 0.0;
							}*/
							if(rotX >= 6.28){
								rotX = 0.0;
							}
							moving = false;
							movingXC = false;
						}
					}
				}else{
					//xcc
					if(movingXCC == true){
						if(rotXa > tempDeg2){
							ct2 = ofGetElapsedTimeMillis();
							rotXa -= 0.1;//(ct2 - ct1)*((1.57)/animTime);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,rotXa);
							ct1 = ct2;
						}else{
							rotX = tempDeg2;
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,rotX);
							/*if(rotX <= -360){
							rotX = 0.0;
							}*/
							if(rotX <= -6.28){
								rotX = 0.0;
							}
							moving = false;
							movingXCC = false;
						}
					}
				}
			}

			///////////////////////////////////////////////////////////
			////////rotate and move with the whole puzzle
			SG_VECTOR vrotH = {0,1,0}; //rotate H                            
			if (objectList[j]->GetTempMatrix()==0){
				objectList[j]->InitTempMatrix()->Rotate(pointRotate,vrotH,rot.y);
			}else{
				objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotH,rot.y);
			}
			SG_VECTOR vrotV = {1,0,0}; //rotate V								 
			objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotV,rot.x);
			//translations
			SG_VECTOR transBox11 = {pos.x,pos.y,pos.z}; 
			objectList[j]->GetTempMatrix()->Translate(transBox11);
			objectList[j]->ApplyTempMatrix();
		}
	}
	//}
	else{
		//puzzle is not moving
		//if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			for(int i=0; i<myMatrix.size();i++){
				SG_POINT protFace = {0,0,0};										 
				SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
				float d = myMatrix.at(i).deg;
				if(myMatrix.at(i).dir == true){
					//c
					if(myMatrix.at(i).vector.x == 1){
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}else if(myMatrix.at(i).vector.y == 1){
						//rotY = d;
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}else{
						//rotZ = d;
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
				}else{
					//cc
					if(myMatrix.at(i).vector.x == 1){
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}else if(myMatrix.at(i).vector.y == 1){
						//rotY = d;
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}else{
						//rotZ = d;
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
				}
				//objectList[j]->ApplyTempMatrix();
				//objectList[j]->DestroyTempMatrix();
			}
			/////////////////////////////////////////////////////////////////	 
			//SG_VECTOR vrotFaceX ={1,0,0};
			//SG_VECTOR vrotFaceY ={0,1,0};
			//SG_VECTOR vrotFaceZ ={0,0,1};
			//objectList[j]->InitTempMatrix()->Rotate(pointRotate,vrotFaceX,rotX);
			//objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotFaceY,rotY);
			//objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotFaceZ,rotZ);
			///////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////
			////////rotate and move with the whole puzzle
			SG_VECTOR vrotH = {0,1,0}; //rotate H                            
			if (objectList[j]->GetTempMatrix()==0){
				objectList[j]->InitTempMatrix()->Rotate(pointRotate,vrotH,rot.y);
			}else{
				objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotH,rot.y);
			}
			SG_VECTOR vrotV = {1,0,0}; //rotate V								 
			objectList[j]->GetTempMatrix()->Rotate(pointRotate,vrotV,rot.x);
			//translations
			SG_VECTOR transBox11 = {pos.x,pos.y,pos.z}; 
			objectList[j]->GetTempMatrix()->Translate(transBox11);
			objectList[j]->ApplyTempMatrix();


			//	//objectList[j]->Triangulate(SG_VERTEX_TRIANGULATION);
			//	//objectList[j]->DestroyTempMatrix();
			//	//objectList[j]->SetAttribute(SG_OA_COLOR,color);
			//	//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_FULL);
			//	//sgGetScene()->AttachObject(objectList[j]);
			//}
		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	//now it draws its vector of Vbos, myVbos

	// it attaches to scene
	//now its not attaching to scene

	//use this cubies objectList[] to draw elements without ever loosing them on groupBreaking on undraw
	//now there is no group breaking, there will be no need for an undraw

	//if(okDraw == true){
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			glPushMatrix();
			////all puzzle
			//glTranslatef(posX,posY,posZ);
			//glRotatef(rotH,0,1,0);
			//glRotatef(rotV,1,0,0);
			////this cubie
			//glRotatef(rotX,1,0,0);
			//glRotatef(rotY,0,1,0);
			//glRotatef(rotZ,0,0,1);
			if (objectList[j]->GetTempMatrix()!=0)
				glMultMatrixd(objectList[j]->GetTempMatrix()->GetTransparentData());
			objectList[j]->DestroyTempMatrix();
			myVbos[j].draw(GL_TRIANGLES, 0,myMeshs[j].getNumIndices());
			glPopMatrix();
		}
	}
	//	okDraw = false;
	//}

}
//--------------------------------------------------------------
//void cubie::unDraw(){  
//sgGetScene()->Clear();
//detach from scene
//if(objects != NULL){
//	for (int j=0; j < numObjs; j++){
//		//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_HIDE);
//		//sgGetScene()->DetachObject(objectList[j]);
//		//sgCObject::DeleteObject(objectList[j]);
//		okDraw = true;
//	}
//	////////////////////////////////////IMPORTANT!!!!!!!!///////////////////////////////////
//	////remake objectList for next drawing
//	////there is somememory leakage here!!!!! even after sgCObject::DeleteObject(objectList[j]);
//	/////////////////////////////////////////////////////////////////////////////////////////////

//	sgCGroup *objects2 = copyObjects(); 
//	const int ChCnt = objects2->GetChildrenList()->GetCount();
//	numObjs = ChCnt;
//	//give this cubies list some memory
//	objectList = (sgC3DObject**)malloc(ChCnt*sizeof(sgC3DObject*));
//	//start breaking incoming group
//	sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
//	objects2->BreakGroup(allParts);
//	sgCObject::DeleteObject(objects2);

//	for (int j=0; j < ChCnt; j++){
//		objectList[j] = (sgC3DObject*)allParts[j];
//	}
//	free(allParts);
//}else{
//	//cout << "null at undraw" << endl;
//}
//}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		//if(moving == false){
		for (int j=0; j < numObjs; j++){
			SG_POINT protFace = {0,0,0};
			SG_VECTOR vrotFace = axis;
			/*rotX=0;
			rotY=0;
			rotZ=0;*/
			moving = true;
			tempDeg = 0.0;
			ct1 = ofGetElapsedTimeMillis();
			if(di == true){
				//c
				if(axis.x == 1){
					movingXC = true;
					tempDeg = rotX;
					tempDeg += 1.57;
					sample = false;
					/*rotX += 1.57;
					if(rotX >= 6.28){
					rotX = 0.0;
					}*/

					/*movingXC = true;
					tempDeg = rotX;
					tempDeg += 90;*/

					/*rotX += 90;
					if(rotX >= 360){
					rotX = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotX);
					//build matrix and add it to the vector myMatrix
					//tempDeg = tempDeg/100;
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << "rotX: " << tempDeg << endl;
				}else if(axis.y == 1){
					movingYC = true;
					tempDeg = rotY;
					tempDeg += 1.57;

					/*rotY += 1.57;
					if(rotY >= 6.28){
					rotY = 0.0;
					}*/

					/*movingYC = true;
					tempDeg = rotY;
					tempDeg += 90;*/

					/*rotY += 90;
					if(rotY >= 360){
					rotY = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotY);
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << endl;
				}else{
					movingZC = true;
					tempDeg = rotZ;
					tempDeg += 1.57;

					/*rotZ += 1.57;
					if(rotZ >= 6.28){
					rotZ = 0.0;
					}*/

					/*movingZC = true;
					tempDeg = rotZ;
					tempDeg += 90;*/

					/*rotZ += 90;
					if(rotZ >= 360){
					rotZ = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotZ);
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << endl;
				}
			}else{
				//cc
				if(axis.x == 1){
					movingXCC = true;
					tempDeg = rotX;
					tempDeg -= 1.57;
					sample = false;
					/*rotX -= 1.57;
					if(rotX <= -6.28){
					rotX = 0.0;
					}*/

					/*ovingXCC = true;
					tempDeg = rotX;
					tempDeg -= 90;*/

					/*rotX -= 90;
					if(rotX <= -360){
					rotX = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotX);
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << "rotX: " << tempDeg << endl;
				}else if(axis.y == 1){
					movingYCC = true;
					tempDeg = rotY;
					tempDeg -= 1.57;

					/*rotY -= 1.57;
					if(rotY <= -6.28){
					rotY = 0.0;
					}*/

					/*movingYCC = true;
					tempDeg = rotY;
					tempDeg -= 90;*/

					/*rotY -= 90;
					if(rotY <= -360){
					rotY = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotY);
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << endl;
				}else{
					movingZCC = true;
					tempDeg = rotZ;
					tempDeg -= 1.57;

					/*rotZ -= 1.57;
					if(rotZ <= -6.28){
					rotZ = 0.0;
					}*/

					/*movingZCC = true;
					tempDeg = rotZ;
					tempDeg -= 90;*/

					/*rotZ -= 90;
					if(rotZ <= -360){
					rotZ = 0.0;
					}*/
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotZ);
					myMatrix.push_back(matrix(axis,tempDeg,di));
					cout << "history: " << myMatrix.size() << endl;
				}
			}
			//myMatrix.push_back(matrix(axis,tempDeg,di));
			//objectList[j]->ApplyTempMatrix();
			//objectList[j]->DestroyTempMatrix();
		}
		//}
	}else{
		//cout << "null at face rotation" << endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs,int cubieId){
	//it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	//it takes the input group and breaks it, to put parts on cubie group "objects"
	if(objs != NULL){
		sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
		int objctr = 0;
		//break incomming group
		const int ChCnt = objs->GetChildrenList()->GetCount();
		numObjs = ChCnt;
		//give this cubies list some memory
		objectList = (sgC3DObject**)malloc(ChCnt*sizeof(sgC3DObject*));
		//start breaking incoming group
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objs->BreakGroup(allParts);
		sgCObject::DeleteObject(objs);

		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgC3DObject *temp = (sgC3DObject*) allParts[j];
			//put clone on *[] tomake new group
			objectList[j] = (sgC3DObject*)temp->Clone();
			objcts[objctr] = (sgC3DObject*)temp->Clone();
			objctr ++;
			//////////////////////create ofMEsh
			ofMesh tempMesh;
			ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
			sgC3DObject *o = (sgC3DObject*)temp->Clone();
			o->Triangulate(SG_VERTEX_TRIANGULATION);
			//convert to ofMEsh with cubie ID!!!
			ofr->sgCoretoOFmesh(o,tempMesh,cubieId); //give cubie id!!
			myMeshs.push_back(tempMesh);
			ofVbo tempVbo;
			tempVbo.setMesh(tempMesh, GL_STATIC_DRAW);
			myVbos.push_back(tempVbo);
			free(ofr);
			sgDeleteObject(o);
		}
		free(allParts);
		//put that new group inside *objects of this class, of every cubie
		objects = sgCGroup::CreateGroup(objcts,objctr);
		free(objcts);
	}else{
		numObjs = 0;
		objects = NULL;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::rotate(SG_VECTOR r){
	rot = r;
}
//--------------------------------------------------------------
void cubie::move(SG_VECTOR p){
	pos = p;
}
//--------------------------------------------------------------
void cubie::changeColorToColor(ofFloatColor Sc, ofFloatColor Tc){
	ofRender *ofr = new ofRender(); 
	for(int j=0; j< numObjs; j++){
		ofr->changeColorToColor(Sc,Tc,myMeshs[j]);
		//have to replace the vbo
		ofVbo tempVbo;
		tempVbo.setMesh(myMeshs[j], GL_STATIC_DRAW);
		myVbos[j]=tempVbo;
	}

	free(ofr);
}
//----------------------------------------------------------------
void cubie::exit(){
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			sgCObject::DeleteObject(objectList[j]);
		}
		free(objectList);
		sgCObject::DeleteObject(objects);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
sgCGroup* cubie::copyObjects(){
	//make a copy of *objects send outside cubie
	//so originals dont get messed up, and cubies can draw every time without making a the boolean first

	sgCGroup* aux;
	//sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
	//sgCObject **objcts1 = (sgCObject**)malloc(50*sizeof(sgCObject*));
	//int objctr = 0;

	//if(objects != NULL){
	//	const int ChCnt = objects->GetChildrenList()->GetCount();
	//	sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	//	objects->BreakGroup(allParts);
	//	sgCObject::DeleteObject(objects);
	//	for (int j=0; j < ChCnt; j++){
	//		//clone each object
	//		sgCObject *temp = allParts[j];
	//		//put clone on *[] tomake new group
	//		objcts[objctr] = temp->Clone();
	//		objcts1[objctr] = temp->Clone();
	//		objctr ++;
	//		sgCObject::DeleteObject(temp);
	//	}
	//	free(allParts);
	//	//put that new group inside aux**[]
	//	objects = sgCGroup::CreateGroup(objcts,objctr); //so objects[] has the data again, and keeps it for future requests
	//	aux = sgCGroup::CreateGroup(objcts1,objctr);  
	//}else{
	//	return NULL;
	//}
	//free(objcts);
	//free(objcts1);
	return aux;
}