#include "myobject3D.h"
#include "sgCore.h"

myobject3D::myobject3D(){
}
//--------------------------------------------------------------
void myobject3D::setup(){
}
//--------------------------------------------------------------
void myobject3D::update(){
}
//--------------------------------------------------------------
void myobject3D::draw(){  
   SG_VECTOR transV3 = {250,250,0};  
   object->InitTempMatrix()->Translate(transV3);  
   object->ApplyTempMatrix();  
   object->DestroyTempMatrix();

   object->SetAttribute(SG_OA_COLOR,5);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void myobject3D::loadObjectFromFile(const char* pathTofile){ 
	//it will load a stl file into object
	sgFileManager::ImportSTL(sgCScene::GetScene(), pathTofile);
	cout << "draw: " << sgGetScene()->GetObjectsList()->GetCount() << endl;
}
//----------------------------------------------------------------
void myobject3D::loadObject(sgCObject &obj){
	//it will load a sgCore lib object: torus, box
	object = &obj;
}
//----------------------------------------------------------------
sgCObject* myobject3D::getObject(){
	//it returns a clone of the object
	//NEVER return the real object
	return object->Clone();
}