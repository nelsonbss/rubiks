#include "testApp.h"
#include "myobject3D.h"
#include "cutter.h"
#include "armature.h"
///////////////////////////////////////////
#include "Painter.h"
#include "sgCore.h"
///////////////////////////////////////////
#define planeThicknes 0.1
#define planeSize 400
#define tamCubie 80
//--------------------------------------------------------------
void testApp::setup(){
	/////initialize sgCore library
	sgInitKernel();  

	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING//////////////////////////////////////
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D();
	objectDisplayed->loadObject(*(sgCObject*)sgCreateTorus(100,20,24,24));
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1); //to make a plane based cutter
	myCutter->setup();
	//////////////////////////////end create cutter///////////////////////////////////

	
	//////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter);
	mySlicer->setup();
	////BOOLEAN SUBSTRACTION//////////////////////////////////////////////////////////
	//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
	///boolean INTERSECTION///////////////////////////////////////////////////////////
	mySlicer->intersectCubes(mySlicer->myCutter,objectDisplayed->getObject());
	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////end create slicer /////////////////////////////////////////

	//////////////////////////////create puzzle///////////////////////////////////////
	myPuzzle = new puzzle ();
	myPuzzle->loadPieces(mySlicer->getPieces);
	//////////////////////////////end create puzzle///////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//Draw the pieces!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for(int i=0; i<27 ; i++){
		if(mySlicer->pieces[i] != NULL){
			sgCGroup *result2 = (sgCGroup*) mySlicer->pieces[i];
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
				allChilds3[j]->SetAttribute(SG_OA_COLOR,rand()%50);
				sgGetScene()->AttachObject(allChilds3[j]);
			}
			free(allChilds3);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////

}
//--------------------------------------------------------------
void testApp::update(){
	////////put all elements on the scene/////////////////
	//add cutter to scene
	myCutter->update();


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

	//addGroupToScene((sgCGroup*)myCutter->getCutterPlanes());
	addGroupToScene((sgCGroup*)myCutter->getCutterCubes());
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