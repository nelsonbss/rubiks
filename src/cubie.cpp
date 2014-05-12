#include "cubie.h"
#include "sgCore.h"
#include "ofRender.h"
#include <math.h>

cubie::cubie(float x, float y,float z, int idi, int selObjId, ofVec3f offset, int gs){
	//objects = NULL;
	id = idi;
	selectedObjectID = selObjId;

	pos.x = x;
	pos.y = y;
	pos.z = z;

	xpos=1;
	ypos=2;
	zpos=3;
	gridSize=gs;

	zpos=idi%gridSize;
	ypos=(idi/gridSize)%gridSize;
	xpos=(idi/(gridSize*gridSize))%gridSize;

	pointRotate.x = offset.x;
	pointRotate.y = offset.y;
	pointRotate.z = offset.z;

	masterAngle = 0.0;
	/*ct1 = 0.0;
	ct2 = 0.0;*/
	animTime = 2; //this changes the speed of the animations

	sample = true;
	//rotXa = 0.0;
	//initializing cubie history

	vrotFace.x = 0;
	vrotFace.y = 0;
	vrotFace.z = 0;

	protFace.x = 0;
	protFace.y = 0;
	protFace.z = 0;
	pivotTransformed = false;

	SG_POINT axis = {0,0,0};
	myMatrix.push_back(matrix(axis,0.0,true));
	//to control undo
	undoing = false;
	goBackb = false;
	moving = false;
	locked = true;
	active = false;

	bDraw = true;
	bDrawWire = false;
	bRotate = false;

	centroid2d.set(0,0,0);
	centroid3d.set(0,0,0);

	bHavePoint = false;
	bUnproject = false;

	radiurS = 10;
}
//--------------------------------------------------------------
void cubie::setup(){
	/////////////////////////////////////////////////////////
	//////undo rotations of armature in z-y-x order
	SG_VECTOR vrotZ = {0,0,1};      
	SG_VECTOR puzzleRotate = {0,0,0};
	for (int j=0; j < numObjs; j++){
		if (objectList[j]->GetTempMatrix()==0){
			objectList[j]->InitTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(0));//armRotations.z));
		}else{
			objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(0));//armRotations.z));
		}
		SG_VECTOR vrotY = {0,1,0}; 							 
		objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotY,ofDegToRad(0));//armRotations.x));
		SG_VECTOR vrotX = {1,0,0}; 							 
		objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(0));//-armRotations.y));
		objectList[j]->ApplyTempMatrix();
	}

	//SG_VECTOR vrotX = {1,0,0};      
	//SG_VECTOR puzzleRotate = {0,0,0};
	//for (int j=0; j < numObjs; j++){
	//	if (objectList[j]->GetTempMatrix()==0){
	//		objectList[j]->InitTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(armRotations.x));
	//	}else{
	//		objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(armRotations.x));
	//	}
	//	//SG_VECTOR vrotY = {0,1,0}; 							 
	//	//objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotY,ofDegToRad(armRotations.x));
	//	//SG_VECTOR vrotX = {1,0,0}; 							 
	//	//objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(armRotations.y));
	//	objectList[j]->ApplyTempMatrix();
	//}
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setPos(int xp, int yp, int zp) {
	xpos=xp;
	ypos=yp;
	zpos=zp;
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis,bool di,float angle){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	if(numObjs != 0){
		if(moving == false && goBackb == false && undoing == false){
			if(locked == false){
				active = true;

				protFace.x = pointRotate.x;
				protFace.y = pointRotate.y;
				protFace.z = pointRotate.z;
				ofVec3f p;
				ofVec3f p2;
				ofVec3f p3;

				ofVec3f t;
				ofVec3f t2;
				ofVec3f t3;

				if(armRotations.x==0 && armRotations.y==0 && armRotations.z == 0){
					vrotFace.x = axis.x;
					vrotFace.y = axis.y;
					vrotFace.z = axis.z;
				}else{
					ofVec3f aux;
					aux.x = axis.x;
					aux.y = axis.y;
					aux.z = axis.z;
					//////t = aux.getRotated(armRotations.z,ofVec3f(0,0,1));
					//////t2 = t.getRotated(armRotations.x,ofVec3f(0,1,0));
					//////t3 = t2.getRotated(-armRotations.y,ofVec3f(1,0,0));
					t = aux.getRotated(-armRotations.y,ofVec3f(1,0,0));
					t2 = t.getRotated(armRotations.x,ofVec3f(0,1,0));
					t3 = t2.getRotated(armRotations.z,ofVec3f(0,0,1));
					vrotFace.x = t3.x;
					vrotFace.y = t3.y;
					vrotFace.z = t3.z;
					if(pivotTransformed == false){
						/////////////////////////////////////////////////////
						//transform the point of rotation
//						aux.x = protFace.x;
//						aux.y = protFace.y;
//						aux.z = protFace.z;
///*						p = aux.getRotated(armRotations.z,ofVec3f(0,0,1));
//						p2 = p.getRotated(armRotations.x,ofVec3f(0,1,0));
//						p3 = p2.getRotated(-armRotations.y,ofVec3f(1,0,0))*/;
//						p = aux.getRotated(-armRotations.y,ofVec3f(1,0,0));
//						p2 = p.getRotated(armRotations.x,ofVec3f(0,1,0));
//						p3 = p2.getRotated(armRotations.z,ofVec3f(0,0,1));
//						protFace.x = p3.x;
//						protFace.y = p3.y;
//						protFace.z = p3.z;

						//protFace.x = -1 * protFace.x;
						//protFace.y = -1 * protFace.y;
						//protFace.z = -1 * protFace.z;

						pivotTransformed = true;
					}				
				}


				//ofVec3f protFaceV(pointRotate.x, pointRotate.y, pointRotate.z);
				//ofVec3f vrotFaceV(vrotFace.x, vrotFace.y, vrotFace.z);
				dir = di;

				//ct1 = ofGetElapsedTimeMillis();
				//if(di == true){
				//c
				//myMatrix.push_back(matrix(axis,angle,di));
				bool reducing = true;
				//cout << "masterAngle face rotate IN: " << masterAngle << endl;
				if(dir){
					//if(masterAngle > 45){
					//	moving = true;
					//}else {
					//	masterAngle += angle;
					//	for (int j=0; j < numObjs; j++){
					//		myMeshs[j].updatePosition(protFaceV, vrotFaceV, angle);
					//	}
					//}
					while (reducing){
						if(masterAngle + angle > 89){
							angle = ceil(angle/2);
						}else{
							masterAngle += angle;
							for (int j=0; j < numObjs; j++){
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(angle));
								//myMeshs[j].updatePosition(protFaceV, vrotFaceV, angle);
								//myMeshs[j].rotateTriangles(protFaceV, vrotFaceV, angle);
							}
							reducing = false;
						}
					}
				}else{
					//if(masterAngle < -45){
					//	moving = true;
					//}else {
					//	masterAngle -= angle;
					//	for (int j=0; j < numObjs; j++){
					//		myMeshs[j].updatePosition(protFaceV, vrotFaceV, -angle);
					//	}
					//}
					while (reducing){
						if(masterAngle - angle < -89){
							angle = ceil(angle/2);
						}else{
							masterAngle -= angle;
							for (int j=0; j < numObjs; j++){
								objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(-angle));
								//myMeshs[j].updatePosition(protFaceV, vrotFaceV, -angle);
							}
							reducing = false;
						}
					}

				}
				//cout << "masterAngle face rotate OUT: " << masterAngle << endl;
				//////////for (int j=0; j < numObjs; j++){
				//////////	/*if (objectList[j]->GetTempMatrix()==0){
				//////////	objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(angle));
				//////////	}else{
				//////////	objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(angle));
				//////////	}*/
				//////////	if(dir){
				//////////		myMeshs[j].updatePosition(protFaceV, vrotFaceV, angle);
				//////////	}else{
				//////////		myMeshs[j].updatePosition(protFaceV, vrotFaceV, -angle);
				//////////	}
				//////////}
				//}else{
				//	//cc
				//	//myMatrix.push_back(matrix(axis,-angle,di));
				//	masterAngle -= angle;
				//	for (int j=0; j < numObjs; j++){
				//		if (objectList[j]->GetTempMatrix()==0){
				//			objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(-angle));
				//		}else{
				//			objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,ofDegToRad(-angle));
				//		}
				//	}

				//	if(masterAngle <= -30){
				//	
				//	}
				//}
			}
		}
	}else{
		//cout << "null at face rotation" << endl;
	}
}
//---------------------------------------------------------------
void cubie::goBack(){
	goBackb = true;
	sample = false;
	myMatrix.push_back(matrix(vrotFace,-90,dir));
}
//---------------------------------------------------------------
void cubie::goForward(){
	moving = true;
	sample = false;
	myMatrix.push_back(matrix(vrotFace,90,dir));
}
//--------------------------------------------------------------
void cubie::update(){
	int stepSize = 10;

	if(numObjs > 0){
		if(goBackb==true){

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//do animation to 0 position from masterAngle position
			//for (int j=0; j < numObjs; j++){
			SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};		///////////////////////////////////////////								 
			//ofVec3f protFaceV(pointRotate.x, pointRotate.y, pointRotate.z);
			//ofVec3f vrotFaceV(vrotFace.x, vrotFace.y, vrotFace.z);
			bool reducing = true;
			if(masterAngle == 0){
				goBackb = false;
				masterAngle = 0;
				locked = true;
			}else if(masterAngle < 0 ){

				while (reducing){
					if((masterAngle + stepSize) > 0){
						//overshoots
						//reduce stepZise
						stepSize = ceil(stepSize/2);
					}else{
						//move can be made
						masterAngle +=stepSize;
						for (int j=0; j < numObjs; j++){
							double aux =  ofDegToRad(stepSize);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
							//myMeshs[j].updatePosition(protFaceV, vrotFaceV, stepSize);
						}
						reducing = false;
					}
				}
				active = false;
			}else if(masterAngle > 0 ){

				while (reducing){
					if((masterAngle - stepSize) < 0){
						//we cant move that amount,,, reduce it
						stepSize =  ceil(stepSize/2);
					}else{
						masterAngle -=stepSize;
						for (int j=0; j < numObjs; j++){
							double aux =  ofDegToRad(stepSize);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,-aux);
							//myMeshs[j].updatePosition(protFaceV, vrotFaceV, -stepSize);
						}
						reducing = false;
					}
				}
				active = false;
			}
			//}
		}
		if(moving==true){

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//do 90 deg animation to new position
			//for (int j=0; j < numObjs; j++){
			SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};
			//ofVec3f protFaceV(pointRotate.x, pointRotate.y, pointRotate.z);
			//ofVec3f vrotFaceV(vrotFace.x, vrotFace.y, vrotFace.z);
			bool reducing = true;
			//////if(masterAngle == 90 || masterAngle == -90){
			//////	moving = false;
			//////	masterAngle = 0;
			//////}else if(masterAngle >= 45 ){
			//////	while (reducing){
			//////		if((masterAngle + stepSize) > 90){
			//////			//overshoots
			//////			//reduce stepZise
			//////			stepSize = ceil(stepSize/2);
			//////		}else{
			//////			//move can be made
			//////			masterAngle +=stepSize;
			//////			myMeshs[j].updatePosition(protFaceV, vrotFaceV, stepSize);
			//////			reducing = false;
			//////		}
			//////	}
			//////}else if(masterAngle < 45 ){
			//////	while (reducing){
			//////		if((masterAngle - stepSize) < -90){
			//////			//we cant move that amount,,, reduce it
			//////			stepSize =  ceil(stepSize/2);
			//////		}else{
			//////			masterAngle -=stepSize;
			//////			myMeshs[j].updatePosition(protFaceV, vrotFaceV, -stepSize);
			//////			reducing = false;
			//////		}
			//////	}
			//////}
			//if(sample==false){
			//	//this should only be sampled once during the animation
			//	rotXa = 0;//myMatrix.at(myMatrix.size()-2).deg;
			//	sample=true;
			//}
			if(dir == true){
				//animate rotation xc
				//tempDeg2 = 90-masterAngle;
				//if(movingXC == true){
				while (reducing){
					if(masterAngle==90){
						moving = false;
						masterAngle = 0;
						reducing = false;
						locked = true;
					}else if(masterAngle + stepSize > 90){
						//overshoots
						//reduce stepZise
						stepSize =  ceil(stepSize/2);
					}else{
						//move can be made
						masterAngle +=stepSize;
						for (int j=0; j < numObjs; j++){
							double aux =  ofDegToRad(stepSize);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
							//myMeshs[j].updatePosition(protFaceV, vrotFaceV, stepSize);
						}
						reducing = false;
					}

				}
				active = false;
				/*if(rotXa < tempDeg2){*/
				//ct2 = ofGetElapsedTimeMillis();
				//rotXa += stepSize;//animTime;//0.1;//(ct2 - ct1)*((1.57)/animTime);
				//double aux =  ofDegToRad(1);
				/*for (int j=0; j < numObjs; j++){*/
				//objectList[0]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
				//myMeshs[j].updatePosition(protFaceV, vrotFaceV, stepSize);
				//}
				//ct1 = ct2;
				//}else if(rotXa >= tempDeg2){
				//	//////rotXa = tempDeg2;
				//	//////double aux =  ofDegToRad(rotXa);
				//	////////for (int j=0; j < numObjs; j++){
				//	//objectList[0]->GetTempMatrix()->Rotate(protFace,vrotFace,90);
				//	////////}
				//	moving = false;
				//	masterAngle = 0;
				//	//movingXC = false;
				//	//if(undoing == true){
				//	//	//pop 2 histories on this cubie
				//	//	//this will leave us in the same position
				//	//	myMatrix.pop_back();
				//	//	myMatrix.pop_back();
				//	//	undoing=false;
				//	//}
				//}
				//}
			}else{
				//xcc
				while (reducing){
					if(masterAngle==-90){
						moving = false;
						masterAngle = 0;
						reducing = false;
						locked = true;
					}else if(masterAngle - stepSize < -90){
						//overshoots
						//reduce stepZise
						stepSize =  ceil(stepSize/2);
					}else{
						//move can be made
						masterAngle -=stepSize;
						for (int j=0; j < numObjs; j++){
							double aux =  ofDegToRad(stepSize);
							objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,-aux);
							//myMeshs[j].updatePosition(protFaceV, vrotFaceV, -stepSize);
						}
						reducing = false;
					}
				}
				active = false;
				//tempDeg2 = -90-masterAngle;
				////if(movingXCC == true){
				//if(rotXa > tempDeg2){
				//	//ct2 = ofGetElapsedTimeMillis();
				//	rotXa -= stepSize;//animTime;//0.1;  //(ct2 - ct1)*((1.57)/animTime);
				//	//double aux =  ofDegToRad(-1);
				//	//////for (int j=0; j < numObjs; j++){
				//	//objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
				//	myMeshs[j].updatePosition(protFaceV, vrotFaceV, -stepSize);
				//	//////}
				//	//ct1 = ct2;
				//}else  if(rotXa <= tempDeg2){
				//	//////rotXa = tempDeg2;
				//	//////double aux =  ofDegToRad(rotXa);
				//	//////for (int j=0; j < numObjs; j++){
				//	//////	objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
				//	//////}
				//	moving = false;
				//	masterAngle = 0;
				//	//movingXCC = false;
				//	//if(undoing == true){
				//	//	//pop 2 histories on this cubie
				//	//	//this wwill leave us in the same position
				//	//	myMatrix.pop_back();
				//	//	myMatrix.pop_back();
				//	//	undoing=false;
				//	//}
				//}
				//}
			}
			//}

			/////////////////////////////////////USING HISTORY VECTOR
			////if(myMatrix.size()>=2){
			////	for(int i=0; i<myMatrix.size()-1;i++){
			////		//build rotation matrix for all steps up to the one where it was at the moment of a new movement
			////		SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
			////		SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis; //rotate to do a face move
			////		double d = myMatrix.at(i).deg;
			////		d = ofDegToRad(d);
			////		if(myMatrix.at(i).dir == true){
			////			//c
			////			for (int j=0; j < numObjs; j++){
			////				if (objectList[j]->GetTempMatrix()==0){
			////					objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			////				}else{
			////					objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			////				}
			////			}
			////		}else{
			////			//cc
			////			for (int j=0; j < numObjs; j++){
			////				if (objectList[j]->GetTempMatrix()==0){
			////					objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			////				}else{
			////					objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			////				}
			////			}
			////		}
			////	}
			////	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////	//we are at the last positon
			////	SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
			////	SG_VECTOR vrotFace = myMatrix.at(myMatrix.size()-1).vector;//  axis; //rotate to do a face move
			////	double tempDeg2 = myMatrix.at(myMatrix.size()-1).deg; //target angle, the last angle it will move to
			////	if(sample==false){
			////		//this should only be sampled once during the animation
			////		rotXa = 0;//myMatrix.at(myMatrix.size()-2).deg;
			////		sample=true;
			////	}
			////	if(myMatrix.at(myMatrix.size()-1).dir == true){
			////		//animate rotation xc
			////		//if(movingXC == true){
			////		if(rotXa < tempDeg2){
			////			//ct2 = ofGetElapsedTimeMillis();
			////			rotXa += animTime;//0.1;//(ct2 - ct1)*((1.57)/animTime);
			////			double aux =  ofDegToRad(rotXa);
			////			for (int j=0; j < numObjs; j++){
			////				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
			////			}
			////			//ct1 = ct2;
			////		}else{
			////			rotXa = tempDeg2;
			////			double aux =  ofDegToRad(rotXa);
			////			for (int j=0; j < numObjs; j++){
			////				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
			////			}
			////			moving = false;
			////			//movingXC = false;
			////			if(undoing == true){
			////				//pop 2 histories on this cubie
			////				//this will leave us in the same position
			////				myMatrix.pop_back();
			////				myMatrix.pop_back();
			////				undoing=false;
			////			}
			////		}
			////		//}
			////	}else{
			////		//xcc
			////		//if(movingXCC == true){
			////		if(rotXa > tempDeg2){
			////			//ct2 = ofGetElapsedTimeMillis();
			////			rotXa -= animTime;//0.1;  //(ct2 - ct1)*((1.57)/animTime);
			////			double aux =  ofDegToRad(rotXa);
			////			for (int j=0; j < numObjs; j++){
			////				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
			////			}
			////			//ct1 = ct2;
			////		}else{
			////			rotXa = tempDeg2;
			////			double aux =  ofDegToRad(rotXa);
			////			for (int j=0; j < numObjs; j++){
			////				objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,aux);
			////			}
			////			moving = false;
			////			//movingXCC = false;
			////			if(undoing == true){
			////				//pop 2 histories on this cubie
			////				//this wwill leave us in the same position
			////				myMatrix.pop_back();
			////				myMatrix.pop_back();
			////				undoing=false;
			////			}
			////		}
			////		//}
			////	}
			////}
		}
		else{
			//puzzle is not moving
			///////////////////////////////////applying rotations matrix HISTORY
			//use vector with matrix(s)
			//////for(int i=0; i<myMatrix.size();i++){
			//////	SG_POINT protFace = {pointRotate.x,pointRotate.y,pointRotate.z};										 
			//////	SG_VECTOR vrotFace = myMatrix.at(i).vector;//  axis of rotation
			//////	double d = myMatrix.at(i).deg;
			//////	d = ofDegToRad(d);
			//////	//if(myMatrix.at(i).dir == true){
			//////	//c
			//////	for (int j=0; j < numObjs; j++){
			//////		if (objectList[j]->GetTempMatrix()==0){
			//////			objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//////		}else{
			//////			objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//////		}
			//////	}
			//////	//}else{
			//////	//	//cc
			//////	//	for (int j=0; j < numObjs; j++){
			//////	//		if (objectList[j]->GetTempMatrix()==0){
			//////	//			objectList[j]->InitTempMatrix()->Rotate(protFace,vrotFace,d);
			//////	//		}else{
			//////	//			objectList[j]->GetTempMatrix()->Rotate(protFace,vrotFace,d);
			//////	//		}
			//////	//	}
			//////	//}
			//////}
		}
		///////////////////////////////////////////////////////////
		////////undo rotations of armature in z-y-x order
		//SG_VECTOR vrotZ = {0,0,1};      
		//SG_VECTOR puzzleRotate = {0,0,0};
		//for (int j=0; j < numObjs; j++){
		//	if (objectList[j]->GetTempMatrix()==0){
		//		objectList[j]->InitTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(armRotations.z));
		//	}else{
		//		objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotZ,ofDegToRad(armRotations.z));
		//	}
		//	SG_VECTOR vrotY = {0,1,0}; 							 
		//	objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotY,ofDegToRad(armRotations.y));
		//	SG_VECTOR vrotX = {1,0,0}; 							 
		//	objectList[j]->GetTempMatrix()->Rotate(puzzleRotate,vrotX,ofDegToRad(armRotations.x));
		//	objectList[j]->ApplyTempMatrix();
		//}
	}
	else{

	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::draw(){  
	//each cubie draws its own sgCGroup *objects;
	//now it draws its vector of Vbos, myVbos
	//use this cubies objectList[]
	//if(objects != NULL){
	for (int j=0; j < numObjs; j++){
		glPushMatrix();
		//ofScale(1.2,1.2,1.2);
		if (objectList[j]->GetTempMatrix()!=0)
			glMultMatrixd(objectList[j]->GetTempMatrix()->GetTransparentData());
		//objectList[j]->DestroyTempMatrix();
		if(bDraw){
			if(bDrawWire){
				myMeshs[j].drawWireframe();
			} else {
				//myVbos[j].draw(GL_TRIANGLES, 0,myMeshs[j].getNumIndices());
				myMeshs[j].draw();
			}
		}
		//ofPushMatrix();
		//ofTranslate(centroid3d.x, centroid3d.y, centroid3d.z);
		//ofSetColor(centroidColor.x, centroidColor.y, centroidColor.z);
		//ofDrawSphere(0,0,0,radiurS);
		//centroid2d = projectPoint(centroid3d);
		//ofPopMatrix();

		glPopMatrix();
	}
	//}
}
//------------------------------------------------------------------------------------------------------------------------------------------
void cubie::faceRotate(SG_VECTOR axis,bool di){
	//this function is to handle a face rotation for a cubbie
	//its invoked on a group of cubies determined by the puzzle..??(stil lneeds to be determined)
	//use this cubies objectList to draw elements without ever loosing them on groupBreaking
	////if(objects != NULL){
	//	if(moving == false){
	//		if(undoing == false){
	//			SG_POINT protFace = {0,0,0};
	//			SG_VECTOR vrotFace = axis;
	//			moving = true;
	//			sample = false;
	//			//ct1 = ofGetElapsedTimeMillis();
	//			if(di == true){
	//				//c
	//				myMatrix.push_back(matrix(axis,90,di));
	//			}else{
	//				//cc
	//				myMatrix.push_back(matrix(axis,-90,di));
	//			}
	//		}
	//	}
	////}else{
	////	//cout << "null at face rotation" << endl;
	////}
}

//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::setObjects(sgCGroup *objs,int cubieId,ofVec3f rotationCompensation){
	////it receives a group, when Puzzle loadsPieces(ySlicer->getPieces())  on main
	////it takes the input group and breaks it, to put parts on cubie group "objects"

	armRotations = (rotationCompensation);

	if(objs != NULL){
		//sgCObject **objcts = (sgCObject**)malloc(50*sizeof(sgCObject*));
		//int objctr = 0;
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
			//sgC3DObject *temp = (sgC3DObject*) allParts[j];
			//put clone on *[] tomake new group
			objectList[j] = (sgC3DObject*)allParts[j]->Clone();
			//objcts[objctr] = (sgC3DObject*)temp->Clone();
			//objctr ++;
		}
		free(allParts);
		//put that new group inside *objects of this class, of every cubie
		//objects = sgCGroup::CreateGroup(objcts,objctr);
		//free(objcts);
	}else{
		numObjs = 0;
		//objects = NULL;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::crateOfMeshs(){
	CubieMesh tempMesh;
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)

	//if(objects != NULL){
	for (int j=0; j < numObjs; j++){
		//////////////////////create ofMEsh
		sgC3DObject *o = (sgC3DObject*)objectList[j]->Clone();
		o->Triangulate(SG_VERTEX_TRIANGULATION);
		//convert to ofMEsh with cubie ID!!!
		//ask if its a plain color puzzle:: bunny torus??
		//if(selectedObjectID == 1){
		//	//torus
		//	ofr->sgCoretoOFmesh(o,tempMesh,-1,selectedObjectID);
		//}else if(selectedObjectID == 4){
		//	//bunny
		//	ofr->sgCoretoOFmesh(o,tempMesh,-3,selectedObjectID);
		//}else{
		ofr->sgCoretoOFmesh(o,tempMesh,id,selectedObjectID,"no"); //give cubie id!! so that it knows if its a plain color puzzle or not
		//}
		myMeshs.push_back(tempMesh);
		ofVbo tempVbo;
		tempVbo.setMesh(tempMesh, GL_STATIC_DRAW);
		myVbos.push_back(tempVbo);
		sgDeleteObject(o);
	}
	//}
	getCentroid();
	free(ofr);
}
//--------------------------------------------------------------------------------------
void cubie::getCentroid(){
	centroid3d.set(0,0,0);
	int count = 0;
	for(auto mIter = myMeshs.begin(); mIter != myMeshs.end(); mIter++){
		ofVec3f tVec = mIter->getCentroid();
		centroid3d += tVec;
		count++;
	}
	centroid3d /= count;
	//cout << "Centroid = " << centroid3d.x << ", " << centroid3d.y << ", " << centroid3d.z << endl;
	//cout << count << endl;
	centroidColor.set(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255));
	//centroidColor.set(255,255,255);
	if (numObjs>0) {
		SG_POINT cpt = {centroid3d.x,centroid3d.y,centroid3d.z};

		objectList[0]->GetTempMatrix()->ApplyMatrixToPoint(cpt);

		centroid3d.x=cpt.x;
		centroid3d.y=cpt.y;
		centroid3d.z=cpt.z;
	}
}
//--------------------------------------------------------------------------------------
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

