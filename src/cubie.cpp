#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"

cubie::cubie(float x, float y,int idi){
	objects = NULL;
	id = idi;
	posX = x;
	posY = y;
	rotH = 0.0;
	rotV = 0.0;
	color = rand()%27;
	okDraw = true;
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){

}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis, float deg,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			SG_POINT protFace = {0,0,0};
			SG_VECTOR vrotFace = axis;
			rotX=0;
			rotY=0;
			rotZ=0;
			if(di == true){
				//c
				if(axis.x == 1){
					rotX += 1.57;
					if(rotX >= 6.28){
						//rotX = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotX);
					//build matrix and add it to the vector myMatrix
					myMatrix.push_back(matrix(axis,rotX,di));
					cout << "history: " << myMatrix.size() << endl;
					cout << rotX << endl;
				}else if(axis.y == 1){
					rotY += 1.57;
					if(rotY >= 6.28){
						rotY = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotY);
					myMatrix.push_back(matrix(axis,rotY,di));
				}else{
					rotZ += 1.57;
					if(rotZ >= 6.28){
						rotZ = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotZ);
					myMatrix.push_back(matrix(axis,rotZ,di));
				}
			}else{
				//cc
				if(axis.x == 1){
					rotX -= 1.57;
					if(rotX <= -6.28){
						rotX = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotX);
					myMatrix.push_back(matrix(axis,rotX,di));
					cout << rotX << endl;
				}else if(axis.y == 1){
					rotY -= 1.57;
					if(rotY <= -6.28){
						rotY = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotY);
					myMatrix.push_back(matrix(axis,rotY,di));
				}else{
					rotZ -= 1.57;
					if(rotZ <= -6.28){
						rotZ = 0.0;
					}
					//objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,rotZ);
					myMatrix.push_back(matrix(axis,rotZ,di));
				}
			}

			//objectList[j]->ApplyTempMatrix();
			//objectList[j]->DestroyTempMatrix();
		}
	}else{
		//cout << "null at face rotation" << endl;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	// it attaches to scene
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(okDraw == true){
		if(objects != NULL){
			for (int j=0; j < numObjs; j++){
				//rotations
				//use vector with matrix..es
				for(int i=0; i<myMatrix.size();i++){
					//if(myMatrix.at(i).type==1){
					//its a rotation
					//do this # of t
					//int tam = myMatrix.size();
					//if(tam >0){
					SG_POINT protFace = {0,0,0};//myMatrix.at(0).point;// point;
					SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
					float d = myMatrix.at(i).deg;

					if(myMatrix.at(i).dir == true){
						//c
						if(myMatrix.at(i).vector.x == 1){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							//rotZ = d;
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}

					}else{
						//cc
						if(myMatrix.at(i).vector.x == 1){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else if(myMatrix.at(i).vector.y == 1){
							//rotY = d;
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							//rotZ = d;
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
					objectList[j]->ApplyTempMatrix();
					objectList[j]->DestroyTempMatrix();
				}
				//}
				//}

				//if(myMatrix[myMatrix.size()-1].vector.x == 1){
				//	rotX = myMatrix[myMatrix.size()-1].deg;
				//}else if(myMatrix[myMatrix.size()-1].vector.y == 1){
				//	//rotY = d;
				//	rotY = myMatrix[myMatrix.size()-1].deg;
				//}else{
				//	//rotZ = d;
				//	rotZ = myMatrix[myMatrix.size()-1].deg;
				//}
				////rotate and move with the whole puzzle
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
				objectList[j]->SetAttribute(SG_OA_COLOR,color);
				objectList[j]->Triangulate(SG_VERTEX_TRIANGULATION);
				objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_FULL);

				//sgGetScene()->AttachObject(objectList[j]);

			}
		}else{
			//cout << "null at draw" << endl;
		}
		okDraw = false;
	}
}
//--------------------------------------------------------------
void cubie::unDraw(){  
	//sgGetScene()->Clear();
	//detach from scene
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			//objectList[j]->SetAttribute(SG_OA_DRAW_STATE,SG_DS_HIDE);
			//sgGetScene()->DetachObject(objectList[j]);
			//sgCObject::DeleteObject(objectList[j]);
			okDraw = true;
		}
		////////////////////////////////////IMPORTANT!!!!!!!!///////////////////////////////////
		////remake objectList for next drawing
		////there is somememory leakage here!!!!! even after sgCObject::DeleteObject(objectList[j]);
		/////////////////////////////////////////////////////////////////////////////////////////////

		sgCGroup *objects2 = copyObjects(); 
		const int ChCnt = objects2->GetChildrenList()->GetCount();
		numObjs = ChCnt;
		//give this cubies list some memory
		objectList = (sgC3DObject**)malloc(ChCnt*sizeof(sgC3DObject*));
		//start breaking incoming group
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objects2->BreakGroup(allParts);
		sgCObject::DeleteObject(objects2);

		for (int j=0; j < ChCnt; j++){
			objectList[j] = (sgC3DObject*)allParts[j];
		}
		free(allParts);
	}else{
		//cout << "null at undraw" << endl;
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
		sgCObject::DeleteObject(objects);
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
		objectList = (sgC3DObject**)malloc(ChCnt*sizeof(sgC3DObject*));
		//start breaking incoming group
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		objs->BreakGroup(allParts);
		sgCObject::DeleteObject(objs);

		for (int j=0; j < ChCnt; j++){
			//clone each object
			sgC3DObject *temp = (sgC3DObject*) allParts[j];
			//put clone on *[] tomake new group

			objectList[objctr] = (sgC3DObject*)temp->Clone();
			objcts[objctr] = (sgC3DObject*)temp->Clone();
			objctr ++;
			//make ofMEsh
			//ofMesh tempMesh;
			//ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
			//sgC3DObject *o = (sgC3DObject*)temp->Clone();
			//cout << o->GetType() << endl;

			//ofr->sgCoretoOFmesh(o,tempMesh);
			//
			//myMeshs.push_back(tempMesh);
			//ofVbo tempVbo;
			//tempVbo.setMesh(tempMesh, GL_STATIC_DRAW);
			//myVbos.push_back(tempVbo);
			//free(ofr);


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
//----------------------------------------------------------------
void cubie::exit(){
	if(objects != NULL){
		free(objectList);
		sgCObject::DeleteObject(objects);
	}
}
