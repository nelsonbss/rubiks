#include "myobject3D.h"
#include "sgCore.h"
#include "ofRender.h"

myobject3D::myobject3D(float x, float y){
	posX = x;
	posY = y;
	deg = 0.0;
}
//--------------------------------------------------------------
void myobject3D::setup(){
	temp = (sgC3DObject *) object->Clone();
	/*SG_VECTOR rotD = {posX,posY,0};
	temp->InitTempMatrix()->Translate(rotD);
	temp->ApplyTempMatrix();  */
	//temp->DestroyTempMatrix();
	//object->Triangulate(SG_VERTEX_TRIANGULATION);

	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	temp->SetAttribute(SG_OA_COLOR,2);
	ofRender *ofr = new ofRender(); //class that has the metods to transform sgCore to OF mesh and set the normals (in one function)
	ofr->sgCoretoOFmesh(temp,myMesh);
	free(ofr);
}
//--------------------------------------------------------------
void myobject3D::update(){
	//SG_POINT rotP = {posX,posY,0};
	SG_POINT rotP = {0,0,0};
	SG_VECTOR rotV = {0,1,0};
	//SG_POINT rotPobject = {posX,posY,0};
	//SG_VECTOR rotDobject = {0,0,0};
	deg += 0.01;
	temp->InitTempMatrix()->Rotate(rotP,rotV,deg);
	SG_VECTOR transP = {posX,posY,0};
	temp->GetTempMatrix()->Translate(transP);
	temp->ApplyTempMatrix();  
	//temp->DestroyTempMatrix();

	//object->DestroyTempMatrix();
	//object->InitTempMatrix()->Rotate(rotPobject,rotV,0.01);
	//object->GetTempMatrix()->Translate(rotD);
	//object->GetTempMatrix()->Translate(rotDobject);
	//object->ApplyTempMatrix();
	//object->DestroyTempMatrix();

}
//--------------------------------------------------------------
void myobject3D::draw(){
	//ofTranslate(0,0);
	ofSetColor(ofColor(255,0,0));
	ofCircle(ofPoint(100,100),5);
	//sgGetScene()->AttachObject(temp);
	//instead of attaching to scene
	ofPushMatrix();
	//ofTranslate(posX,posY);
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	myMesh.draw();
	ofPopMatrix();
	temp->DestroyTempMatrix();

	//object->SetAttribute(SG_OA_COLOR,5);
	////object->Triangulate(SG_VERTEX_TRIANGULATION);
	////SG_VECTOR rotD = {posX,posY,0};
	////object->InitTempMatrix()->Translate(rotD);
	////object->ApplyTempMatrix();  
	////object->DestroyTempMatrix();
	//sgGetScene()->AttachObject(object);
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
void myobject3D::loadObject(sgC3DObject *obj){
	//it will load a sgCore lib object: torus, box
	object = obj;
}
//----------------------------------------------------------------
sgCObject* myobject3D::getObject(){
	//it returns a clone of the object
	//NEVER return the real object
	return object;
}
//----------------------------------------------------------------
void myobject3D::exit(){
	sgCObject::DeleteObject(temp);
	sgCObject::DeleteObject(object);
}
