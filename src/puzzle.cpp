#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"
#include "ofRender.h"

#include <vector>
#include <iostream>

#define HEIGHT 3
#define WIDTH 3
#define DEPTH 3

puzzle::puzzle(SG_VECTOR p, ofVec3f offset){
	numPieces = 27;
	myCubies = (cubie**)malloc(numPieces*sizeof(cubie*));

	cubiesOffset = offset;

	pos.x = p.x;
	pos.y = p.y;
	pos.z = p.z;

	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	const size_t NElements1(3);
	const size_t NElements2(3);
	const size_t NElements3(3);
	const int InitialValueForAllEntries(0);

	ThreeDimensions three_dim(NElements3, TwoDimensions(NElements2, OneDimension(NElements1, InitialValueForAllEntries)));

	three_dim1 = three_dim;

	three_dim1[0][0][2] = 15;	three_dim1[1][0][2] = 16;	three_dim1[2][0][2] = 17;
	three_dim1[0][1][2] = 6;		three_dim1[1][1][2] = 7;		three_dim1[2][1][2] = 8;
	three_dim1[0][2][2] = 24;	three_dim1[1][2][2] = 25;	three_dim1[2][2][2] = 26;

	three_dim1[0][0][1] = 14;	three_dim1[1][0][1] = 9;		three_dim1[2][0][1] = 10;
	three_dim1[0][1][1] = 5;		three_dim1[1][1][1] = 0;		three_dim1[2][1][1] = 1;
	three_dim1[0][2][1] = 23;	three_dim1[1][2][1] = 18;	three_dim1[2][2][1] = 19;

	three_dim1[0][0][0] = 13;	three_dim1[1][0][0] = 12;	three_dim1[2][0][0] = 11;
	three_dim1[0][1][0] = 4;		three_dim1[1][1][0] = 3;		three_dim1[2][1][0] = 2;
	three_dim1[0][2][0] = 22;	three_dim1[1][2][0] = 21;	three_dim1[2][2][0] = 20;

	/* now read the value: */
	//std::cout << "It should be 13: " << three_dim1[0][0][0] << "\n";
	//std::cout << "It should be 4: " << three_dim1[0][1][0] << "\n";

	/* get X slize 1*/
	//TwoDimensions& two_dim(three_dim[1]);
	/* read it: */
	//std::cout << "It should be 0: " << two_dim[1][1] << "\n";

	/* get Y=2 strip fom  of that X slice 1 */
	//OneDimension& one_dim(two_dim[2]);

	/* read it (this is two_dim[2][1], aka three_dim[1][2][1]): */
	//std::cout << "It should be 18: " << one_dim[1] << "\n";
	/* or */
	//std::cout << "It should be 21: " << one_dim.at(2) << "\n";
	SubObMediator::Instance()->addObserver("ibox-bl:1", this);
	SubObMediator::Instance()->addObserver("ibox-bl:0", this);
	bDrawLine = false;
	bHaveActiveCubie = false;
	bHaveRotationCubie = false;
	faceRotateB = false;
	activeCubie = -1;
	bUnproject = false;
	bHaveAxis = false;
}
//----------------------------------------------------------------
void puzzle::setup(){
	/*
	init();
	addParam("drag", "true");
	addParam("n", "1");
	scale = 1.0;
	setName("puzzle");
	*/
	//activate();
}
//----------------------------------------------------------------
void puzzle::update(){
	//iterate through cubies
	//cout << "UPDATING PUZZLE" << endl;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->update();
			if(myCubies[i]->getRotate()){
				cout << "rotating cubies." << endl;
				int id = myCubies[i]->getId() - 1;
				int rotationDirection = myCubies[i]->getRotationDirection();
				myCubies[i]->setRotate(false);
				SG_VECTOR axis;
				if(rotationDirection == ROTATE_UP){
					axis.x = 0;
					axis.y = 1;
					axis.z = 0;
					rotateByIDandAxis(id, axis, true);
				} else if(rotationDirection == ROTATE_DOWN){
					axis.x = 0;
					axis.y = 1;
					axis.z = 0;
					rotateByIDandAxis(id, axis, false);
				} else if(rotationDirection == ROTATE_LEFT){
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;
					rotateByIDandAxis(id, axis, true);
				} else {
					axis.x = 1;
					axis.y = 0;
					axis.z = 0;
					rotateByIDandAxis(id, axis, false);
				}
			}
		}
	}
}
//----------------------------------------------------------------
void puzzle::draw(){  

	/*
	if(bUnproject){
	unprojectPoint();
	bUnproject = false;
	}
	*/

	ofPushMatrix();
	//ofTranslate(pos.x,pos.y,pos.z);
	//puzzle tells every cubie to attach objects to scene
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->draw();
		}
	}
	ofPopMatrix();
	if(bDrawLine && bHaveLine){
		ofSetColor(255,0,0);
		ofLine(lineStart.x, lineStart.y, lineStart.z, lineStop.x, lineStop.y, lineStop.z);
	}
}
//----------------------------------------------------------------
int puzzle::giveNumCubies(){
	//tell how many cubies we have with objects inside
	int aux=0;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i]->objects!= NULL){
			aux++;
		}
	}
	return aux;
}
//----------------------------------------------------------------
void puzzle::loadPieces(sgCGroup **pcs,int selObjId, ofVec3f v){
	//it loads the pieces that the slicer made, the pieces are in a sgCGroup** pieces[], 
	//this function receives a copy of that sgCGroup** made by mySlicer->getPieces()
	//it loads them into its own cubies
	//create cubies
	//so each time there is a new boolean operation, whole new cubies get created with variables in zero or blank
	for(int i=0;i<numPieces;i++){
		cubie *auxCubie = new cubie(pos.x,pos.y,pos.z,i,selObjId,cubiesOffset);
		//auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}

	for(int i=0;i<numPieces;i++){
		//get group from pieces[] copy: pcs[]
		sgCGroup *part = pcs[i];

		if(part != NULL){
			const int ChCnt = part->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			part->BreakGroup(allParts);

			sgCObject **obj = (sgCObject**)malloc(50*sizeof(sgCObject*));
			int realNumPieces=0;

			for (int j=0; j < ChCnt; j++){
				//clone each part
				obj[j] =allParts[j];
				realNumPieces ++;
			}
			//make them a group
			cubieGroup = sgCGroup::CreateGroup(obj,realNumPieces);
			myCubies[i]->setObjects(cubieGroup,i,v);//here goes the group of clones from the iriginal slicing pieces[]
			myCubies[i]->setup();
			//i is the cubie ID
			//put that cubie on the cubies[]

			//cleanup
			free(obj);
			if(allParts != NULL){
				for (int j=0; j < ChCnt; j++){
					if(allParts[j] != NULL){
						sgCObject::DeleteObject(allParts[j]);
					}
				}
				free(allParts);
			}
			sgDeleteObject(part);
		}else{
			myCubies[i]->setObjects(NULL,i,v);
			myCubies[i]->setup();
		}
	}

	//create the meshes from the sgCore objects
	//so the objects can be renderes by openFrameworks
	for(int i=0;i<numPieces;i++){
		myCubies[i]->crateOfMeshs();
	}
}
//----------------------------------------------------------------
void puzzle::rotate(SG_VECTOR r){  
	//puzzle tells every cubie to rotate
	/*rot.x +=  r.x;
	rot.y +=  r.y;
	rot.z +=  r.z;*/
	//for(int i=0;i<numPieces;i++){
	//	myCubies[i]->rotate(rot);
	//}
}
//----------------------------------------------------------------
void puzzle::rotateTB(float anglei, ofVec3f axisi){
	//this function is being called from the trackBall object, which has the pointer to the puzzle object
	//gets info from trackball object
	qaxis = axisi; 
	qangle = anglei;
}
//----------------------------------------------------------------
void puzzle::move(SG_VECTOR p){
	//iterate through cubies
	pos.x +=  p.x;
	pos.y +=  p.y;
	pos.z +=  p.z;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->move(pos);
		}
	}
}
//----------------------------------------------------------------
bool puzzle::isMoving(){
	//returns the state of the moving boolean
	//to prevent key press events from messing up the movements
	moving = false;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			moving = myCubies[i]->isMoving();
			if(moving==true){
				return moving;
			}
		}
	}
	return moving;
}
//---------------------------------------------------------------
int puzzle::rotateTwoIds(int cubieA, int cubieB,bool inside){
	//receives two ids, from two cubies
	//each cubie con do 6 possible moves
	//each cubie ahs 6 possible gestures, starting from him
	//6 starting from cubie, + 4 ending at cubie. 

	///////////////////////////////////////////////////////////
	//it returns an int
	//the first number of the int is x-1, y-2,z-3
	//the second number of the int is 0-false, 1-true
	///////////////////////////////////////////////////////////
	int selected[9];
	int counter=0;
	int x1 =0;
	int y1 =0;
	int z1 =0;
	int x2 =0;
	int y2 =0;
	int z2 =0;

	// first find the x-y-z of both cubies
	//look for positon of cubie 1 on the 3d data structure
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			for(int z=0;z<3;z++){
				if(three_dim1[x][y][z] == cubieA){
					//when selected cubie is found
					x1 = x;
					y1 = y;
					z1 = z;
				}
			}
		}
	}
	//look for positon of cubie 2 on the 3d data structure
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			for(int z=0;z<3;z++){
				if(three_dim1[x][y][z] == cubieB){
					//when selected cubie is found
					x2 = x;
					y2 = y;
					z2 = z;
				}
			}
		}
	}

	//decide which are the two possible planes to rotate
	SG_VECTOR axx = {1,0,0};
	SG_VECTOR axy = {0,1,0};
	SG_VECTOR axz = {0,0,1};
	//look for the similar axis components
	if(x1 == x2 && y1 == y2){
		// x & y ==
		//se the other component
		if(z1 > z2){
			//z decreases
			//x-cc//y-cc
			//for now the choice (x or y) is made randomly
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//x-cc
				rotateByIDandAxis(cubieA, axx, false);
				return 10;
			}else{
				//y-cc
				rotateByIDandAxis(cubieA, axy, false);
				return 20;
			}
		}else{
			//z increases
			//x-c//y-c
			//for now the choice (x or y) is made randomly
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//x-c
				rotateByIDandAxis(cubieA, axx, true);
				return 11;
			}else{
				//y-c
				rotateByIDandAxis(cubieA, axy, true);
				return 21;
			}
		}
	}else if (x1 == x2 && z1 == z2){
		// x & z ==
		//se the other component
		if(y1 > y2){
			//y decreases
			//x-c//z-cc
			//for now the choice (x or y) is made randomly
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//x-c
				rotateByIDandAxis(cubieA, axx, true);
				return 11;
			}else{
				//z-cc
				rotateByIDandAxis(cubieA, axz, false);
				return 30;
			}
		}else{
			//y increases
			//x-cc//z-c
			//for now the choice (x or y) is made randomly
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//x-cc
				rotateByIDandAxis(cubieA, axx, false);
				return 10;
			}else{
				//z-c
				rotateByIDandAxis(cubieA, axz, true);
				return 31;
			}
		}
	}else if(y1 == y2 && z1 == z2){
		// y & z ==
		//se the other component
		if(x1 > x2){
			//x decreases
			//y-cc//z-cc
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//y-cc
				rotateByIDandAxis(cubieA, axy, false);
				return 20;
			}else{
				//z-cc
				rotateByIDandAxis(cubieA, axz, false);
				return 30;
			}
		}else{
			//x increases
			//y-c//z-c
			string u = ofToString( ofRandom(0,100));
			int ug = ofToInt(u);
			if(ug%2 == 0 ){
				//y-c
				rotateByIDandAxis(cubieA, axy, true);
				return 21;
			}else{
				//z-c
				rotateByIDandAxis(cubieA, axz, true);
				return 31;
			}
		}
	}
}
//---------------------------------------------------------------------------
void puzzle::rotateByIDandAxis(int id, SG_VECTOR axis, bool dir){
	//it receives the id of a cubie,the axis and the direction of rotation
	//it looks for the other 9 ids, according to the axis
	//and makes those 9 myCubies[]->faceRotate
	int selected[9];
	int counter=0;
	int selX =0;
	int selY =0;
	int selZ =0;
	//look for positon of cubie on the 3d data structure
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			for(int z=0;z<3;z++){
				if(three_dim1[x][y][z] == id){
					//when selected cubie is found
					selX = x;
					selY = y;
					selZ = z;
				}
			}
		}
	}
	//now we ask for the cubies on that axis
	if(axis.x != 0){
		//if the move is on an x axis
		for(int y=0;y<3;y++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[selX][y][z];
				counter ++;
			}
		}
		//now we re-arrange ids on the 3d array
		//according to axis of rotation
		// and actual selected plane: selX = x; selY = y; selZ = z;
		rearange3dArray(axis,selX,dir);
	}else if(axis.y != 0){
		//if the move is on a y axis
		for(int x=0;x<3;x++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[x][selY][z];
				counter ++;
			}
		}
		rearange3dArray(axis,selY,dir);
	}else{
		//if the move is on a z axis
		for(int x=0;x<3;x++){
			for(int y=0; y<3;y++){
				selected[counter] = three_dim1[x][y][selZ];
				counter ++;
			}
		}
		rearange3dArray(axis,selZ,dir);
	}
	//now we tell the 9 selected cubies to rotate
	for(int i=0;i<9;i++){
		myCubies[selected[i]]->faceRotate(axis,dir);
	}
	//myCubies[11]->faceRotate(axis,deg,dir);

	//rearrange cubies position
	//do we do this after we complete 90 deg rotation???
	/////it doesnt matter, it can be that instant, since the 3d array is only looked upon before moving
	/////the animation will lock selection of new cubie, so on ly one movement is done at a time
	/////so the re-aranging of numbers can happen "during" the animation
}
//---------------------------------------------------------------------------
void puzzle::rotateByIDandAxis(int id, SG_VECTOR axis, bool dir,float angle){
	//it receives the id of a cubie,the axis and the direction of rotation
	//it looks for the other 9 ids, according to the axis
	//and makes those 9 myCubies[]->faceRotate

	//////////int selected[9];//now its global to do mouse release animations

	int counter=0;
	selX =0;//now its global to do mouse release animations
	selY =0;
	selZ =0;
	//look for positon of cubie on the 3d data structure
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			for(int z=0;z<3;z++){
				if(three_dim1[x][y][z] == id){
					//when selected cubie is found
					selX = x;
					selY = y;
					selZ = z;
				}
			}
		}
	}
	//now we ask for the cubies on that axis
	if(axis.x != 0){
		//if the move is on an x axis
		for(int y=0;y<3;y++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[selX][y][z];
				counter ++;
			}
		}
		//now we re-arrange ids on the 3d array
		//according to axis of rotation
		// and actual selected plane: selX = x; selY = y; selZ = z;
		//rearange3dArray(axis,selX,dir);
	}else if(axis.y != 0){
		//if the move is on a y axis
		for(int x=0;x<3;x++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[x][selY][z];
				counter ++;
			}
		}
		//rearange3dArray(axis,selY,dir);
	}else{
		//if the move is on a z axis
		for(int x=0;x<3;x++){
			for(int y=0; y<3;y++){
				selected[counter] = three_dim1[x][y][selZ];
				counter ++;
			}
		}
		//rearange3dArray(axis,selZ,dir);
	}

	//look for rotation constraints
	//user could be dragging the rotating face beyond he 90 deg move

	if(myCubies[selected[0]]->masterAngle < 90 && myCubies[selected[0]]->masterAngle > -90){
		if(ofSign(myCubies[selected[0]]->masterAngle) > 0){
			//positive rotation in relation to original position
			if((myCubies[selected[0]]->masterAngle + angle) > 90){
				//move until 90
				angle = 90 - myCubies[selected[0]]->masterAngle;

				for(int i=0;i<9;i++){
					//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
					myCubies[selected[i]]->faceRotate(axis,dir,angle);
				}
			}else{
				//now we tell the 9 selected cubies to rotate
				//bool rearrange=false;
				for(int i=0;i<9;i++){
					//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
					myCubies[selected[i]]->faceRotate(axis,dir,angle);
				}
			}
		}else{
			//negative rotation in relation to original position
			if((myCubies[selected[0]]->masterAngle + angle) < -90){
				//move until -90
				angle = -90 - myCubies[selected[0]]->masterAngle;

				for(int i=0;i<9;i++){
					//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
					myCubies[selected[i]]->faceRotate(axis,dir,angle);
				}
			}else{
				//now we tell the 9 selected cubies to rotate
				//bool rearrange=false;
				for(int i=0;i<9;i++){
					//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
					myCubies[selected[i]]->faceRotate(axis,dir,angle);
				}
			}
		}
	}else if(myCubies[selected[0]]->masterAngle == 90){
		if((myCubies[selected[0]]->masterAngle + angle) < 90){
			for(int i=0;i<9;i++){
				//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
				myCubies[selected[i]]->faceRotate(axis,dir,angle);
			}
		}
	}else if(myCubies[selected[0]]->masterAngle == -90){
		//negative rotation in relation to original position
		if((myCubies[selected[0]]->masterAngle + angle) > -90){
			for(int i=0;i<9;i++){
				//rearrange = myCubies[selected[i]]->faceRotate(axis,dir,angle);
				myCubies[selected[i]]->faceRotate(axis,dir,angle);
			}
		}
	}


}
//--------------------------------------------------------------------------------------------
void puzzle::decideMove(){
	//ask for the current masterangle of the selected cubies
	//selected[] will have the latest 9 cubies active on the rotation

	int angleM;
	bool dirM;

	//only need to ask one of the selected cubies, thay all have the same rotation angle
	angleM = myCubies[selected[0]]->masterAngle;
	dirM =  myCubies[selected[0]]->dir;

	if(dirM == true){
		//positive angle
		if(angleM >= 45){
			for(int i=0;i<9;i++){
				//only need to ask one of the selected cubies, thay all have the same rotation angle
				myCubies[selected[i]]->goForward();
			}
			//rearrange cubies involved on the move
			if(myCubies[selected[0]]->vrotFace.x != 0){
				rearange3dArray(myCubies[selected[0]]->vrotFace,selX,dirM);
			}else if(myCubies[selected[0]]->vrotFace.y != 0){
				rearange3dArray(myCubies[selected[0]]->vrotFace,selY,dirM);
			}else{
				rearange3dArray(myCubies[selected[0]]->vrotFace,selZ,dirM);
			}
		}else{
			for(int i=0;i<9;i++){
				//only need to ask one of the selected cubies, thay all have the same rotation angle
				myCubies[selected[i]]->goBack();
			}
		}
	}else{
		//negative angle
		if(angleM <= -45){
			for(int i=0;i<9;i++){
				//only need to ask one of the selected cubies, thay all have the same rotation angle
				myCubies[selected[i]]->goForward();
			}
			//rearrange cubies involved on the move
			if(myCubies[selected[0]]->vrotFace.x != 0){
				rearange3dArray(myCubies[selected[0]]->vrotFace,selX,dirM);
			}else if(myCubies[selected[0]]->vrotFace.y != 0){
				rearange3dArray(myCubies[selected[0]]->vrotFace,selY,dirM);
			}else{
				rearange3dArray(myCubies[selected[0]]->vrotFace,selZ,dirM);
			}
		}else{
			for(int i=0;i<9;i++){
				//only need to ask one of the selected cubies, thay all have the same rotation angle
				myCubies[selected[i]]->goBack();
			}
		}
	}


}
//--------------------------------------------------------------------------------------------
void puzzle::update(string _eventName, SubObEvent* _event){
	/*
	if(_eventName == "ibox-bl:1"){
		int phase = _event->getArg("phase")->getInt();
		//cout << "puzzle phase = " << phase << endl;
		if(phase == 0){
			ofVec2f pos = _event->getArg("absPos")->getVec2();
			//myCubies[1]->setMousePoint(ofVec3f(pos.x, pos.y, 0));
			if(isInside(pos.x, pos.y)){
				cout << "puzzle got cubie." << endl;
			}
		}
		if(phase == 1){
			ofVec2f pos = _event->getArg("absPos")->getVec2();
			bool result = isInside(pos.x, pos.y);
		}
		if(phase == 2){
			doRotation();
		}
	}
	if(_eventName == "ibox-bl:0"){
		//cout << "doing rotation." << endl;
		doRotation();
	}
	*/
}

