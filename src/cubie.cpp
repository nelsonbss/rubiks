#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"

cubie::cubie(float x, float y,float z, int idi){
	objects = NULL;
	id = idi;
	posX = x;
	posY = y;
	posZ = z;
	rotH = 0.0;
	rotV = 0.0;
	color = rand()%27;
	okDraw = true;
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
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){
	if(moving==true){
		//animate rotation xc
		if(movingXC == true){
			if(rotX < tempDeg){
				ct2 = ofGetElapsedTimeMillis();
				rotX += (ct2 - ct1)*((1.57)/animTime);
				ct1 = ct2;
			}else{
				rotX = tempDeg;
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
		//	//xcc
		//	if(movingXCC == true){
		//		if(rotX > tempDeg){
		//			ct2 = ofGetElapsedTimeMillis();
		//			rotX -= (ct2 - ct1)*((90)/animTime);
		//			ct1 = ct2;
		//		}else{
		//			rotX = tempDeg;
		//			if(rotX <= -360){
		//				rotX = 0.0;
		//			}
		//			moving = false;
		//			movingXCC = false;
		//		}
		//	}
		//	////////////   Y
		//	//animate rotation yc
		//	if(movingYC == true){
		//		if(rotY < tempDeg){
		//			ct2 = ofGetElapsedTimeMillis();
		//			rotY += (ct2 - ct1)*((90)/animTime);
		//			ct1 = ct2;
		//		}else{
		//			rotY = tempDeg;
		//			if(rotY >= 360){
		//				rotY = 0.0;
		//			}
		//			moving = false;
		//			movingYC = false;
		//		}
		//	}
		//	//ycc
		//	if(movingYCC == true){
		//		if(rotY > tempDeg){
		//			ct2 = ofGetElapsedTimeMillis();
		//			rotY -= (ct2 - ct1)*((90)/animTime);
		//			ct1 = ct2;
		//		}else{
		//			rotY = tempDeg;
		//			if(rotY <= -360){
		//				rotY = 0.0;
		//			}
		//			moving = false;
		//			movingYCC = false;
		//		}
		//	}
		//	////////////  Z
		//	//animate rotation zc
		//	if(movingZC == true){
		//		if(rotZ < tempDeg){
		//			ct2 = ofGetElapsedTimeMillis();
		//			rotZ += (ct2 - ct1)*((90)/animTime);
		//			ct1 = ct2;
		//		}else{
		//			rotZ = tempDeg;
		//			if(rotZ >= 360){
		//				rotZ = 0.0;
		//			}
		//			moving = false;
		//			movingZC = false;
		//		}
		//	}
		//	//zcc
		//	if(movingZCC == true){
		//		if(rotZ > tempDeg){
		//			ct2 = ofGetElapsedTimeMillis();
		//			rotZ -= (ct2 - ct1)*((90)/animTime);
		//			ct1 = ct2;
		//		}else{
		//			rotZ = tempDeg;
		//			if(rotZ <= -360){
		//				rotZ = 0.0;
		//			}
		//			moving = false;
		//			movingZCC = false;
		//		}
		//	}
	}

	//if(objects != NULL){
	for (int j=0; j < numObjs; j++){

		///////////////////////////////////applying rotations matrix HISTORY
		//use vector with matrix(s)
		//for(int i=0; i<myMatrix.size();i++){
		//	SG_POINT protFace = {0,0,0};										 
		//	SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
		//	float d = myMatrix.at(i).deg;

		//	if(myMatrix.at(i).dir == true){
		//		//c
		//		if(myMatrix.at(i).vector.x == 1){
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}else if(myMatrix.at(i).vector.y == 1){
		//			//rotY = d;
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}else{
		//			//rotZ = d;
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}

		//	}else{
		//		//cc
		//		if(myMatrix.at(i).vector.x == 1){
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}else if(myMatrix.at(i).vector.y == 1){
		//			//rotY = d;
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}else{
		//			//rotZ = d;
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
		//			}
		//		}
		//	}
		//	//objectList[j]->ApplyTempMatrix();
		//	//objectList[j]->DestroyTempMatrix();
		//}

		/////////////////////////////////////////////////////////////////
		SG_POINT protFace = {0,0,0};										 
		SG_VECTOR vrotFaceX ={1,0,0};
		SG_VECTOR vrotFaceY ={0,1,0};
		SG_VECTOR vrotFaceZ ={0,0,1};
		objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFaceX,rotX);
		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFaceY,rotY);
		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFaceZ,rotZ);
		/////////////////////////////////////////////////////////////////

		//	/////////////////////////////////////////////////////////////
		//	//if(myMatrix[myMatrix.size()-1].vector.x == 1){
		//	//	rotX = myMatrix[myMatrix.size()-1].deg;
		//	//}else if(myMatrix[myMatrix.size()-1].vector.y == 1){
		//	//	//rotY = d;
		//	//	rotY = myMatrix[myMatrix.size()-1].deg;
		//	//}else{
		//	//	//rotZ = d;
		//	//	rotZ = myMatrix[myMatrix.size()-1].deg;
		//	//}

		///////////////////////////////////////////////////////////
		////////rotate and move with the whole puzzle
		SG_VECTOR vrotH = {0,1,0}; //rotate H
		SG_POINT protH = {0,0,0};                                             
		/*if (objectList[j]->GetTempMatrix()==0){
		objectList[j]->InitTempMatrix()->Rotate(protH,vrotH,rotH);
		}else{*/
		objectList[j]->GetTempMatrix()->Rotate(protH,vrotH,rotH);
		//}
		SG_VECTOR vrotV = {1,0,0}; //rotate V
		SG_POINT protV = {0,0,0};												 
		objectList[j]->GetTempMatrix()->Rotate(protV,vrotV,rotV);
		//translations
		SG_VECTOR transBox11 = {posX,0,0}; 
		objectList[j]->GetTempMatrix()->Translate(transBox11);
		SG_VECTOR transBox121 = {0,posY,0}; 
		objectList[j]->GetTempMatrix()->Translate(transBox121);
		objectList[j]->ApplyTempMatrix();


		//	//objectList[j]->Triangulate(SG_VERTEX_TRIANGULATION);

		//	//objectList[j]->DestroyTempMatrix();
		//	//objectList[j]->SetAttribute(SG_OA_COLOR,color);
		//	//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_FULL);
		//	//sgGetScene()->AttachObject(objectList[j]);
		//}
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
void cubie::unDraw(){  
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
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis, float deg,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		if(moving == false){
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
						myMatrix.push_back(matrix(axis,rotX,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "X: " << rotX << endl;
					}else if(axis.y == 1){
						rotY += 1.57;
						/*if(rotY >= 6.28){
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
						myMatrix.push_back(matrix(axis,rotY,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "Y: " << rotY << endl;
					}else{
						rotZ += 1.57;
						/*if(rotZ >= 6.28){
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
						myMatrix.push_back(matrix(axis,rotZ,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "Z: " << rotZ << endl;
					}
				}else{
					//cc
					if(axis.x == 1){
						rotX -= 1.57;
						/*if(rotX <= -6.28){
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
						myMatrix.push_back(matrix(axis,rotX,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "X: " << rotX << endl;
					}else if(axis.y == 1){
						rotY -= 1.57;
						/*if(rotY <= -6.28){
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
						myMatrix.push_back(matrix(axis,rotY,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "Y: " << rotY << endl;
					}else{
						rotZ -= 1.57;
						/*if(rotZ <= -6.28){
						rotZ = 0.0;
						}*/
						/*movingZCC = true;
						tempDeg = rotZ;
						tempDeg -= 90*/;
						/*rotZ -= 90;
						if(rotZ <= -360){
						rotZ = 0.0;
						}*/
						//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotZ);
						myMatrix.push_back(matrix(axis,rotZ,di));
						cout << "history: " << myMatrix.size() << endl;
						cout << "Z: " << rotZ << endl;
					}
				}
				//myMatrix.push_back(matrix(axis,tempDeg,di));
				objectList[j]->ApplyTempMatrix();
				//objectList[j]->DestroyTempMatrix();
			}
		}
	}else{
		//cout << "null at face rotation" << endl;
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
void cubie::rotateH(float rad){
	rotH = rad;
	//movePuzzle = true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::rotateV(float rad){
	rotV = rad;
	//movePuzzle = true;
}
//--------------------------------------------------------------
void cubie::moveH(float posx){
	posX = posx;
	//movePuzzle = true;
}
//--------------------------------------------------------------
void cubie::moveV(float posy){
	posY = posy;
	//movePuzzle = true;
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