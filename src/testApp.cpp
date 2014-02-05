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
#define planeSize 300
#define tamCubie 50

#define displayX 500
#define displayY 500
//--------------------------------------------------------------
void testApp::setup(){
	 
	///////////////////////////////////////////////////////////////
	makeCut = false;
	drawCuts = false;
	drawCuts1 = false;
	draw3dObject = true;
	moveRight = false;
	/////initialize sgCore library
	sgInitKernel();  
	initScene();
	sgC3DObject::AutoTriangulate(false, SG_DELAUNAY_TRIANGULATION);

	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING//////////////////////////////////////
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D(displayX,displayY);
	objectDisplayed->loadObject(sgCreateTorus(100,80,34,34));
	//objectDisplayed->loadObject(sgCreateCone(200,1,300.0, 3));
	
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
	objectDisplayed->setup();
	//////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1,0,0); //to make a plane based cutter
	myCutter->setup();
	////////////////////////////////end create cutter///////////////////////////////////

	////////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter);
	mySlicer->setup();
	/////////////////////////end create slicer /////////////////////////////////////////

	////////////////////////////////create puzzle///////////////////////////////////////
	myPuzzle = new puzzle();
	myPuzzle->setup();
	//////////////////////////////end create puzzle////////////////////////////////////

}
//--------------------------------------------------------------
void testApp::update(){
	////////update cutter/////////////////
	//myCutter->update();


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
		mySlicer->intersectCubes(objectDisplayed->getObject()); 
		//now slicer has all the parts inside sgCGroup ** = pieces[]

		//////////////////////////////create puzzle///////////////////////////////////////
		//sgCGroup ** aux = (sgCGroup**) mySlicer->pieces; //WHY CANT I GET THE DATA ** HERE??? its a group**!!
		//sgCGroup ** aux = (sgCGroup**) mySlicer->getPieces(); //WHY CANT I GET THE DATA ** HERE??? on the SETUP?? now on the update I can...!!!
		//sgCGroup * aux2 = (sgCGroup*) mySlicer->pieces[6]; //AND HERE IT IS!!! ->> why???
		myPuzzle->loadPieces(mySlicer->getPieces());
		//////////////////////////////end create puzzle////////////////////////////////////
		makeCut = false;

		drawCuts1 = true;
		//cout << "end cut:" << ofGetElapsedTimeMillis() << endl;
		///////////////////////////////////////////////////////////////////////////////////
	}

	if(moveRight){
		myPuzzle->unDraw();
		myPuzzle->update();
		
		//drawElements();
		drawCuts = true;
	}

	objectDisplayed->update(); //rotates the onject.. jsut for show
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50, 50, 50, 0);
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(drawCuts1==true){
		//mySlicer->draw();
		drawCuts1 = false;
		draw3dObject = false;
	}

	if(draw3dObject){
		//objectDisplayed->draw();
		//myCutter->draw();
	}else{
		objectDisplayed->unDraw();
		//myCutter->unDraw();
	}

	if(drawCuts==true){
		myPuzzle->draw();
		drawCuts = false;
	}

	

	
	//SG_VECTOR rotD = {1,0,0};
	//objectDisplayed->getObject()->InitTempMatrix()->Translate(rotD);
	//objectDisplayed->getObject()->ApplyTempMatrix();  
	////objectDisplayed->getObject()->DestroyTempMatrix();
	//sgGetScene()->AttachObject(objectDisplayed->getObject());

	ofPushMatrix();
		ofTranslate(300,300);
		ofCircle(ofPoint(0,0),30);
	ofPopMatrix();

	
	drawElements();
	
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'c') {
		//cout << "init cut: " << ofGetElapsedTimeMillis() << endl;
		makeCut = true;
	}
	if(key == 'd') {
		cout << "manualDRAW" << endl;
		drawCuts = true;
	}
	if(key == 'g') {
		cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
	}
	if(key == 'h') {
		cout << "nu pieces " << mySlicer->countPieces() << endl;
	}
	if(key == 'r') {
	  myPuzzle->rotate();
	}
	if(key == 'l') {
	  moveRight = true;
	}

	if(key == 'u') {
	  myPuzzle->unDraw();
	}
	


}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 'l') {
	  moveRight = false;
	}
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

	if (group->BreakGroup(allChilds)){  
		//assert(0);  
		for (int i=0;i<ChildsCount;i++){  
			sgGetScene()->AttachObject(allChilds[i]);  
		}  
		free(allChilds); 
	}  
	//const int sz = group->GetChildrenList()->GetCount();  
	//assert(sz==0);  
	//sgDeleteObject(group);  

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void testApp::exit(){
	myCutter->exit();
	mySlicer->exit();
	//sgFreeKernel();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//
// InitScene()
// Called when the OpenGL RC has been created. Sets initial state for the OpenGL scene.
//
void testApp::initScene(){
  // Lights properties
  float ambientProperties[]  = {0.1f, 0.1f, 0.1f, 1.0f};
  float diffuseProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  float specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
  glLightfv( GL_LIGHT0, GL_DIFFUSE,diffuseProperties);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

  glClearColor(1.0,1.0,1.0,1.0f);

  glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);

  //// Texture
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


  // Default : lighting

  //glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);


  // Default : blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void testApp::drawElements(){
//////////////////////////////////////////////////////////////
	////draw the elements of the scene
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


	//int tt = sgGetScene()->GetObjectsList()->GetCount();
	//if(tt != 0){
	//	sgCObject*  curObj = sgGetScene()->GetObjectsList()->GetHead();
	//	for(int i=0;i<tt;i++){
	//		if(curObj){
	//			//sgGetScene()->Clear();
	//			Painter::DrawObject(GL_RENDER,curObj,false);
	//			curObj = sgGetScene()->GetObjectsList()->GetNext(curObj);
	//		}
	//	}
	//}
}