void puzzle::unprojectPoint(){
	//cout << "cubie unprojecting point. - " << _pnt.x << ", " << _pnt.y << ", " << _pnt.z << endl;
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float) mousePoint.x;
	winY = (float)viewport[3] - mousePoint.y;
	glReadPixels( mousePoint.z, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	cout << "mouse position = " << posX << ", " << posY << ", " << posZ << endl;
	//cout << "cube postion = " << pos.x << ", " << pos.y << ", " << pos.z << endl;

	unprojectedPoint.set(posX, posY, posZ);
	bHavePoint = true;
}

void puzzle::checkCubiesForHit(ofVec3f _pnt){
	float nearest = 10000.0;
	int nearestId = -1;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			//ofVec3f centroid = myCubies[i]->getCentroidScreen();
			//float dist = centroid.distance(mouse);
			//float dist = myCubies[i]->getDistanceByVertex(mouse);
			float dist = myCubies[i]->getDistanceByCentroid(_pnt);
			cout << "dist = " << dist << endl;
			if(dist < nearest){
				nearestId = myCubies[i]->getId();
				//cout << "nearest = " << i << endl;
				nearest = dist;
			}
		}
	}
	if(nearestId != -1){
		if(nearest < MAX_DIST){
			if(activeCubie != nearestId){
				if(activeCubie > -1){
					//myCubies[activeCubie]->setDraw(true);
				}
				activeCubie = nearestId;
				//myCubies[activeCubie]->setDraw(false);
				activeTriangle = myCubies[activeCubie]->getNearestTri(_pnt);
				//ofVec3f n = activeTriangle.getNormal();
				//vector<Triangle> tris = myCubies[activeCubie]->getTrianglesByNormal(n);
				//ofSeedRandom();
				//ofFloatColor rColor = ((float)ofRandom(0,255) / 255.0, (float)ofRandom(0,255) / 255.0, (float)ofRandom(0,255) / 255.0);
				//myCubies[activeCubie]->setColorToSet(tris, rColor);
				//cout << "Nearest tri normal = " << n.x << ", " << n.y << ", " << n.z << endl;
			}
		}
	}
}