void cubie::printCurrentCentroid(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	ofMatrix4x4 proj, model;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	proj.set(projection);
	model.set(modelview);

	ofVec3f currentCentroid = centroid3d;
}

ofVec3f cubie::getUnprojectedPoint(){
	bHavePoint = false;
	return unprojectedPoint;
}

void cubie::setMousePoint(ofVec3f _pnt){
	if(!bUnproject){
		cout << "cubie setting mouse point." << endl;
		point = _pnt;
		bUnproject = true;
	}
}
//------------------------------------------------------------------------------------------------------------
float cubie::getDistanceByVertex(ofVec3f _pos){
	float nearestDist = 10000;
	ofVec3f nearestVertex;
	int nearestMesh = 0;
	int counter = 0;
	for(int i = 0; i < myMeshs.size(); i++){
		int numVertices = myMeshs[i].getNumVertices();
		for(int j = 0; j < numVertices; j++){
			glPushMatrix();
			if (objectList[i]->GetTempMatrix()==0){
				objectList[i]->InitTempMatrix();
			}
			glMultMatrixd(objectList[i]->GetTempMatrix()->GetTransparentData());
			objectList[i]->DestroyTempMatrix();
			ofVec3f vScreen = projectPoint(myMeshs[i].getVertex(j));
			glPopMatrix();
			float cDist = _pos.distance(vScreen);
			//cout << "comparing " << _pos.x << ", " << _pos.y << " against " << vScreen.x << ", " << vScreen.y << endl;
			if(cDist < nearestDist){
				nearestDist = cDist;
				nearestVertex = myMeshs[i].getVertex(i);
				nearestMesh = counter;
			}
		}
		counter++;
	}
	selectedVertex = nearestVertex;
	selectedMesh = nearestMesh;
	return nearestDist;
}
//--------------------------------------------------------------------------------------------------------------
void cubie::dragInput(ofVec3f _pnt){
	ofVec3f pnt = _pnt.normalize();
	float angle = 0;
	if(abs(pnt.x) > abs(pnt.y) && abs(pnt.x) > abs(pnt.z)){
		angle = pnt.x;
		pnt.x = 0;
		pnt.z = 0;
		pnt.y = 1;
	}
	if(abs(pnt.y) > abs(pnt.x) && abs(pnt.y) > abs(pnt.z)){
		angle = pnt.y;
		pnt.y = 0;
		pnt.z = 0;
		pnt.x = 1;
	}
	if(abs(pnt.z) > abs(pnt.x) && abs(pnt.z) > abs(pnt.y)){
		angle = pnt.z;
		pnt.x = 0;
		pnt.z = 0;
		pnt.y = 1;
	}
	//angle = 2.0;
	cout << "Cubie " << id << " got drag - " << pnt.x << ", " << pnt.y << ", " << pnt.z << " angle = " << angle << endl; 
	cout << "Cubie " << id << " from - " << _pnt.x << ", " << _pnt.y << ", " << _pnt.z << endl; 
	//cout << "Cubie " << id << " got drag - " << pnt.x << ", " << pnt.y << ", " << pnt.z << " angle = " << angle << endl; 
	//SG_VECTOR v = {pnt.x, pnt.y, pnt.z};
	//faceRotate(v, true, angle);
}

