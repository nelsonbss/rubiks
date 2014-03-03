#include "testApp.h"
#include <vector>
#include "game.h"
///////////////////////////////////////////
#include "sgCore.h"

#define displayX 500
#define displayY 400
#define displayZ 0

//--------------------------------------------------------------
void testApp::setup(){
	/////////////////////////////initialize sgCore library
	sgInitKernel();

	/////////////////////////////initialize openFrameworks rendering
	initOFRender();

	////////////////////////////////create games
	////////////////////////////////one game for now
	SG_VECTOR gamePos = {0,0,0}; //one game created at the origin // this will have to change for a game creating function when more stations are anabled
	SG_VECTOR displayPos = {ofGetWidth() / 2, ofGetHeight() / 2,displayZ};
	game *tempGame = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos);
	myGames.push_back(tempGame);
	currentGame = 1;
	//create a second game
	game *tempGame2 = new game(gamePos, ofGetWidth(), ofGetHeight(),displayPos);
	myGames.push_back(tempGame2);
	///////////////////////////////setup games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->setup();
	}

	rotate = true;
}
//--------------------------------------------------------------
void testApp::update(){
	//////////////////////////////open frameworks lights 
	updateOFLights();

	///////////////////////////////////////////update games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->update();
	}
}
//--------------------------------------------------------------
void testApp::draw(){
	///////////////////START OF RENDERING////////////////////
	startOFLights();


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  DUMMY GUI ///  DUMMY GUI  ///  DUMMY GUI
	///to keep track of what menus and buttons to show, and when

	for(int i = 0; i < myGames.size(); i++){
		int gStep=0;
		//get current step of game
		gStep = myGames[i]->getCurrentStep();
		if(gStep == 0){
			//waiting for object to be selected from menu //drag behavior
			//show object menu
			ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
		}else if(gStep == 1){
			//an object has been selected
			ofDrawBitmapString("SELECT AN OBJECT:" + ofToString("") +"\n" + "torus "+ofToString(1)+"\n"+ "box "+ofToString(2)+"\n"+ "cone "+ofToString(3)+"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep ==2){
			//waiting for armature to be selected from menu //drag behavior
			//show armature menu
			ofDrawBitmapString("SELECT AN ARMATURE:" + ofToString("") +"\n" + "armature 1:  "+ofToString(1)+"\n",20, 20);
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 3){
			//armature was selected // show armature...manupulate armature
			ofDrawBitmapString("ARMATURE SELECTED:" + ofToString("") +"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 60); //this makes the puzzle
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 4){
			//puzzle has been created
			//show color palette menu
			ofDrawBitmapString("SELECT a COLOR:" + ofToString("") +"\n" + "color 1:  "+ofToString(1)+"\n",20, 20);
			//show next button.
			ofDrawBitmapString("NEXT: press 'n' " + ofToString("") +"\n" ,20, 60); //this enables the puzzle to have face rotations
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 80);
		}else if(gStep == 5){
			//puzzle has been created
			//rotations can happen
			ofDrawBitmapString("ROTATE PUZZLE:  horizontal: 'm' 'n'              vertial: 'h' 'y'"+ofToString("")+"\n",20, 20);
			ofDrawBitmapString("X rotations: 'q' 'a'"+ofToString(" ")+"\n",20, 40);
			ofDrawBitmapString("Y rotations: 'w' 's'"+ofToString(" ")+"\n",20, 60);
			ofDrawBitmapString("Z rotations: 'e' 'd'"+ofToString(" ")+"\n",20, 80);
			//undo move
			ofDrawBitmapString("UNDO: press 'u' " + ofToString("") +"\n" ,20, 120);
			//show restart button.
			ofDrawBitmapString("RESTART: press 'r' " + ofToString("") +"\n" ,20, 140);
		}
	}


	////////////////////////PUZZLE //////////////////////
	///////////////////////////////draw games
	for(int i = 0; i < myGames.size(); i++){
		myGames[i]->draw();
	}

	///////////////////END OF RENDERING////////////////////
	stopOFLights();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(currentGame == 1){
		myGames[0]->guiInput(key);
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	///////////////////////////////move all puzzle
	//if(key == 'l') {
	//	SG_VECTOR p = {0,0,0};
	//	myGames[0]->moveP(p);
	//}
	//if(key == 'j') {
	//	SG_VECTOR p = {0,0,0};
	//	myGames[0]->moveP(p);
	//}
	//if(key == 'i') {
	//	SG_VECTOR p = {0,0,0};
	//	myGames[0]->moveP(p);
	//}
	//if(key == 'k') {
	//	SG_VECTOR p = {0,0,0};
	//	myGames[0]->moveP(p);
	//}
	///////////////////////////////rotate all puzzle
	//if(key == 'm') {//rotate 
	//	SG_VECTOR r = {0.0,0,0};
	//	myGames[0]->rotateP(r);
	//}
	//if(key == 'n') {//rotate left
	//	SG_VECTOR r = {0.0,0,0};
	//	myGames[0]->rotateP(r);
	//}
	//if(key == 'y') {//rotate up
	//	SG_VECTOR r = {0.0,0,0};
	//	myGames[0]->rotateP(r);
	//}
	//if(key == 'h') {//rotate down
	//	SG_VECTOR r = {0.0,0,0};
	//	myGames[0]->rotateP(r);
	//}
	///////////////////////face rotations//no rotations comming in from gui
	////send: SG_VECTOR axis = {0,0,0};
	//if(key == 'q') {
	//	if(rotate == false) {//c
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,true);
	//		rotate = true;
	//	}
	//}
	//if(key == 'a') {
	//	if(rotate == false) {//cc
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,false);
	//		rotate = true;
	//	}
	//}
	////y axis
	//if(key == 'w') {
	//	if(rotate == false) {
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,true);
	//		rotate = true;
	//	}
	//}if(key == 's') {
	//	//counter clockwise
	//	if(rotate == false) {
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,false);
	//		rotate = true;
	//	}
	//}
	////z axis
	//if(key == 'e') {
	//	if(rotate == false) {
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,true);
	//		rotate = true;
	//	}
	//}if(key == 'd') {
	//	if(rotate == false) {
	//		SG_VECTOR axis = {0,0,0};
	//		myGames[0]->rotateByIDandAxis(0,axis,false);
	//		rotate = true;
	//	}
	//}
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
	myGames[0]->restart();
	myGames[0]->exit();

	myGames[1]->restart();
	myGames[1]->exit();
	sgFreeKernel();
}
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////
// OF rendering
void testApp::initOFRender(){
	ofGLRenderer(true);

	ofSetVerticalSync(true);
	ofEnableDepthTest();
	/////background
	//ofBackground(100, 100, 100, 0);//gray
	//ofBackground(30, 144, 255, 0);//blue
	ofBackground(65, 105, 225, 0);//blue
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
	directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 0.f));
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
	//ofLogoImage.loadImage("of.png");
	//bUseTexture = true;
}
//------------------------------------------------------------------------------
void testApp::startOFLights(){
	// enable lighting //
	ofEnableLighting();
	// enable the material, so that it applies to all 3D objects before material.end() call //
	material.begin();
	// activate the lights //
	if (bPointLight) pointLight.enable();
	//if (bSpotLight) spotLight.enable();
	if (bDirLight) directionalLight.enable();

	// grab the texture reference and bind it //
	// this will apply the texture to all drawing (vertex) calls before unbind() //
	if(bUseTexture) ofLogoImage.getTextureReference().bind();

	//ofSetColor(255, 255, 255, 255);
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
	//ofPushMatrix();
	//ofTranslate(300,300);
	//ofSetColor(ofColor(255,0,255));
	//ofCircle(ofPoint(0,0),5);
	////ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	//ofPopMatrix();

	if(bUseTexture) ofLogoImage.getTextureReference().unbind();

	if (!bPointLight) pointLight.disable();
	//if (!bSpotLight) spotLight.disable();
	if (!bDirLight) directionalLight.disable();
}
//------------------------------------------------------------------------------
void testApp::updateOFLights(){
	/*pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
	sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
	-cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);*/

	/*spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);*/
}
//------------------------------------------------------------------------------
void testApp::stopOFLights(){
	material.end();
	// turn off lighting //
	ofDisableLighting();

	ofSetColor( pointLight.getDiffuseColor() );
	//if(bPointLight) pointLight.draw();

	//ofSetColor(255, 255, 255);
	//ofSetColor( spotLight.getDiffuseColor() );
	//if(bSpotLight) spotLight.draw();

	//ofSetColor(255, 255, 255);
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
