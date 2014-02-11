#include "testApp.h"
#include "myobject3D.h"
#include "slicer.h"
#include "cutter.h"
#include "puzzle.h"

///////////////////////////////////////////
#include "Painter.h"
#include "sgCore.h"

#include <math.h>
///////////////////////////////////////////
#define planeThicknes 0.001
#define planeSize 300
#define tamCubie 100

#define displayX 500
#define displayY 400

#define _USE_MATH_DEFINES
//--------------------------------------------------------------
void testApp::setup(){
	//ofSetVerticalSync(true);
	//ofEnableDepthTest();
	//ofBackground(10, 10, 10, 0);
	// turn on smooth lighting //
   // bSmoothLighting     = true;
    ofSetSmoothLighting(true);
	///////////////////////////////////////////////////////////////
	puzzleExists = false;
	makeCut = false;
	drawCuts = false;
	drawCuts1 = false;
	draw3dObject = true;
	//
	movePRight = false;
	movePLeft = false;
	movePUp = false;
	movePDown = false;
	//
	rotatePHright = false;
	rotatePHleft = false;
	rotatePVup = false;
	rotatePVdown = false;
	//
	tempDeg = 0.0;
	deg = 0.0;
	faceRotateC = false;
	faceRotateCC = false;
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
	//objectDisplayed->loadObject(sgCreateBox(300,300,300));

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
	mySlicer = new slicer(myCutter,displayX,displayY);
	mySlicer->setup();
	/////////////////////////end create slicer /////////////////////////////////////////

}
//--------------------------------------------------------------
void testApp::update(){

	if(makeCut==true){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(displayX,displayY);
		myPuzzle->setup();
		//////////////////////////////end create puzzle////////////////////////////////////

		////BOOLEAN SUBSTRACTION//////////////////////////////////////////////////////////
		//mySlicer->xSlicing(*mySlicer->myCutter,objectDisplayed->getObject(),1,1);
		///boolean INTERSECTION///////////////////////////////////////////////////////////
		mySlicer->intersectCubes(objectDisplayed->getObject()); 
		//now slicer has all the parts inside sgCGroup ** = pieces[]

		//////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle->loadPieces(mySlicer->getPieces());
		puzzleExists = true;
		//////////////////////////////end create puzzle////////////////////////////////////
		makeCut = false;
		drawCuts1 = true;
		//cout << "end cut:" << ofGetElapsedTimeMillis() << endl;
		///////////////////////////////////////////////////////////////////////////////////
	}
	///move all puzzle
	if(movePRight){
		myPuzzle->unDraw();
		myPuzzle->moveRight();
		drawCuts = true;
	}else if(movePLeft){
		myPuzzle->unDraw();
		myPuzzle->moveLeft();
		drawCuts = true;
	}else if(movePUp){
		myPuzzle->unDraw();
		myPuzzle->moveUp();
		drawCuts = true;
	}else if(movePDown){
		myPuzzle->unDraw();
		myPuzzle->moveDown();
		drawCuts = true;
	}
	////////////////////////////////////////////rotate all puzzle
	if(rotatePHright == true) {//rotate right
		myPuzzle->unDraw();
		myPuzzle->rotateHright();
		drawCuts = true;
	}
	if(rotatePHleft == true) {//rotate left
		myPuzzle->unDraw();
		myPuzzle->rotateHleft();
		drawCuts = true;
	}
	if(rotatePVup == true) {//rotate up
		myPuzzle->unDraw();
		myPuzzle->rotateVup();
		drawCuts = true;
	}
	if(rotatePVdown == true) {//rotate down
		myPuzzle->unDraw();
		myPuzzle->rotateVdown();
		drawCuts = true;
	}
	//////////////////////////////////////////////face rotations
	SG_POINT point = {0,0,0};
	SG_VECTOR axis = {1,0,0};
	float animTime = 300; //mills

	if(faceRotateC == true) {//c

		/*tempDeg += 1.57;
		if(tempDeg >= 6.28){
			tempDeg = 0.0;
		}*/
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(7,axis,true,tempDeg);
		drawCuts = true;
		faceRotateC = false;
		//cout << tempDeg << endl;

		/*myPuzzle->unDraw();
		if(deg < 1.57079633){
		ct2 = ofGetElapsedTimeMillis();
		deg += (ct2 - ct1)*((1.57079633)/animTime);
		myPuzzle->rotateByIDandAxis(7,axis,true,deg);
		ct1 = ct2;
		}else{
		faceRotateC = false;
		//re-arange order of numbers on 3d vector
		cout << deg << endl;
		drawCuts = true;*/
	}
	if(faceRotateCC == true) {//
		//tempDeg -= 1.57;
		//if(tempDeg <= -6.28){
		//	tempDeg = 0.0;
		//}
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(7,axis,false,tempDeg);
		drawCuts = true;
		faceRotateCC = false;
		//cout << tempDeg << endl;

		/*myPuzzle->unDraw();
		if(deg < 1.57079633){
		ct2 = ofGetElapsedTimeMillis();
		deg += ((ct2 - ct1)*(1.57079633))/animTime;
		myPuzzle->rotateByIDandAxis(7,axis,false,deg);
		ct1 = ct2;
		}else{
		faceRotateCC = false;
		}
		cout << deg << endl;
		drawCuts = true;*/
	}

	SG_VECTOR axisy = {0,1,0};
	if(faceRotateCy == true) {//c
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(7,axisy,true,tempDeg);
		drawCuts = true;
		faceRotateCy = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCy == true) {//
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(7,axisy,false,tempDeg);
		drawCuts = true;
		faceRotateCCy = false;
		//cout << tempDeg << endl;
	}

	SG_VECTOR axisz = {0,0,1};
	if(faceRotateCz == true) {//c
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(7,axisz,true,tempDeg);
		drawCuts = true;
		faceRotateCz = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCz == true) {//
		myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(7,axisz,false,tempDeg);
		drawCuts = true;
		faceRotateCCz = false;
		//cout << tempDeg << endl;
	}
	if(draw3dObject){
		objectDisplayed->update(); //rotates the selected object...just for show
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(drawCuts1==true){
		//mySlicer->draw();
		drawCuts1 = false;
		draw3dObject = false;
		drawCuts = true;
	}

	if(draw3dObject){
		objectDisplayed->draw();
		myCutter->draw();
	}else{
		objectDisplayed->unDraw();
		//myCutter->unDraw();
	}

	if(drawCuts==true){
		if(puzzleExists == true){
			myPuzzle->draw();
		}
		drawCuts = false;
	}

	//small test of openFrameworks simple drawing embeded with sgCore geometry 
	ofPushMatrix();
	ofTranslate(300,300);
	ofSetColor(ofColor(255,0,255));
	ofCircle(ofPoint(0,0),5);
	ofPopMatrix();

	///use openGL do draw elements taht are on the sgCore Scene object
	drawElements();

}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'c') {
		//cout << "init cut: " << ofGetElapsedTimeMillis() << endl;
		makeCut = true;
	}
	if(puzzleExists == true){
		if(key == 'd') {
			cout << "manualDRAW" << endl;
			drawCuts = true;
		}
		if(key == 'f') {
			cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
		}
		if(key == 'g') {
			cout << "nu pieces " << mySlicer->countPieces() << endl;
		}
		//////////////////////////////move all puzzle
		if(key == 'l') {
			movePRight = true;
		}
		if(key == 'j') {
			movePLeft= true;
		}
		if(key == 'i') {
			movePUp= true;
		}
		if(key == 'k') {
			movePDown= true;
		}
		/////////////////////////////rotate all puzzle
		if(key == 'm') {//rotate right
			rotatePHright = true;
		}
		if(key == 'n') {//rotate left
			rotatePHleft = true;
		}
		if(key == 'y') {//rotate up
			rotatePVup = true;
		}
		if(key == 'h') {//rotate down
			rotatePVdown = true;
		}
		////////////erase object ///////////
		if(key == 'u') {
			//myPuzzle->unDraw();
			myPuzzle->unDo();
		}
		/////////////////////FACE ROTATIONS!!!///////////////////////////
		////rotate "FRONT" face original cubie #s:
		//16-17-18
		//7-8-9
		//25-26-27
		//rotation point: 3D-center of cubie 8
		if(key == 'q') {
			ct1 = ofGetElapsedTimeMillis();
			//tempDeg = 0.0;
			randcubie = rand()%26;
			faceRotateC = true; //clockwise
		}if(key == 'a') {
			//tempDeg = 0.0;
			ct1 = ofGetElapsedTimeMillis();
			faceRotateCC = true; //counter clockwise
		}

		if(key == 'w') {
			ct1 = ofGetElapsedTimeMillis();
			//tempDeg = 0.0;
			randcubie = rand()%26;
			faceRotateCy = true; //clockwise
		}if(key == 's') {
			//tempDeg = 0.0;
			ct1 = ofGetElapsedTimeMillis();
			faceRotateCCy = true; //counter clockwise
		}
		if(key == 'e') {
			ct1 = ofGetElapsedTimeMillis();
			//tempDeg = 0.0;
			randcubie = rand()%26;
			faceRotateCz = true; //clockwise
		}if(key == 'd') {
			//tempDeg = 0.0;
			ct1 = ofGetElapsedTimeMillis();
			faceRotateCCz = true; //counter clockwise
		}
		if(OF_KEY_DOWN){
			cout << "space" << endl;
		}
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(puzzleExists == true){
		///////////////////////////move all puzzle
		if(key == 'l') {
			movePRight = false;
		}
		if(key == 'j') {
			movePLeft = false;
		}
		if(key == 'i') {
			movePUp = false;
		}
		if(key == 'k') {
			movePDown = false;
		}
		/////////////////////////////rotate all puzzle
		if(key == 'm') {
			rotatePHright = false;
		}
		if(key == 'n') {
			rotatePHleft = false;
		}
		if(key == 'y') {
			rotatePVup = false;
		}
		if(key == 'h') {
			rotatePVdown = false;
		}
		/////////////////////FACE ROTATIONS!!!///////////////////////////
		if(key == 'q') {
			//faceRotateC = false; //clockwise
		}if(key == 'a') {
			//faceRotateCC = false; //counter clockwise
		}
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------
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
	if(puzzleExists == true){
		myPuzzle->exit();
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
	}else{
		objectDisplayed->exit();
		myCutter->exit();
		mySlicer->exit();
	}
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