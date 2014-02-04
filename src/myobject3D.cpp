#include "myobject3D.h"
#include "sgCore.h"

myobject3D::myobject3D(float x, float y){
	posX = x;
	posY = y;
}
//--------------------------------------------------------------
void myobject3D::setup(){
	/*SG_VECTOR rotD = {500,500,0};
	object->InitTempMatrix()->Translate(rotD);
	object->ApplyTempMatrix();  
	object->DestroyTempMatrix();*/
	temp = (sgC3DObject *) object->Clone();
	SG_VECTOR rotD = {posX,posY,0};
	temp->InitTempMatrix()->Translate(rotD);
	temp->ApplyTempMatrix();  
	temp->DestroyTempMatrix();
}
//--------------------------------------------------------------
void myobject3D::update(){
	//glPushMatrix();
	SG_POINT rotP = {posX,posY,0};
	SG_POINT rotPobject = {0,0,0};
	SG_VECTOR rotV = {0,1,0};
	temp->InitTempMatrix()->Rotate(rotP,rotV,0.01);
	object->InitTempMatrix()->Rotate(rotPobject,rotV,0.01);
	SG_VECTOR rotD = {0,0,0};
	temp->GetTempMatrix()->Translate(rotD);
	SG_VECTOR rotDobject = {0,0,0};
	object->GetTempMatrix()->Translate(rotDobject);
	temp->ApplyTempMatrix();  
	object->ApplyTempMatrix();
	temp->DestroyTempMatrix();
	object->DestroyTempMatrix();
	//glPopMatrix();
}
//--------------------------------------------------------------
void myobject3D::draw(){  
	//ofPushMatrix();
		//ofTranslate(0,0);
		ofCircle(ofPoint(100,100),30);
		//temp->Triangulate(SG_VERTEX_TRIANGULATION);
		sgGetScene()->AttachObject(temp);
	//ofPopMatrix();
	reset();
}
//--------------------------------------------------------------
void myobject3D::reset(){  
/*	SG_VECTOR rotD = {-500,-500,0};
	object->InitTempMatrix()->Translate(rotD);
	object->ApplyTempMatrix();  
	object->DestroyTempMatrix()*/;
}
//--------------------------------------------------------------
void myobject3D::unDraw(){  
	sgGetScene()->DetachObject(temp);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void myobject3D::loadObjectFromFile(const char* pathTofile){ 
	//it will load a stl file into object
	sgFileManager::ImportSTL(sgCScene::GetScene(), pathTofile);
	cout << "draw: " << sgGetScene()->GetObjectsList()->GetCount() << endl;
}
//----------------------------------------------------------------
void myobject3D::loadObject(sgCObject *obj){
	//it will load a sgCore lib object: torus, box
	object = obj;
}
//----------------------------------------------------------------
sgCObject* myobject3D::getObject(){
	//it returns a clone of the object
	//NEVER return the real object
	return object;
}