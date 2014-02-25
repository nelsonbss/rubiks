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

	deg = 0.0;
	id=0;
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

		/*SG_VECTOR rotD = {posX,posY,0};
		temp->InitTempMatrix()->Translate(rotD);
		temp->ApplyTempMatrix();  
		temp->DestroyTempMatrix();*/

		temp->Triangulate(SG_VERTEX_TRIANGULATION);
		temp->SetAttribute(SG_OA_COLOR,2);
		ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
		//ofr->sgCoretoOFmesh(temp,myMesh,-1,true); //-1 because its not a cubie but want color
		ofr->sgCoretoOFmesh(temp,myMesh,-2,true); //-2 for plain color
		myVbo.setMesh(myMesh, GL_STATIC_DRAW);
		free(ofr);


}
//--------------------------------------------------------------
void myobject3D::update(){

	SG_POINT rotP = {0,0,0};
	SG_VECTOR rotV = {0,1,0};
	//deg += 0.01;
	temp->InitTempMatrix()->Rotate(rotP,rotV,deg);
	SG_VECTOR transP = {tempPos.x,tempPos.y,tempPos.z};
	temp->GetTempMatrix()->Translate(transP);
	if(id == 2){
		//cube
		SG_VECTOR offset = {-150,-150,-150}; //for the cube to be in place
		temp->GetTempMatrix()->Translate(offset);//this translates the object to be cut!!
	}
	if(id == 4){
		//rabbit
		SG_POINT rotP2 = transP;//{tempPos.x,tempPos.y,tempPos.z};
		SG_VECTOR rotV2 = {1,0,0};
		temp->GetTempMatrix()->Rotate(rotP2,rotV2,ofDegToRad(180));
	}
	temp->ApplyTempMatrix();  
}
//--------------------------------------------------------------
void myobject3D::draw(){
	//ofTranslate(0,0);
	//ofSetColor(ofColor(255,0,0));
	//ofCircle(ofPoint(100,100),5);
	//sgGetScene()->AttachObject(temp);
	//instead of attaching to scene

	ofPushMatrix();
	//ofScale(1.2,1.2,1.2);
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	temp->DestroyTempMatrix();
	myVbo.draw(GL_TRIANGLES, 0,myMesh.getNumIndices());
	ofPopMatrix();
}
//--------------------------------------------------------------
void myobject3D::unDraw(){  
	//sgGetScene()->DetachObject(temp);
	//sgGetScene()->DetachObject(object);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void myobject3D::loadObjectFromFile(const char* pathTofile){ 
	//it will load a stl file into object
	sgFileManager::ImportSTL(sgCScene::GetScene(), pathTofile);
	cout << "object from file: " << sgGetScene()->GetObjectsList()->GetCount() << endl;
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
		id = ID;
		if(id == 2){
			//cube
			SG_VECTOR offset = {-150,-150,-150}; //for the cube to be in place
			object->InitTempMatrix()->Translate(offset);//this translates the object to be cut!!
			object->ApplyTempMatrix();  
			object->DestroyTempMatrix();
		}
		if(id == 4){
			//rabbit
			SG_POINT rotP = {0,0,0};
			SG_VECTOR rotV = {1,0,0};
			object->InitTempMatrix()->Rotate(rotP,rotV,ofDegToRad(180));
			/*SG_VECTOR offset = {0,800,200}; 
			object->GetTempMatrix()->Translate(offset);*/
			object->ApplyTempMatrix();  
			object->DestroyTempMatrix();
		}

}
//----------------------------------------------------------------
sgCObject* myobject3D::getObject(){
	return object;
}
//----------------------------------------------------------------
void myobject3D::exit(){
	sgCObject::DeleteObject(temp);
	sgCObject::DeleteObject(object);
}
