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
	makeCut = false;
	drawCuts = false;
	/////initialize sgCore library
	sgInitKernel();  

	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING//////////////////////////////////////
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D();
	//objectDisplayed->loadObject(*(sgCObject*)sgCreateTorus(100,80,24,24));
	objectDisplayed->loadObject(*(sgCObject*)sgCreateCone(200,1,300.0, 3));
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
		makeCut = false;

		drawCuts = true;
		cout << "end cut:" << ofGetElapsedTimeMillis() << endl;

		///////////////////////////////////////////////////////////////////////////////////

	}
}

//--------------------------------------------------------------
void testApp::draw(){

	////////////////////////////////Draw the pieces////////////////////////////////////
	if(drawCuts==true){
		myPuzzle->draw();
		drawCuts = false;
	}

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
	if(key == 'c') {
		cout << "init cut: " << ofGetElapsedTimeMillis() << endl;
		makeCut = true;
	}
	if(key == 'd') {
		cout << "manualDRAW" << endl;
		drawCuts = true;
	}
	if(key == 'g') {
		cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
	}
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