bool puzzle::isInside(int _x, int _y){
	//cout << "puzzle checking insides" << endl;
	ofVec3f mouse(_x,_y, 0);
	/*
	if(!bUnproject){
	mousePoint = mouse;
	bUnproject = true;
	}
	*/
	float nearest = 10000.0;
	int nearestId = -1;
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			//ofVec3f centroid = myCubies[i]->getCentroidScreen();
			//float dist = centroid.distance(mouse);
			//float dist = myCubies[i]->getDistanceByVertex(mouse);
			//cout << "dist = " << dist << endl;
			float dist = myCubies[i]->getDistanceByCentroid(mouse);
			if(dist < nearest){
				nearestId = myCubies[i]->getId();
				//cout << "nearest = " << i << endl;
				nearest = dist;
				//ofVec3f tVertex = myCubies[i]->getNearestVertex();
				ofVec3f tVertex = myCubies[i]->getCentroidScreen();
				if(bDrawLine){
					lineStart.set(mouse.x, mouse.y, mouse.z);
					lineStop.set(tVertex.x, tVertex.y, tVertex.z);
				}
				//cout << "dist = " << dist << endl;
			}
		}
	}
	if(nearestId != -1){
		if(nearest < MAX_DIST){
			/*
			bHaveLine = true;
			if(bHaveActiveCubie){
			myCubies[activeCubie]->setDrawWire(false);
			} else {
			bHaveActiveCubie = true;
			}
			if(!bHaveActiveCubie){
			activeCubie = nearestId;
			myCubies[activeCubie]->setDrawWire(true);
			} else {
			rotationCubie = nearestId;
			myCubies[rotationCubie]->setDrawWire(true);
			bHaveRotationCubie = true;
			}
			return true;
			*/
			if(!bHaveActiveCubie){
				activeCubie = nearestId;
				bHaveActiveCubie = true;
			} else if(nearestId != activeCubie){
				rotationCubie = nearestId;
				bHaveRotationCubie = true;
			}
		} else {
			bHaveLine = false;
			bHaveActiveCubie = false;
		}
	}
	return false;
}

