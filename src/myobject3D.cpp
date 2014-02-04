#include "myobject3D.h"
#include "sgCore.h"

myobject3D::myobject3D(){
}
//--------------------------------------------------------------
void myobject3D::setup(){
}
//--------------------------------------------------------------
void myobject3D::update(){
	SG_POINT rot27 = {0,0,0};
	SG_VECTOR rotD27 = {0,-1,0};
	object->InitTempMatrix()->Rotate(rot27,rotD27,0.01);
	SG_VECTOR rotD = {0,0,0};
	object->GetTempMatrix()->Translate(rotD);
	object->ApplyTempMatrix();  
	object->DestroyTempMatrix();
}
//--------------------------------------------------------------
void myobject3D::draw(){  
	ofPushMatrix();
	ofTranslate(0,0);
	
	ofCircle(ofPoint(100,100),30);
	sgGetScene()->AttachObject(object);
	ofPopMatrix();
	
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