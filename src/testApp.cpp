#include "testApp.h"
#include <vector>
#include "game.h"
///////////////////////////////////////////
#include "Painter.h"
#include "sgCore.h"

///////////////////////////////////////////
#include <math.h>
#define _USE_MATH_DEFINES

#define displayX 500
#define displayY 400
#define displayZ 100
//--------------------------------------------------------------
void testApp::setup(){
	/////////////////////////////initialize sgCore library
	sgInitKernel();  
	//initScene();//this function was from openGL initial drawing

	/////////////////////////////initialize openFrameworks rendering
	initOFRender();

	////////////////////////////////create games
	//one game for now
	SG_VECTOR p = {0,0,0};
	game *tempGame = new game(p, 1024, 768);
	myGames.push_back(tempGame);

	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->setup();
	}
}
//--------------------------------------------------------------
void testApp::update(){
	//////////////////////////////open frameworks lights /////////////////////////////////
	pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
		sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
		-cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);

	spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);

	/////////////////////////////////////////////////////update games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->update();
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

	//small test of openFrameworks drawing embeded with sgCore geometry 
	ofPushMatrix();
	ofTranslate(300,300);
	ofSetColor(ofColor(255,0,255));
	ofCircle(ofPoint(0,0),5);
	//ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofPopMatrix();

	if(bUseTexture) ofLogoImage.getTextureReference().unbind();

	if (!bPointLight) pointLight.disable();
	if (!bSpotLight) spotLight.disable();
	if (!bDirLight) directionalLight.disable();


	////////////////////////////////////////////////////////////////////////////PUZZLE !!!!//////////////////////////////////////
	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->draw();
	}


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

	////////////////////////////////////////////////////////////////
	///////////////////load objects for a game//step 1 of the experience
	//right now we only have one game, ideal: the master gui will tell a game wich object to load on drag and drop gestures
	SG_VECTOR objectPos = {0,0,0}; 
	SG_VECTOR tempPos = {displayX,displayY,displayZ};
	if(key == '1') {
		myGames[0]->loadObject(1,objectPos,tempPos); //pos.z its the torus radious
	}
	if(key == '2') {
		SG_VECTOR v = {500,400,100}; 
		myGames[0]->loadObject(2,objectPos,tempPos);
	}
	if(key == '3') {
		SG_VECTOR v = {500,400,100}; 
		myGames[0]->loadObject(3,objectPos,tempPos);
	}
	////////////////////////////////////////////////////////////////
	//////////make a cut//NEXT button action on step 2 of experience .. now its step 1..because we dont have armature yet
	if(key == 'c') {
		//tell a game to make a cut
		SG_VECTOR v = {500,400,100};
		myGames[0]->createPuzzle(v);
	}
	/////////////////////////////////////////////////////////////////
	//////////resart// RESART button action on any step of the experience
	if(key == 'r'){
		//tell a game to restart 
		myGames[0]->restart();
	}
	//////////////////////////////move all puzzle
	if(key == 'l') {
		SG_VECTOR p = {10,0,0};
		myGames[0]->move(p);
	}
	if(key == 'j') {
		SG_VECTOR p = {-10,0,0};
		myGames[0]->move(p);
	}
	if(key == 'i') {
		SG_VECTOR p = {0,-10,0};
		myGames[0]->move(p);
	}
	if(key == 'k') {
		SG_VECTOR p = {0,10,0};
		myGames[0]->move(p);
	}
	/////////////////////////////rotate all puzzle
	if(key == 'm') {//rotate right
		SG_VECTOR r = {0,0.1,0};
		myGames[0]->rotate(r);
	}
	if(key == 'n') {//rotate left
		SG_VECTOR r = {0,-0.1,0};
		myGames[0]->rotate(r);
	}
	if(key == 'y') {//rotate up
		SG_VECTOR r = {-0.1,0,0};
		myGames[0]->rotate(r);
	}
	if(key == 'h') {//rotate down
		SG_VECTOR r = {0.1,0,0};
		myGames[0]->rotate(r);
	}

	//if(puzzleExists == true){
	//	if(key == 'd') {
	//		cout << "manualDRAW" << endl;
	//		drawCuts = true;
	//	}
	//	if(key == 'f') {
	//		cout << "nu cubies " << myPuzzle->giveNumCubies() << endl;
	//	}
	//	if(key == 'g') {
	//		cout << "nu pieces " << mySlicer->countPieces() << endl;
	//	}
	//	/////////////////////FACE ROTATIONS!!!//////////////////////////////
	//	//rotation point: 3D-center of cubie 7 (8 in the original numbering)
	//	///x axis
	//	if(key == 'q') {
	//		ct1 = ofGetElapsedTimeMillis();
	//		//tempDeg = 0.0;
	//		randcubie = rand()%26;
	//		faceRotateC = true; //clockwise
	//	}if(key == 'a') {
	//		//tempDeg = 0.0;
	//		ct1 = ofGetElapsedTimeMillis();
	//		faceRotateCC = true; //counter clockwise
	//	}
	//	//y axis
	//	if(key == 'w') {
	//		ct1 = ofGetElapsedTimeMillis();
	//		//tempDeg = 0.0;
	//		randcubie = rand()%26;
	//		faceRotateCy = true; //clockwise
	//	}if(key == 's') {
	//		//tempDeg = 0.0;
	//		ct1 = ofGetElapsedTimeMillis();
	//		faceRotateCCy = true; //counter clockwise
	//	}
	//	//z axis
	//	if(key == 'e') {
	//		ct1 = ofGetElapsedTimeMillis();
	//		//tempDeg = 0.0;
	//		randcubie = rand()%26;
	//		faceRotateCz = true; //clockwise
	//	}if(key == 'd') {
	//		//tempDeg = 0.0;
	//		ct1 = ofGetElapsedTimeMillis();
	//		faceRotateCCz = true; //counter clockwise
	//	}
	//}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	/////////////////////////////move all puzzle
	if(key == 'l') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->move(p);
	}
	if(key == 'j') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->move(p);
	}
	if(key == 'i') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->move(p);
	}
	if(key == 'k') {
		SG_VECTOR p = {0,0,0};
		myGames[0]->move(p);
	}
	/////////////////////////////rotate all puzzle
	if(key == 'm') {//rotate 
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotate(r);
	}
	if(key == 'n') {//rotate left
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotate(r);
	}
	if(key == 'y') {//rotate up
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotate(r);
	}
	if(key == 'h') {//rotate down
		SG_VECTOR r = {0.0,0,0};
		myGames[0]->rotate(r);
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
	/*if(puzzleExists == true){
	myPuzzle->exit();
	objectDisplayed->exit();
	myCutter->exit();
	mySlicer->exit();
	}else{
	objectDisplayed->exit();
	myCutter->exit();
	mySlicer->exit();
	}*/
	myGames[0]->restart();
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