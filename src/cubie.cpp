#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"
#include <math.h>


cubie::cubie(float x, float y,float z, int idi, int selObjId, ofVec3f offset){
	objects = NULL;
	id = idi;
	selectedObjectID = selObjId;

	pos.x = x;
	pos.y = y;
	pos.z = z;

	/*rot.x = 0.0;
	rot.y = 0.0;
	rot.z = 0.0;*/

	//color = rand()%27;

	pointRotate.x = offset.x;
	pointRotate.y = offset.y;
	pointRotate.z = offset.z;

	//rotCompensation.x = offrotate.x;//this is coming in as degrees
	//rotCompensation.y = offrotate.y;
	//rotCompensation.z = offrotate.z;

	moving = false;

	/*ct1 = 0.0;
	ct2 = 0.0;*/
	animTime = 2; //this changes the speed of the animations

	sample = true;
	rotXa = 0.0;
	//initializing cubie history
	SG_POINT axis = {0,0,0};
	SG_VECTOR vrotFace = {0,0,0};
	myMatrix.push_back(matrix(axis,0.0,true));
	//to control undo
	undoing = false;
	
	bDraw = true;
	bDrawWire = false;
	bRotate = false;
}
//--------------------------------------------------------------
void cubie::setup(){
}

//--------------------------------------------------------------
void cubie::update(){

	if(numObjs > 0){
		if(moving==true){
			if(myMatrix.size()>=2){
				for(int i=0; i<myMatrix.size()-1;i++){
					//build rotation matrix for all steps up to the one where it was at the moment of a new movement
					SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
					SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
					double d = myMatrix.at(i).deg;
					d = ofDegToRad(d);
					if(myMatrix.at(i).dir == true){
						//c
						for (int j=0; j < numObjs; j++){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}

					}else{
						//cc
						for (int j=0; j < numObjs; j++){
							if (objectList[j]->GetTempMatrix()==0){
								objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
							}else{
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
							}
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//we are at the last positon
				SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
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
						for (int j=0; j < numObjs; j++){
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						}
						//ct1 = ct2;
					}else{
						rotXa = tempDeg2;
						double aux =  ofDegToRad(rotXa);
						for (int j=0; j < numObjs; j++){
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						}
						moving = false;
						//movingXC = false;
						if(undoing == true){
							//pop 2 histories on this cubie
							//this will leave us in the same position
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
						for (int j=0; j < numObjs; j++){
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						}
						//ct1 = ct2;
					}else{
						rotXa = tempDeg2;
						double aux =  ofDegToRad(rotXa);
						for (int j=0; j < numObjs; j++){
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
						}
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
		else{
			//puzzle is not moving
			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			for(int i=0; i<myMatrix.size();i++){
				SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
				SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis of rotation
				double d = myMatrix.at(i).deg;
				d = ofDegToRad(d);
				if(myMatrix.at(i).dir == true){
					//c
					for (int j=0; j < numObjs; j++){
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
				}else{
					//cc
					for (int j=0; j < numObjs; j++){
						if (objectList[j]->GetTempMatrix()==0){
							objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
						}else{
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
						}
					}
				}
			}
		}
		/////////////////////////////////////////////////////////
		//////undo rotations of armature in z-y-x order
		SG_VECTOR vrotZ = {0,0,1};      
		SG_VECTOR puzzleRotate = {0,0,0};
		for (int j=0; j < numObjs; j++){
			if (objectList[j]->GetTempMatrix()==0){
				objectList[j]->InitTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(armRotations.z));
			}else{
				objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(armRotations.z));
			}
			SG_VECTOR vrotY = {0,1,0}; 							 
			objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotY,ofDegToRad(armRotations.y));
			SG_VECTOR vrotX = {1,0,0}; 							 
			objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(armRotations.x));
			objectList[j]->ApplyTempMatrix();
		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	//now it draws its vector of Vbos, myVbos
	//use this cubies objectList[]
	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			glPushMatrix();
			//ofScale(1.2,1.2,1.2);
			if (objectList[j]->GetTempMatrix()!=0)
			glMultMatrixd(objectList[j]->GetTempMatrix()->GetTransparentData());
			objectList[j]->DestroyTempMatrix();
			if(bDraw){
				if(bDrawWire){
					myMeshs[j].drawWireframe();
				} else {
					myVbos[j].draw(GL_TRIANGLES, 0,myMeshs[j].getNumIndices());
				}
			}
			/*
			if(bDraw){
				myMeshs[j].drawWireframe();
			}
			*/
			//ofPushMatrix();
			//ofTranslate(centroid3d.x, centroid3d.y, centroid3d.z);
			//ofSetColor(centroidColor.x, centroidColor.y, centroidColor.z);
			//ofDrawSphere(0,0,0,10);
			centroid2d = projectPoint(centroid3d);
			//ofPopMatrix();
			glPopMatrix();
		}
	}
	/*
	if(!bDraw){
		bDraw = true;
	}
	*/
	/*
	ofFill();
	ofSetColor(centroidColor.x, centroidColor.y, centroidColor.z);
	ofPushMatrix();
	ofTranslate(0,0,centroid2d.z);
	ofRect(drawPos.x - (drawSize.x / 2), drawPos.y - (drawSize.y / 2),drawSize.x, drawSize.y); 
	ofPopMatrix();
	*/
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(objects != NULL){
		if(moving == false){
			if(undoing == false){
				//for (int j=0; j < numObjs; j++){
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
					myMatrix.push_back(matrix(axis,90,di));
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
				//}
			}
		}
	}else{
		//cout << "null at face rotation" << endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs,int cubieId,ofVec3f v){
	////it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	////it takes the input group and breaks it, to put parts on cubie group "objects"

	armRotations = (v);//*-1; did not have to inverse the angle to compensate due to left hand and right hand rules on sgcore and on openframeworks

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
void cubie::crateOfMeshs(){
	ofMesh tempMesh;
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)

	if(objects != NULL){
		for (int j=0; j < numObjs; j++){
			//////////////////////create ofMEsh
			sgC3DObject *o = (sgC3DObject*)objectList[j]->Clone();
			o->Triangulate(SG_VERTEX_TRIANGULATION);
			//convert to ofMEsh with cubie ID!!!
			//ask if its a plain color puzzle:: bunny torus??
			if(selectedObjectID == 1){
				//torus
				ofr->sgCoretoOFmesh(o,tempMesh,-1,selectedObjectID);
			}else if(selectedObjectID == 4){
				//bunny
				ofr->sgCoretoOFmesh(o,tempMesh,-3,selectedObjectID);
			}else{
				ofr->sgCoretoOFmesh(o,tempMesh,id,selectedObjectID); //give cubie id!! so that it knows if its a plain color puzzle or not
			}
			myMeshs.push_back(tempMesh);
			ofVbo tempVbo;
			tempVbo.setMesh(tempMesh, GL_STATIC_DRAW);
			myVbos.push_back(tempVbo);
			sgDeleteObject(o);
		}
	}
	getCentroid();
	free(ofr);
}

void cubie::getCentroid(){
	centroid3d.set(0,0,0);
	int count = 0;
	for(vector<ofMesh>::iterator mIter = myMeshs.begin(); mIter != myMeshs.end(); mIter++){
		ofVec3f tVec = mIter->getCentroid();
		centroid3d += tVec;
		count++;
	}
	centroid3d /= count;
	//cout << "Centroid = " << centroid3d.x << ", " << centroid3d.y << ", " << centroid3d.z << endl;
	//cout << count << endl;
	centroidColor.set(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255));
	//centroidColor.set(255,255,255);
}

ofVec3f cubie::projectPoint(ofVec3f _pnt){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble modelView[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluProject(_pnt.x, _pnt.y, _pnt.z, modelView, projection, viewport, &x, &y, &z);
	y = ofGetHeight() - y;
	return ofVec3f(x, y, z);
	//cout << "made pick point " << centroid2d.x << ", " << centroid2d.y << endl;
	//cout << "Cubie " << id << " z = " << centroid2d.z << endl;
}

float cubie::getDistanceByVertex(ofVec3f _pos){
	float nearestDist = 10000;
	ofVec3f nearestVertex;
	int nearestMesh = 0;
	int counter = 0;
	for(int i = 0; i < myMeshs.size(); i++){
		int numVertices = myMeshs[i].getNumVertices();
		for(int j = 0; j < numVertices; j++){
			if (objectList[i]->GetTempMatrix()!=0){
				glPushMatrix();
				glMultMatrixd(objectList[j]->GetTempMatrix()->GetTransparentData());
				objectList[j]->DestroyTempMatrix();
				ofVec3f vScreen = projectPoint(myMeshs[i].getVertex(j));
				glPopMatrix();
				float cDist = _pos.distance(myMeshs[i].getVertex(i));
				if(cDist < nearestDist){
					nearestDist = cDist;
					nearestVertex = myMeshs[i].getVertex(i);
					nearestMesh = counter;
				}
			}
		}
		counter++;
	}
	selectedVertex = nearestVertex;
	selectedMesh = nearestMesh;
	return nearestDist;
}

bool cubie::getRotate(){
	if(bRotate){
		cout << "returning bRotate" << endl;
	}
	return bRotate;
}

void cubie::setRotate(bool _rotate){
	bRotate = _rotate;
}

//-------------------------------------------------------------------------------------------------------------------------------------------
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
				//for (int j=0; j < numObjs; j++){
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
				//}
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
bool cubie::isMoving(){
	//returns the state of the moving boolean
	//to prevent key press events from messing up the movements
	return moving;
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