void puzzle::dragInput(ofVec3f _pnt){
	if(activeCubie > -1){
		//ofVec3f pnt = _pnt.normalize();
		ofVec3f normal = activeTriangle.getNormal();
		ofVec3f dir = getDir(_pnt);
		float angle = getMainComponent(_pnt);
		dir.normalize();
		ofVec3f axis = getDir(dir.getCrossed(normal));
		//float angle = 0;
		cout << "Dragged - " << _pnt.x << ", " << _pnt.y << ", " << _pnt.z << endl;
		cout << "Dir - " << dir.x << ", " << dir.y << ", " << dir.z << endl;
		cout << "Normal = " << normal.x << ", " << normal.y << ", " << normal.z << endl;
		cout << "Axis - " << axis.x << ", " << axis.y << ", " << axis.z << endl;
		cout << "Angle = " << angle << endl;
		/*
		if(abs(pnt.x) > abs(pnt.y) && abs(pnt.x) > abs(pnt.z)){
		angle = pnt.x;
		pnt.x = 0;
		pnt.z = 0;
		pnt.y = 1;
		}
		if(abs(pnt.y) > abs(pnt.x) && abs(pnt.y) > abs(pnt.z)){
		angle = pnt.y;
		pnt.y = 0;
		pnt.z = 0;
		pnt.x = 1;
		}
		if(abs(pnt.z) > abs(pnt.x) && abs(pnt.z) > abs(pnt.y)){
		angle = pnt.z;
		pnt.x = 0;
		pnt.z = 0;
		pnt.y = 1;
		}
		//angle = 2.0;
		cout << "Cubie " << id << " got drag - " << pnt.x << ", " << pnt.y << ", " << pnt.z << " angle = " << angle << endl; 
		cout << "Cubie " << id << " from - " << _pnt.x << ", " << _pnt.y << ", " << _pnt.z << endl; 
		//cout << "Cubie " << id << " got drag - " << pnt.x << ", " << pnt.y << ", " << pnt.z << " angle = " << angle << endl; 
		//SG_VECTOR v = {pnt.x, pnt.y, pnt.z};
		if(!bHaveAxis){
		v.x = pnt.x;
		v.y = pnt.y;
		v.z = pnt.z;
		bHaveAxis = true;
		} else {
		SG_VECTOR t = {pnt.x, pnt.y, pnt.z};
		if(t.x == v.x && t.y == v.y && t.z == v.z){
		rotateByIDandAxis(activeCubie, v, true, angle);
		}
		}
		//faceRotate(v, true, angle);
		//myCubies[activeCubie]->dragInput(_pnt);
		*/
		if(!bHaveAxis){
			v.x = axis.x;
			v.y = axis.y;
			v.z = axis.z;
			bHaveAxis = true;
		} else {
			SG_VECTOR t = {axis.x, axis.y, axis.z};
			if(t.x == v.x && t.y == v.y && t.z == v.z){
				bool bDir = true;
				if(angle < 0.0){
					bDir = false;
				}
				rotateByIDandAxis(activeCubie, v, bDir, angle);		
			}
		}
	}
}