float cubie::getDistanceByCentroid(ofVec3f _pos){
	getCentroid();
	return centroid3d.distance(_pos);
}

Triangle cubie::getNearestTri(ofVec3f _pnt){
	if (myMeshs.size()==0) {
		Triangle tri(0,1,2,ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(0,0,1),ofFloatColor(0,1));
		return tri;
	}
	nearestTri = myMeshs[0].getNearest(_pnt);
	return nearestTri;
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

void cubie::setColorToSet(vector<Triangle> _tris, ofFloatColor _c){
	for(int i = 0; i < myMeshs.size(); i++){
		myMeshs[i].setColorToSet(_tris, _c);
		ofVbo tempVbo;
		tempVbo.setMesh(myMeshs[i], GL_STATIC_DRAW);
		myVbos[i]=tempVbo;
	}
}

void cubie::setColorToSetArmature(ofFloatColor _c){
	for(int i = 0; i < myMeshs.size(); i++){
		myMeshs[i].setColorToSetArmature(_c);
		ofVbo tempVbo;
		tempVbo.setMesh(myMeshs[i], GL_STATIC_DRAW);
		myVbos[i]=tempVbo;
	}
}

void cubie::setColorToCurvedObject(ofFloatColor _c){
	for(int i = 0; i < myMeshs.size(); i++){
		myMeshs[i].setColorToCurvedObject(_c);
		//need to recolor black faces
		ofVbo tempVbo;
		tempVbo.setMesh(myMeshs[i], GL_STATIC_DRAW);
		myVbos[i]=tempVbo;
	}
}

//void cubie::updatePosition(){
//	ofVec3f axis(vrotFace.x, vrotFace.y, vrotFace.z);
//	float angle = 0.0;
//	if(dir){
//		angle = 90.0;
//	} else {
//		angle = -90.0;
//	}
//	for(auto cIter = myMeshs.begin(); cIter != myMeshs.end(); cIter++){
//cIter->rotateNormals(angle, axis);
//		cIter->rotateVertices(angle, axis);
//	}
//}

//-------------------------------------------------------------------------------------------------------------------------------------------
void cubie::move(SG_VECTOR p){
	pos = p;
}
//--------------------------------------------------------------
int cubie::getId(){
	return id;
}
//-------------------------------------------------------------
int cubie::getXpos(){
	return xpos;
}
//-------------------------------------------------------------
int cubie::getYpos(){
	return ypos;
}
//-------------------------------------------------------------
int cubie::getZpos(){
	return zpos;
}
//-------------------------------------------------------------
int cubie::getWpos(){
	return wpos;
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
//--------------------------------------------------------------
void cubie::unDoMenuPuzzle(){
	//it removes the las element on the vector with the history of each cubie
	if(myMatrix.size()>1){
		for (int j=0; j < numObjs; j++){
			if (objectList[j]->GetTempMatrix()!=0)
				objectList[j]->DestroyTempMatrix();
		}
		setup();//to restore initial TempMatrix with possible armature rotationss

		//have to reset the position of each cubie so puzzle continues to be stable on next runs
		zpos=id%gridSize;
		ypos=(id/gridSize)%gridSize;
		xpos=(id/(gridSize*gridSize))%gridSize;
	}
}
//---------------------------------------------------------------
//void cubie::changeColorToColor(ofFloatColor Sc, ofFloatColor Tc){
//	ofRender *ofr = new ofRender(); 
//	for(int j=0; j< numObjs; j++){
//		ofr->changeColorToColor(Sc,Tc,myMeshs[j]);
//		//have to replace the vbo
//		ofVbo tempVbo;
//		tempVbo.setMesh(myMeshs[j], GL_STATIC_DRAW);
//		myVbos[j]=tempVbo;
//	}
//
//	free(ofr);
//}
//-----------------------------------------------------------------
void cubie::colorBlackSides(int idCubie, float playRoom){
	//have key sides of cubie colored black
	ofRender *ofr = new ofRender(); 
	for(int j=0; j< numObjs; j++){
		//ofr->colorBlackSides(myMeshs[j],idCubie,playRoom,selectedObjectID);
		ofr->colorBlackSidesFromAxes(myMeshs[j],xpos,ypos,zpos,gridSize,playRoom,armRotations);
		//have to replace the vbo
		ofVbo tempVbo;
		tempVbo.setMesh(myMeshs[j], GL_STATIC_DRAW);
		myVbos[j]=tempVbo;
	}

	free(ofr);
}
//--------------------------------------------------------------------
void cubie::colorTorus(){
	//set random color for this cubie surface
	ofRender *ofr = new ofRender(); 
	for(int j=0; j< numObjs; j++){
		ofr->colorTorus(myMeshs[j]);
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
	if(moving || undoing || goBackb){
		return true;
	}
	return false;
}
//----------------------------------------------------------------
void cubie::exit(){
	//if(objects != NULL){
	for (int j=0; j < numObjs; j++){
		sgCObject::DeleteObject(objectList[j]);
	}
	//free(objectList);
	//sgCObject::DeleteObject(objects);
	//}
}
//----------------------------------------------------------------
