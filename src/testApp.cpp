#include "testApp.h"
#include "myobject3D.h"
#include "cutter.h"
#include "armature.h"
///////////////////////////////////////////
#include "Painter.h"
#include "sgCore.h"
///////////////////////////////////////////
#define planeThicknes 5.1
#define planeSize 450
#define tamPiece 30
//--------------------------------------------------------------
void testApp::setup(){
	/////initialize sgCore library
	sgInitKernel();  

	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D();
	objectDisplayed->loadObject(*(sgCObject*)sgCreateTorus(100,20,24,24));
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
	/////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////create cutter//////////////////////////////////////
	normalCutter = new cutter(planeThicknes,planeSize,tamPiece);
	normalCutter->setup();
	/////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////create slicer//////////////////////////////////////
	mySlicer = new slicer(normalCutter);
	mySlicer->setup();
	/////////////////////////////////////////////////////////////////////////////////


	////BOOLEAN SUBSTRACTION////////////////////////////////Make all the CUTS//////////////////////////////
	//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////
	//for testing reasosn., we know we should be getting one object on
	//wantedObjects[0]
	/*sgCObject *result = (sgCObject*) mySlicer->wantedObjects[0]->Clone();
	SG_VECTOR transBox1 = {300,0,0}; 
	result->InitTempMatrix()->Translate(transBox1);
	SG_VECTOR transBox12 = {0,300,0}; 
	result->GetTempMatrix()->Translate(transBox12);
	result->ApplyTempMatrix();  
	result->DestroyTempMatrix();
	result->SetAttribute(SG_OA_COLOR,8);
	sgGetScene()->AttachObject(result);*/
	//////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
	//more testing.. to see if the piece is getting wrapped on the **group
	sgCGroup *result2 = (sgCGroup*) mySlicer->pieces[0]; //cubie 1
	const int ChCnt = result2->GetChildrenList()->GetCount();
	sgCObject** allChilds3 = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
	result2->BreakGroup(allChilds3);// ->BreakGroup(allChilds3);
	sgCObject::DeleteObject(result2);

	for (int j=0; j < ChCnt; j++){
		SG_VECTOR transBox11 = {500,0,0}; 
		allChilds3[j]->InitTempMatrix()->Translate(transBox11);
		SG_VECTOR transBox121 = {0,500,0}; 
		allChilds3[j]->GetTempMatrix()->Translate(transBox121);
		allChilds3[j]->ApplyTempMatrix();  
		allChilds3[j]->DestroyTempMatrix();
		allChilds3[j]->SetAttribute(SG_OA_COLOR,8);
		sgGetScene()->AttachObject(allChilds3[j]);
	}
	free(allChilds3);
	///////////////////////////////////////////////////////////////////////////////////////
	
}
//--------------------------------------------------------------
void testApp::update(){
	////////put all elements on the scene/////////////////
	//add cutter to scene
    normalCutter->update();
	 

	//add original object to scene
	//objectDisplayed->update();
	//sgGetScene()->AttachObject((sgCObject*)objectDisplayed->getObject());

   //sgGetScene()->AttachObject(sub1); 
   //sub1->SetAttribute(SG_OA_COLOR,3);

   //sgGetScene()->AttachObject(add); 
   //add->SetAttribute(SG_OA_COLOR,12);
}

//--------------------------------------------------------------
void testApp::draw(){

  sgGetScene()->AttachObject(objectDisplayed->getObject());
  
  addGroupToScene((sgCGroup*)normalCutter->getCutter());
  ////////////////////////////////////////////////////////////
  //draw the elements of the scene
  sgCObject*  curObj = sgGetScene()->GetObjectsList()->GetHead();
  while (curObj)
  {
    if (true)
    {
      Painter::DrawObject(GL_RENDER,curObj,false);

      if ((curObj->GetAttribute(SG_OA_DRAW_STATE) & SG_DS_GABARITE))
      {
        SG_POINT a1,a2;
        curObj->GetGabarits(a1,a2);
        Painter::DrawGabariteBox(a1,a2);
      }
    }
    curObj = sgGetScene()->GetObjectsList()->GetNext(curObj);
  }

  //sgGetScene()->Clear();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}
/////////////////////////////////////////////////////////////////////////////
//////////////////////CUSTOM TWISTY FUNCTIONS////////////////////////////////
void testApp::addGroupToScene(sgCGroup *group){
   const int ChildsCount = group->GetChildrenList()->GetCount();  
   sgCObject**  allChilds = (sgCObject**)malloc(ChildsCount*sizeof(sgCObject*));  

   if (!group->BreakGroup(allChilds)){  
     //assert(0);  
   }  
   const int sz = group->GetChildrenList()->GetCount();  
   //assert(sz==0);  
   sgDeleteObject(group);  
   for (int i=0;i<ChildsCount;i++){  
        sgGetScene()->AttachObject(allChilds[i]);  
   }  
   free(allChilds);  
}