void puzzle::changeFaceColor(ofVec3f _pnt, ofFloatColor _c){
	activeCubie = -1;
	checkCubiesForHit(_pnt);
	if(activeCubie > -1){
		ofVec3f n = activeTriangle.getNormal();
		vector<Triangle> tris = myCubies[activeCubie]->getTrianglesByNormal(n);
		myCubies[activeCubie]->setColorToSet(tris, _c);
	}
}

ofVec3f puzzle::getDir(ofVec3f _pnt){
	ofVec3f x(abs(_pnt.x), 0, 0);
	ofVec3f y(0, abs(_pnt.y), 0);
	ofVec3f z(0, 0, abs(_pnt.z));
	float magX = x.lengthSquared();
	float magY = y.lengthSquared();
	float magZ = z.lengthSquared();
	if(magX > magY && magX > magZ){
		return x;
	}
	if(magY > magX && magY > magZ){
		return y;
	}
	if(magZ > magX && magZ > magY){
		return z;
	}
}

float puzzle::getMainComponent(ofVec3f _pnt){
	ofVec3f x(_pnt.x, 0, 0);
	ofVec3f y(0, _pnt.y, 0);
	ofVec3f z(0, 0, _pnt.z);
	float magX = x.lengthSquared();
	float magY = y.lengthSquared();
	float magZ = z.lengthSquared();
	if(magX > magY && magX > magZ){
		return _pnt.x;
	}
	if(magY > magX && magY > magZ){
		return _pnt.y;
	}
	if(magZ > magX && magZ > magY){
		return _pnt.z;
	}
}

