#include "testApp.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

///////////////////////////////////////////
#include "Painter.h"
#include "sgCore.h"
///////////////////////////////////////////
#define planeThicknes 0.1
#define planeSize 500
#define tamCubie 100
//--------------------------------------------------------------
void testApp::setup(){
	makeCut =false;
	/////initialize sgCore library
	sgInitKernel();  

	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING//////////////////////////////////////
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D();
	objectDisplayed->loadObject(*(sgCObject*)sgCreateTorus(100,80,24,24));
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
	///////////////////////end create slicer /////////////////////////////////////////

	//////////////////////////////create puzzle///////////////////////////////////////
	myPuzzle = new puzzle();
	myPuzzle->setup();
	//////////////////////////////end create puzzle////////////////////////////////////

}
//--------------------------------------------------------------
void testApp::update(){
	////////update cutter/////////////////
	myCutter->update();


	//add original object to scene
	//objectDisplayed->update();
	//sgGetScene()->AttachObject((sgCObject*)objectDisplayed->getObject());

	//sgGetScene()->AttachObject(sub1); 
	//sub1->SetAttribute(SG_OA_COLOR,3);

	//sgGetScene()->AttachObject(add); 
	//add->SetAttribute(SG_OA_COLOR,12);


	if(makeCut==true){
		////BOOLEAN SUBSTRACTION//////////////////////////////////////////////////////////
		//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
		///boolean INTERSECTION///////////////////////////////////////////////////////////
		mySlicer->intersectCubes(mySlicer->myCutter,objectDisplayed->getObject()); 

		//////////////////////////////create puzzle///////////////////////////////////////
		//sgCGroup ** aux = (sgCGroup**) mySlicer->pieces; //WHY CANT I GET THE DATA ** HERE??? its a group**!!
		//sgCGroup ** aux = (sgCGroup**) mySlicer->getPieces(); //WHY CANT I GET THE DATA ** HERE??? on the SETUP?? now on the update I can...!!!
		//sgCGroup * aux2 = (sgCGroup*) mySlicer->pieces[6]; //AND HERE IT IS!!! ->> why???
		myPuzzle->loadPieces(mySlicer->getPieces());
		//////////////////////////////end create puzzle////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		makeCut = false;
		cout << "end cut:" << ofGetElapsedTimeMillis() << endl;
		//Draw the pieces!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		sgCGroup **aux = (sgCGroup**)malloc(27*sizeof(sgCGroup*));
		aux = mySlicer->getPieces();
		for(int i=0; i<27 ; i++){
			if(aux[i] != NULL){
				sgCGroup *result2 = (sgCGroup*) aux[i];  //WHY am I loosing the data here?? do I have to make a copy always before doing this?? how????
				const int ChCnt = result2->GetChildrenList()->GetCount();
				sgCObject** allChilds3a = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
				result2->BreakGroup(allChilds3a);// ->BreakGroup(allChilds3);
				sgCObject::DeleteObject(result2);
				for (int j=0; j < ChCnt; j++){
					SG_VECTOR rotD = {0,1,0};
					SG_POINT rot = {0,0,0};
					allChilds3a[j]->InitTempMatrix()->Rotate(rot,rotD,1.0);
					SG_VECTOR transBox11 = {500,0,0}; 
					allChilds3a[j]->GetTempMatrix()->Translate(transBox11);
					SG_VECTOR transBox121 = {0,500,0}; 
					allChilds3a[j]->GetTempMatrix()->Translate(transBox121);
					allChilds3a[j]->ApplyTempMatrix();  
					allChilds3a[j]->DestroyTempMatrix();
					allChilds3a[j]->SetAttribute(SG_OA_COLOR,rand()%50);
					sgGetScene()->AttachObject(allChilds3a[j]);
					break;
				}
				free(allChilds3a);
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	/*SG_VECTOR rotD = {0,1,0};
	SG_POINT rot = {500,500,0};
	objectDisplayed->getObject()->InitTempMatrix()->Rotate(rot,rotD,0.0); 
	objectDisplayed->getObject()->ApplyTempMatrix();  
	objectDisplayed->getObject()->DestroyTempMatrix();*/
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
	cout << "init cut: " << ofGetElapsedTimeMillis() << endl;
	makeCut = true;
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