#include "myobject3D.h"
#include "sgCore.h"
#include "ofRender.h"

myobject3D::myobject3D(SG_VECTOR p, SG_VECTOR t){
	pos.x = p.x;
	pos.y = p.y;
	pos.z = p.z;

	tempPos.x = t.x;
	tempPos.y = t.y;
	tempPos.z = t.z;

	objectId=0;
	object = NULL;
}
//--------------------------------------------------------------
void myobject3D::setup(){
	//the real object is never rendered or moved::::it is used to make the boolean intersection
	//the rendered and animated object is temp

	temp = (sgC3DObject *) object->Clone();

	SG_VECTOR transP = {pos.x,pos.y,pos.z};
	object->InitTempMatrix()->Translate(transP);//this translates the object to be cut!!
	object->ApplyTempMatrix();  
	object->DestroyTempMatrix();

	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	temp->SetAttribute(SG_OA_COLOR,2);
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
	//ofr->sgCoretoOFmesh(temp,myMesh,-1); //-1 because its not a cubie but want color on the sample object
	ofr->sgCoretoOFmesh(temp,myMesh,-2,objectId); //-2 for plain color
	myVbo.setMesh(myMesh, GL_STATIC_DRAW);
	free(ofr);
}
//--------------------------------------------------------------
void myobject3D::update(){
	SG_VECTOR transP = {tempPos.x,tempPos.y,tempPos.z};
	temp->InitTempMatrix()->Translate(transP);
	if(objectId == 2){
		//cube
		SG_VECTOR offset = {-150,-150,-150}; //for the cube to be in place
		temp->GetTempMatrix()->Translate(offset);//this translates the object to be cut!!
		//apply armature axis rotations (x-y-z) to the real object
		SG_POINT rotP = {0,0,0};
		SG_VECTOR rotV = {1,0,0};
		object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
		SG_VECTOR rotV2 = {0,1,0};
		object->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
		SG_VECTOR rotV3 = {0,0,1};
		object->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
		object->ApplyTempMatrix(); 
		object->DestroyTempMatrix(); 
	}else if(objectId == 3){
		////cone..pyramid
		//SG_POINT rotP = transP;
		//SG_VECTOR rotV = {1,0,0};
		//temp->GetTempMatrix()->Rotate(rotP,rotV,ofDegToRad(90));
		//SG_VECTOR offset = {0,100,0}; 
		//temp->GetTempMatrix()->Translate(offset);
	}else if(objectId == 4){
		//rabbit
		SG_POINT rotP2 = transP;//{tempPos.x,tempPos.y,tempPos.z};
		SG_VECTOR rotV2 = {1,0,0};
		temp->GetTempMatrix()->Rotate(rotP2,rotV2,ofDegToRad(180));
	}else if(objectId == 200){
		//extruded object
		SG_VECTOR offset = {0,150,0}; 
		temp->GetTempMatrix()->Translate(offset);
	}
	temp->ApplyTempMatrix();  
}
//--------------------------------------------------------------
void myobject3D::draw(){
	
	ofPushMatrix();
	//ofScale(1,1,1);
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	temp->DestroyTempMatrix();
	myVbo.draw(GL_TRIANGLES, 0,myMesh.getNumIndices());
	ofPopMatrix();
}
//----------------------------------------------------------------
void myobject3D::loadObject(sgC3DObject *obj, int ID){
	//it will load a sgCore lib object: torus, box

	if(object==NULL){
		object = obj;
	}else{
		sgDeleteObject(object);
		object = obj;
	}
	objectId = ID;
	if(objectId == 1){
		//torus
		SG_VECTOR offset = {0,0,0}; //for torus to be in place, the 
		object->InitTempMatrix()->Translate(offset);//this translates the object to be cut!!
		//SG_POINT rotP = {0,0,0};
		//SG_VECTOR rotV = {1,0,0};
		//object->GetTempMatrix()->Rotate(rotP,rotV,ofDegToRad(45));
		object->ApplyTempMatrix();  
		object->DestroyTempMatrix();
	}else if(objectId == 2){
		//cube
		SG_VECTOR offset = {-150,-150,-150}; //for the cube to be in center  place, it has sides of 300
		object->InitTempMatrix()->Translate(offset);//this translates the object to be cut!!
		object->ApplyTempMatrix();  
		object->DestroyTempMatrix();
	}else if(objectId == 3){
		//cone..pyramid
		/*SG_POINT rotP = {0,0,0};
		SG_VECTOR rotV = {1,0,0};
		object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(90));
		SG_VECTOR offset = {0,100,0}; 
		object->GetTempMatrix()->Translate(offset);
		object->ApplyTempMatrix();  
		object->DestroyTempMatrix();*/
	}else if(objectId == 4){
		//rabbit
		SG_POINT rotP = {0,0,0};
		SG_VECTOR rotV = {1,0,0};
		object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(180));
		/*SG_VECTOR offset = {0,800,200}; 
		object->GetTempMatrix()->Translate(offset);*/
		object->ApplyTempMatrix();  
		object->DestroyTempMatrix();
	}else if(objectId == 200){
		//extruded object
		SG_VECTOR offset = {0,150,0}; 
		object->InitTempMatrix()->Translate(offset);
		object->ApplyTempMatrix();  
		object->DestroyTempMatrix();
	}

}
//----------------------------------------------------------------
sgC3DObject* myobject3D::getObject(){
	return object;
}
//----------------------------------------------------------------
void myobject3D::applyArmRotations(ofVec3f v, float angleA, ofVec3f axistbA, int modei){
	//mode:
	//1 is for regular euler angle rotation inside rotateSlicer
	//2 is for quaternion rotate angle and vector info
	mode = modei;
	if(mode ==1 ){
		armRot = (v)*-1;
		//apply armature axis rotations (x-y-z) to the real object
		SG_POINT rotP = {0,0,0};
		SG_VECTOR rotV = {1,0,0};
		object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(armRot.x));
		SG_VECTOR rotV2 = {0,1,0};
		object->GetTempMatrix()->Rotate(rotP,rotV2,ofDegToRad(armRot.y));
		SG_VECTOR rotV3 = {0,0,1};
		object->GetTempMatrix()->Rotate(rotP,rotV3,ofDegToRad(armRot.z));
		object->ApplyTempMatrix();
		object->DestroyTempMatrix(); 
	}else if (mode == 2){
		quatAngle = angleA;
		SG_POINT rotP = {0,0,0};
		rotVQuat.x = axistbA.x;
		rotVQuat.y = axistbA.y;
		rotVQuat.z = axistbA.z;
		object->InitTempMatrix()->Rotate(rotP,rotVQuat,quatAngle);
		object->ApplyTempMatrix();
		object->DestroyTempMatrix(); 
	}
}
//----------------------------------------------------------------
void myobject3D::exit(){
	sgCObject::DeleteObject(temp);
	sgCObject::DeleteObject(object);
}
