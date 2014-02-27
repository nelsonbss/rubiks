#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"

cubie::cubie(float x, float y,float z, int idi, int selObjId){
	objects = NULL;
	id = idi;
	selectedObjectID = selObjId;
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
	/*rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;*/
	moving = false;
	/*	movingXC = false;
	movingXCC = false;
	movingYC = false;
	movingYCC = false;
	movingZC = false;
	movingZCC = false*/;
	/*ct1 = 0.0;
	ct2 = 0.0;*/
	animTime = 2;

	sample = true;
	rotXa = 0.0;
	//initializing cubie history
	SG_POINT axis = {0,0,0};
	SG_VECTOR vrotFace = {0,0,0};
	myMatrix.push_back(matrix(axis,0.0,true));
	//to control undo
	undoing = false;
}
//--------------------------------------------------------------
void cubie::setup(){

}
//--------------------------------------------------------------
void cubie::update(){
	for (int j=0; j < numObjs; j++){
		if(moving==true){
			//for (int j=0; j < numObjs; j++){
			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			//if(myMatrix.size()<2){
			//	for(int i=0; i<myMatrix.size();i++){
			//		SG_POINT protFace = {0,0,0};										 
			//		SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
			//		double d = myMatrix.at(i).deg;
			//		d = ofDegToRad(d);
			//		if(myMatrix.at(i).dir == true){
			//			//c
			//			//if(myMatrix.at(i).vector.x == 1){
			//			if (objectList[j]->GetTempMatrix()==0){
			//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			}else{
			//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			}
			//			//}else if(myMatrix.at(i).vector.y == 1){
			//			//	//rotY = d;
			//			//	if (objectList[j]->GetTempMatrix()==0){
			//			//		objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}else{
			//			//		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}
			//			//}else{
			//			//	//rotZ = d;
			//			//	if (objectList[j]->GetTempMatrix()==0){
			//			//		objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}else{
			//			//		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}
			//			//}
			//		}else{
			//			//cc
			//			//if(myMatrix.at(i).vector.x == 1){
			//			if (objectList[j]->GetTempMatrix()==0){
			//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			}else{
			//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			}
			//			//}else if(myMatrix.at(i).vector.y == 1){
			//			//	//rotY = d;
			//			//	if (objectList[j]->GetTempMatrix()==0){
			//			//		objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}else{
			//			//		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}
			//			//}else{
			//			//	//rotZ = d;
			//			//	if (objectList[j]->GetTempMatrix()==0){
			//			//		objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}else{
			//			//		objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//			//	}
			//			//}
			//		}
			//	}
			//}
			if(myMatrix.size()>=2){
				//build rotation matrix for all steps up to the one where it was at the moment of a new movement
				for(int i=0; i<myMatrix.size()-1;i++){
					SG_POINT protFace = {0,0,0};										 
					SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
					double d = myMatrix.at(i).deg;
					d = ofDegToRad(d);
					if(myMatrix.at(i).dir == true){
						//c
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}

					}else{
						//cc
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
				}
				//we are at the last positon
				SG_POINT protFace = {0,0,0};										 
				SG_VECTOR vrotFace = myMatrix.at(myMatrix.size()-1).vector;//  axis; //rotate to do a face move
				double tempDeg2 = myMatrix.at(myMatrix.size()-1).deg; //target angle, the last angle it will move to
				if(sample==false){
					//this should only be sampled once during the animation
					rotXa = 0;//myMatrix.at(myMatrix.size()-2).deg;
					sample=true;
				}
				if(myMatrix.at(myMatrix.size()-1).dir == true){
					//animate rotation xc
					//if(movingXC == true){
					if(rotXa < tempDeg2){
						//ct2 = ofGetElapsedTimeMillis();
						rotXa += animTime;//0.1;//(ct2 - ct1)*((1.57)/animTime);
						double aux =  ofDegToRad(rotXa);
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						//ct1 = ct2;
					}else{
						rotXa = tempDeg2;
						double aux =  ofDegToRad(rotXa);
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						moving = false;
						//movingXC = false;
						if(undoing == true){
							//pop 2 histories on this cubie
							//this wwill leave us in the same position
							myMatrix.pop_back();
							myMatrix.pop_back();
							undoing=false;
						}
					}
					//}
				}else{
					//xcc
					//if(movingXCC == true){
					if(rotXa > tempDeg2){
						//ct2 = ofGetElapsedTimeMillis();
						rotXa -= animTime;//0.1;  //(ct2 - ct1)*((1.57)/animTime);
						double aux =  ofDegToRad(rotXa);
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						//ct1 = ct2;
					}else{
						rotXa = tempDeg2;
						double aux =  ofDegToRad(rotXa);
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						moving = false;
						//movingXCC = false;
						if(undoing == true){
							//pop 2 histories on this cubie
							//this wwill leave us in the same position
							myMatrix.pop_back();
							myMatrix.pop_back();
							undoing=false;
						}
					}
					//}
				}
			}
		}
		//  else if (undoing == true){
		//	//go from the last element in cubie history, doing the inverse move that got it there
		//	//i.e. if last move was C on x -> then we do CC on x

		//	SG_POINT protFace = {0,0,0};										 
		//	SG_VECTOR vrotFace = myMatrix.at(myMatrix.size()-1).vector;//  axis; //rotate to do a face move
		//	double targetDeg = 0;//myMatrix.at(myMatrix.size()-1).deg; //target angle, the last angle it will move to
		//	if(sample==false){
		//		//this should only be sampled once during the animation
		//		rotXa = myMatrix.at(myMatrix.size()-1).deg; // from where we want to move towards zero, to do the undo animation
		//		sample=true;
		//	}
		//	if(myMatrix.at(myMatrix.size()-1).dir == true){
		//		//it was rotation C
		//		//so we do CC!!!!!
		//		if(rotXa > targetDeg){
		//			//ct2 = ofGetElapsedTimeMillis();
		//			rotXa -= animTime/2;//0.1;//(ct2 - ct1)*((1.57)/animTime);
		//			double aux =  ofDegToRad(rotXa);
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}
		//			//ct1 = ct2;
		//		}else{
		//			rotXa = targetDeg;
		//			double aux =  ofDegToRad(rotXa);
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}
		//			myMatrix.pop_back();
		//			undoing = false;
		//		}
		//	}else{
		//		//it was cc
		//		//so we do C!!!!
		//		if(rotXa < targetDeg){
		//			//ct2 = ofGetElapsedTimeMillis();
		//			rotXa += animTime;//0.1;  //(ct2 - ct1)*((1.57)/animTime);
		//			double aux =  ofDegToRad(rotXa);
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}
		//			//ct1 = ct2;
		//		}else{
		//			rotXa = targetDeg;
		//			double aux =  ofDegToRad(rotXa);
		//			if (objectList[j]->GetTempMatrix()==0){
		//				objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}else{
		//				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
		//			}
		//			myMatrix.pop_back();
		//			undoing = false;
		//		}
		//	}


		//}
		else{
			//puzzle is not moving
			//if(objects != NULL){
			//for (int j=0; j < numObjs; j++){
			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			for(int i=0; i<myMatrix.size();i++){
				SG_POINT protFace = {0,0,0};										 
				SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis of rotation
				double d = myMatrix.at(i).deg;
				d = ofDegToRad(d);
				if(myMatrix.at(i).dir == true){
					//c
					if (objectList[j]->GetTempMatrix()==0){
						objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
					}else{
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
					}
				}else{
					//cc
					if (objectList[j]->GetTempMatrix()==0){
						objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
					}else{
						objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
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
			//ofScale(1.2,1.2,1.2);
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

//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		if(moving == false){
			if(undoing == false){
				for (int j=0; j < numObjs; j++){
					SG_POINT protFace = {0,0,0};
					SG_VECTOR vrotFace = axis;
					/*rotX=0;
					rotY=0;
					rotZ=0;*/
					moving = true;
					sample = false;
					//ct1 = ofGetElapsedTimeMillis();
					if(di == true){
						//c
						//if(axis.x == 1){
						//movingXC = true;
						//rotX += 90;
						myMatrix.push_back(matrix(axis,90,di));
						//cout << "history: " << myMatrix.size() << " rotX: " << rotX << endl;
						//}else if(axis.y == 1){
						//	//movingYC = true;
						//	sample = false;
						//	//rotY += 90;
						//	myMatrix.push_back(matrix(axis,90,di));
						//}else{
						//	//movingZC = true;
						//	sample = false;
						//	//rotZ += 90;
						//	myMatrix.push_back(matrix(axis,90,di));
						//}
					}else{
						//cc
						//if(axis.x == 1){
						//movingXCC = true;
						//rotX -= 90;
						myMatrix.push_back(matrix(axis,-90,di));
						//}else if(axis.y == 1){
						//	//movingYCC = true;
						//	sample = false;
						//	//rotY -= 90;
						//	myMatrix.push_back(matrix(axis,-90,di));
						//}else{
						//	//movingZCC = true;
						//	sample = false;
						//	//rotZ -= 90;
						//	myMatrix.push_back(matrix(axis,-90,di));
						//}
					}
				}
			}
		}
	}else{
		//cout << "null at face rotation" << endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs,int cubieId){
	////it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	////it takes the input group and breaks it, to put parts on cubie group "objects"
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
			//ask if its a plain color puzzle:: bunny torus??
			if(selectedObjectID == 4){
				//bunny
				ofr->sgCoretoOFmesh(o,tempMesh,-3,selectedObjectID);
			}else{
				ofr->sgCoretoOFmesh(o,tempMesh,cubieId,selectedObjectID); //give cubie id!! so that it knows if its a plain color puzzle or not
			}
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
	int cubie::getId(){
		return id;
	}
	//-------------------------------------------------------------
	float cubie::getNumObjs(){
		return numObjs;
	}
	//--------------------------------------------------------------
	void cubie::unDo(SG_VECTOR axis,bool di){
		//it removes the las element on the vector with the history of each cubie
		if(myMatrix.size()>1){
			//trigger an undo animation routine in the update()
			//at the end of that routine-> we do the pop_back of the cubies history
			if(undoing==false){
				if(moving == false){
					for (int j=0; j < numObjs; j++){
						SG_POINT protFace = {0,0,0};
						SG_VECTOR vrotFace = axis;
						moving = true;
						undoing = true;
						sample = false;
						//ct1 = ofGetElapsedTimeMillis();
						if(di == true){
							//c
							myMatrix.push_back(matrix(axis,90,di));

						}else{
							//cc
							myMatrix.push_back(matrix(axis,-90,di));
						}
					}
				}
			}
		}
	}
	//---------------------------------------------------------------
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
	//-----------------------------------------------------------------
	void cubie::colorBlackSides(int idCubie, float playRoom){
		//have key sides of cubie colored black
		ofRender *ofr = new ofRender(); 
		for(int j=0; j< numObjs; j++){
			ofr->colorBlackSides(myMeshs[j],idCubie,playRoom);
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