void puzzle::doRotation(){
	if(bHaveActiveCubie && bHaveRotationCubie){
		//rotateTwoIds(activeCubie, rotationCubie, true);
		faceRotateB = true;
		//bHaveActiveCubie = bHaveRotationCubie = false;
		cout << "double rotation." << endl;
	} else if(bHaveActiveCubie == true){
		//rotateTwoIds(activeCubie, activeCubie, true);
		faceRotateB = true;
		//bHaveActiveCubie = false;
		cout << "single rotation." << endl;
	}
}

void puzzle::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	if(bHaveLine){
		lineStart = _absPos;
	}
	cout << _phase << endl;
}
//----------------------------------------------------------------
void puzzle::rearange3dArray(SG_VECTOR axis, int plane, bool dir){
	//rearanges ids of cubies inside the 3d array
	//it deppends on the axis, plane on that axis, and the direction of rotation!!

	int store[9] = {0,0,0,0,0,0,0,0,0};
	int ctr =0;
	int cnstplane = plane;
	if(dir == true){
		//clockwise rotations
		if(axis.x != 0){
			//rotation on X
			//store all the values
			for(int y=0;y<3;y++){
				for(int z=0; z<3; z++){
					store[ctr]=three_dim1[cnstplane][y][z];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int z=2; z>-1; z--){
				for(int y=0; y<3; y++){
					three_dim1[cnstplane][y][z] = store[ctr];
					ctr ++;
				}
			}
		}else if(axis.y != 0){
			//rotation on Y
			//store all the values
			for(int x=0;x<3;x++){
				for(int z=0; z<3; z++){
					store[ctr]=three_dim1[x][cnstplane][z];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int z=0; z<3; z++){
				for(int x=2; x>-1; x--){
					three_dim1[x][cnstplane][z] = store[ctr];
					ctr ++;
				}
			}

		}else{
			//rotation on Z
			//store all the values
			for(int x=0;x<3;x++){
				for(int y=0; y<3; y++){
					store[ctr]=three_dim1[x][y][cnstplane];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int y=0; y < 3; y++){
				for(int x=2 ; x>-1; x--){
					three_dim1[x][y][cnstplane] = store[ctr];
					ctr ++;
				}
			}
		}
	}else{
		//counter clockwise rotations
		if(axis.x != 0){
			//rotation on X
			//store all the values
			for(int y=0;y<3;y++){
				for(int z=0; z<3; z++){
					store[ctr]=three_dim1[cnstplane][y][z];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int z=0; z<3; z++){
				for(int y=2; y>-1; y--){
					three_dim1[cnstplane][y][z] = store[ctr];
					ctr ++;
				}
			}

		}else if(axis.y != 0){
			//rotation on Y
			//store all the values
			for(int x=0;x<3;x++){
				for(int z=0; z<3; z++){
					store[ctr]=three_dim1[x][cnstplane][z];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int z=2; z>-1; z--){
				for(int x=0; x<3; x++){
					three_dim1[x][cnstplane][z] = store[ctr];
					ctr ++;
				}
			}
		}else{
			//rotation on Z
			//store all the values
			for(int x=0;x<3;x++){
				for(int y=0; y<3; y++){
					store[ctr]=three_dim1[x][y][cnstplane];
					ctr ++;
				}
			}
			ctr=0;
			//put values in new places on 3d array
			for(int y=2; y>-1; y--){
				for(int x=0 ; x<3; x++){
					three_dim1[x][y][cnstplane] = store[ctr];
					ctr ++;
				}
			}
		}
	}

}
//----------------------------------------------------------------
void puzzle::colorFaces(int objectID){
	////goes through each cubie and makes sets of normals.. to determine all different normals in the object
	//and apply colors to those normals
	ofRender *ofr = new ofRender();

	if((objectID != 4) && (objectID != 1) && (objectID != 200)){
		//not the bunny or the cube -> they were already colored on puzzle::loadPieces->cubie::setObjects
		ofr->colorFaces(myCubies,numPieces,0.01, objectID);
	}
	if(objectID == 200){
		//extrudd object
		ofr->colorFacesExtruded(myCubies,numPieces,0.01, objectID);
	}
	if(objectID != 4){
		//color black all the inside faces of each cubie (after all other face colors have been applied)
		//all the puzzles have to do this
		colorCubiesBlackSides();
		//need to color black sides of bunny in a better way.. will they be colored? or leave it plain?
	}
	free(ofr);
}
//----------------------------------------------------------------
void puzzle::colorTorus(){
	//currently not used since torus will be only one solid color
	for(int i=0;i<numPieces;i++){
		//set random color for each cubie on the torus
		myCubies[i]->colorTorus();
	}
	colorCubiesBlackSides();
}
//----------------------------------------------------------------
void puzzle::colorCubiesBlackSides(){
	//it takes care of the "inside" coloring for each cubie
	//for the platonian solids other than cube
	for(int i=0;i<numPieces;i++){
		myCubies[i]->colorBlackSides(i,0.1);
	}
}
//----------------------------------------------------------------
void puzzle::changeColorToColor(ofFloatColor Sc, ofFloatColor Tc){
	for(int i=0;i<numPieces;i++){
		myCubies[i]->changeColorToColor(Sc,Tc);
	}
}
//----------------------------------------------------------------
void puzzle::unDo(int id, SG_VECTOR axis, bool dir){
	//it receives the id of a cubie,the axis and the direction of rotation
	//it looks for the other 9 ids, according to the axis
	//and makes those 9 myCubies[]->faceRotate
	int selected[9];
	int counter=0;
	int selX =0;
	int selY =0;
	int selZ =0;
	//look for positon of cubie on the 3d data structure
	for(int x=0;x<3;x++){
		for(int y=0;y<3;y++){
			for(int z=0;z<3;z++){
				if(three_dim1[x][y][z] == id){
					//when selected cubie is found
					selX = x;
					selY = y;
					selZ = z;
				}
			}
		}
	}
	//now we ask for the cubies on that axis
	if(axis.x == 1){
		//if the move is on an x axis
		for(int y=0;y<3;y++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[selX][y][z];
				counter ++;
			}
		}
		//now we re-arrange ids on the 3d array
		//according to axis of rotation
		// and actual selected plane: selX = x; selY = y; selZ = z;
		rearange3dArray(axis,selX,dir);
	}else if(axis.y == 1){
		//if the move is on a y axis
		for(int x=0;x<3;x++){
			for(int z=0; z<3;z++){
				selected[counter] = three_dim1[x][selY][z];
				counter ++;
			}
		}
		rearange3dArray(axis,selY,dir);
	}else{
		//if the move is on a z axis
		for(int x=0;x<3;x++){
			for(int y=0; y<3;y++){
				selected[counter] = three_dim1[x][y][selZ];
				counter ++;
			}
		}
		rearange3dArray(axis,selZ,dir);
	}
	//now we tell the 9 selected cubies to rotate
	for(int i=0;i<9;i++){
		myCubies[selected[i]]->unDo(axis,dir);
	}
}
//----------------------------------------------------------------
void puzzle::exit(){

	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->exit();
		}
	}
	//// De-Allocate memory to prevent memory leak
	//for (int i = 0; i < HEIGHT; ++i) {
	//	for (int j = 0; j < WIDTH; ++j){
	//		delete [] three_dim1[i][j];
	//	}
	//	delete [] three_dim1[i];
	//}
	//delete [] three_dim1;
	free(myCubies);
	//deactivate();
}
