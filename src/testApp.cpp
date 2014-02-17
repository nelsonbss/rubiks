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
#define planeSize 400
#define tamCubie 100

#define displayX 500
#define displayY 400
#define displayZ 100

#define _USE_MATH_DEFINES
//--------------------------------------------------------------
void testApp::setup(){
	initOFRender();
	/////////////////////////////////////////PUZzLE //////////
	puzzleExists = false;
	updatePuzzle = false;
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
	///////////////////////////////////////////////////initialize sgCore library
	sgInitKernel();  
	//initScene();//this function was from openGL initial drawing
	//sgC3DObject::AutoTriangulate(false, SG_DELAUNAY_TRIANGULATION);

	//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////3D OBJECT LOADING//////////////////////////////////////
	////////////////////// create primitive torus
	objectDisplayed = new myobject3D(displayX,displayY);
	objectDisplayed->loadObject(sgCreateTorus(displayZ,80,50,50));

	//objectDisplayed->loadObject(sgCreateCone(200,1,300.0, 3));
	//objectDisplayed->loadObject(sgCreateBox(300,300,300));
	////////////////////// from STL file
	/*const char* nel =  ofToDataPath("cube.stl",false).c_str();
	objectDisplayed.loadObjectFromFile(nel);*/
	objectDisplayed->setup();
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////create cutter///////////////////////////////////////
	myCutter = new cutter(planeThicknes,planeSize,tamCubie,1,0,0,-displayZ); //to make a plane based cutter
	myCutter->setup();
	//////////////////////////////////end create cutter///////////////////////////////////

	//////////////////////////////////create slicer///////////////////////////////////////
	mySlicer = new slicer(myCutter,displayX,displayY);
	mySlicer->setup();
	///////////////////////////end create slicer /////////////////////////////////////////

}
//--------------------------------------------------------------
void testApp::update(){

	pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
		sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
		-cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);

	spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);
	/////////////////////////////////////////////////////////////PUZZLE
	if(makeCut==true){
		////////////////////////////////create puzzle///////////////////////////////////////
		myPuzzle = new puzzle(displayX,displayY,displayZ);
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
		////////////////////////////////end create puzzle////////////////////////////////////
		drawCuts1 = true;
		updatePuzzle = true;
		makeCut = false;
		///////////////////////////////////////////////////////////////////////////////////
	}
	///move all puzzle
	if(movePRight){
		//myPuzzle->unDraw();
		myPuzzle->moveRight();
		drawCuts = true;
	}else if(movePLeft){
		//myPuzzle->unDraw();
		myPuzzle->moveLeft();
		drawCuts = true;
	}else if(movePUp){
		//myPuzzle->unDraw();
		myPuzzle->moveUp();
		drawCuts = true;
	}else if(movePDown){
		//myPuzzle->unDraw();
		myPuzzle->moveDown();
		drawCuts = true;
	}
	////////////////////////////////////////////rotate all puzzle
	if(rotatePHright == true) {//rotate right
		//myPuzzle->unDraw();
		myPuzzle->rotateHright();
		drawCuts = true;
	}
	if(rotatePHleft == true) {//rotate left
		//myPuzzle->unDraw();
		myPuzzle->rotateHleft();
		drawCuts = true;
	}
	if(rotatePVup == true) {//rotate up
		//myPuzzle->unDraw();
		myPuzzle->rotateVup();
		drawCuts = true;
	}
	if(rotatePVdown == true) {//rotate down
		//myPuzzle->unDraw();
		myPuzzle->rotateVdown();
		drawCuts = true;
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////face rotations
	SG_POINT point = {0,0,0};
	SG_VECTOR axis = {1,0,0};
	int idcubie = 1;

	if(faceRotateC == true) {//c
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axis,true,tempDeg);
		//drawCuts = true;
		faceRotateC = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCC == true) {//
		
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axis,false,tempDeg);
		//drawCuts = true;
		faceRotateCC = false;
		//drawCuts = true;
	}

	SG_VECTOR axisy = {0,1,0};
	if(faceRotateCy == true) {//c
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axisy,true,tempDeg);
		//drawCuts = true;
		faceRotateCy = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCy == true) {//
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axisy,false,tempDeg);
		//drawCuts = true;
		faceRotateCCy = false;
		//cout << tempDeg << endl;
	}

	SG_VECTOR axisz = {0,0,1};
	if(faceRotateCz == true) {//c
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point, axis, tempDeg,true);
		myPuzzle->rotateByIDandAxis(idcubie,axisz,true,tempDeg);
		//drawCuts = true;
		faceRotateCz = false;
		//cout << tempDeg << endl;
	}
	if(faceRotateCCz == true) {//
		//myPuzzle->unDraw();
		//myPuzzle->faceRotate(point,axis,tempDeg,false);
		myPuzzle->rotateByIDandAxis(idcubie,axisz,false,tempDeg);
		//drawCuts = true;
		faceRotateCCz = false;
		//cout << tempDeg << endl;
	}


	///////////////////////////////////////////////////////////////////////update objects OF
	if(draw3dObject){
		objectDisplayed->update(); //rotates the selected object...just for show
	}
	///////////////////////////////////////update cubies
	if(updatePuzzle){
		if(puzzleExists){
			myPuzzle->update();
			//updatePuzzle = false;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	// enable lighting //
    ofEnableLighting();
    // enable the material, so that it applies to all 3D objects before material.end() call //
	material.begin();
    // activate the lights //
	if (bPointLight) pointLight.enable();
	if (bSpotLight) spotLight.enable();
	if (bDirLight) directionalLight.enable();
    
    // grab the texture reference and bind it //
    // this will apply the texture to all drawing (vertex) calls before unbind() //
    if(bUseTexture) ofLogoImage.getTextureReference().bind();
    
	ofSetColor(255, 255, 255, 255);
    /*ofPushMatrix();
    ofTranslate(center.x, center.y, center.z-300);
    ofRotate(ofGetElapsedTimef() * .8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawSphere( 0,0,0, radius);
    ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(300, 300, cos(ofGetElapsedTimef()*1.4) * 300.f);
	ofRotate(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
	ofRotate(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox(0, 0, 0, 60);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(center.x, center.y, -900);
	ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox( 0, 0, 0, 850);
	ofPopMatrix();*/
    
    if(bUseTexture) ofLogoImage.getTextureReference().unbind();
	
	if (!bPointLight) pointLight.disable();
	if (!bSpotLight) spotLight.disable();
	if (!bDirLight) directionalLight.disable();
	
   
	////////////////////////////////////////////////////////////////////////////PUZZLE !!!!//////////////////////////////////////
	////////////////////////////////Draw the pieces////////////////////////////////////
	if(drawCuts1==true){
		//made the cuts
		//mySlicer->draw();
		drawCuts1 = false;
		draw3dObject = false;
		drawCuts = true;///////////////////////////////////////////turn this ON!!! to keep working on ofMesh render of puzzle, like on the 3dObject
	}

	if(draw3dObject){
		objectDisplayed->draw();
		//myCutter->draw();
	}else{
		//objectDisplayed->unDraw();//now its not doing anything
		//myCutter->unDraw();
	}

	if(drawCuts==true){
		if(puzzleExists == true){
			myPuzzle->draw();
		}
		//drawCuts = false;
	}

	//small test of openFrameworks simple drawing embeded with sgCore geometry 
	ofPushMatrix();
		ofTranslate(300,300);
		ofSetColor(ofColor(255,0,255));
		ofCircle(ofPoint(0,0),5);
		//ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofPopMatrix();

	///use openGL do draw elements taht are on the sgCore Scene object
	//drawElements();

	//////////////////////////////////////////////////////////////////////////END OF RENDERING////////////////////
	 material.end();
	// turn off lighting //
    ofDisableLighting();
    
	ofSetColor( pointLight.getDiffuseColor() );
	if(bPointLight) pointLight.draw();
    
    ofSetColor(255, 255, 255);
	ofSetColor( spotLight.getDiffuseColor() );
	if(bSpotLight) spotLight.draw();
	
	ofSetColor(255, 255, 255);
	/*ofDrawBitmapString("Point Light On (1) : "+ofToString(bPointLight) +"\n"+
					   "Spot Light On (2) : "+ofToString(bSpotLight) +"\n"+
					   "Directional Light On (3) : "+ofToString(bDirLight)+"\n"+
					   "Shiny Objects On (s) : "+ofToString(bShiny)+"\n"+
                       "Spot Light Cutoff (up/down) : "+ofToString(spotLight.getSpotlightCutOff(),0)+"\n"+
                       "Spot Light Concentration (right/left) : " + ofToString(spotLight.getSpotConcentration(),0)+"\n"+
                       "Smooth Lighting enabled (x) : "+ofToString(bSmoothLighting,0)+"\n"+
                       "Textured (t) : "+ofToString(bUseTexture,0),
					   20, 20);*/
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
			//drawCuts =false;
			//myPuzzle->unDraw();
			//myPuzzle->unDo();
		}

		/////////////////////FACE ROTATIONS!!!//////////////////////////////
		//rotation point: 3D-center of cubie 7 (8 in the original numbering)

		///x axis
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
		//y axis
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
		//z axis
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
//-----------------------------------------------------------------------------
void testApp::initOFRender(){
	ofGLRenderer(true);

	ofSetVerticalSync(true);
	ofEnableDepthTest();
	ofBackground(100, 100, 100, 0);
	// turn on smooth lighting //
	bSmoothLighting = true;
	ofSetSmoothLighting(true);

	// lets make a high-res sphere //
	// default is 20 //
	ofSetSphereResolution(128);

	// radius of the sphere //
	radius = 180.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);

	// Point lights emit light in all directions //
	// set the diffuse color, color reflected from the light source //
	pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

	// specular color, the highlight/shininess color //
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
	pointLight.setPointLight();

	spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
	spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));

	// turn the light into spotLight, emit a cone of light //
	spotLight.setSpotlight();

	// size of the cone of emitted light, angle between light axis and side of cone //
	// angle range between 0 - 90 in degrees //
	spotLight.setSpotlightCutOff( 50 );

	// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	// range 0 - 128, zero is even illumination, 128 is max falloff //
	spotLight.setSpotConcentration( 45 );

	// Directional Lights emit light based on their orientation, regardless of their position //
	directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
	directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	directionalLight.setDirectional();

	// set the direction of the light
	// set it pointing from left to right -> //
	directionalLight.setOrientation( ofVec3f(0, 90, 0) );


	bShiny = true;
	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	bPointLight = bSpotLight = bDirLight = true;

	// tex coords for 3D objects in OF are from 0 -> 1, not 0 -> image.width
	// so we must disable the arb rectangle call to allow 0 -> 1
	ofDisableArbTex();
	//// load an image to use as the texture //
	ofLogoImage.loadImage("of.png");
	bUseTexture = true;
}





/////////////////////////////////////////////////////////////////////////////////////
// openGL rendering
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
//-----------------------------------------------------------------------------------
void testApp::drawElements(){
	//////////////////////////////////////////////////////////////
	////draw the elements of the scene
	sgCObject*  curObj = sgGetScene()->GetObjectsList()->GetHead();

	while (curObj)
	{
		if (true)
		{
			Painter::DrawObject(GL_RENDER,curObj,false);

			/*if ((curObj->GetAttribute(SG_OA_DRAW_STATE) & SG_DS_GABARITE))
			{
			SG_POINT a1,a2;
			curObj->GetGabarits(a1,a2);
			Painter::DrawGabariteBox(a1,a2);
			}*/
		}
		curObj = sgGetScene()->GetObjectsList()->GetNext(curObj);
	}
}