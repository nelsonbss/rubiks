#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"

#include <vector>
#include <iostream>

#define HEIGHT 3
#define WIDTH 3
#define DEPTH 3

puzzle::puzzle(float x, float y, float z){
	numPieces = 27;
	myCubies = (cubie**)malloc(numPieces*sizeof(cubie*));

	pos.x = x;
	pos.y = y;
	pos.z = z;

	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
	/*
	create 3 * 10 * 25 array filled with '12'
	*/
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
	std::cout << "It should be 13: " << three_dim1[0][0][0] << "\n";
	std::cout << "It should be 4: " << three_dim1[0][1][0] << "\n";

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
}
//--------------------------------------------------------------
void puzzle::setup(){
}
//--------------------------------------------------------------
void puzzle::update(){
	//iterate through cubies
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->update();
		}
	}
}
//--------------------------------------------------------------
void puzzle::draw(){  
	//puzzle tells every cubie to attach objects to scene
	for(int i=0;i<numPieces;i++){
		if(myCubies[i] != NULL){
			myCubies[i]->draw();
		}
	}
}
//--------------------------------------------------------------
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
//--------------------------------------------------------------
void puzzle::loadPieces(sgCGroup **pcs,bool plain){
	//it loads the pieces that the slicer made, the pieces are in a sgCGroup** pieces[], 
	//this function receives a copy of that sgCGroup** made by mySlicer->getPieces()
	//it loads them into its own cubies
	//create cubies
	//so each time there is a new boolean operation, whole new cubies get created with variables in zero or blank
	for(int i =0;i<numPieces;i++){
		cubie *auxCubie = new cubie(pos.x,pos.y,pos.z,i+1);// is this really creating independent instances of cubie??
		//auxCubie->setup();
		//add this cubie to mycubies[]
		myCubies[i] = auxCubie;
	}
	for(int i=0;i<numPieces;i++){
		//get group from pieces[] copy
		sgCGroup *part = pcs[i];

		if(part != NULL){
			const int ChCnt = part->GetChildrenList()->GetCount();
			sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
			part->BreakGroup(allParts);
			sgCObject::DeleteObject(part);

			sgCObject **obj = (sgCObject**)malloc(50*sizeof(sgCObject*));
			int realNumPieces=0;

			for (int j=0; j < ChCnt; j++){
				//clone each part
				sgCObject *tempOBJ = allParts[j]; //each is an object that goes in one cubie
				obj[j] = tempOBJ;
				realNumPieces ++;
			}
			//make them a group
			sgCGroup* cubieGroup = sgCGroup::CreateGroup(obj,realNumPieces);  
			//put that gorup inside temp cubie
			myCubies[i]->setObjects(cubieGroup,i,plain);//here goes the group of clones from the iriginal slicing pieces[]
			//put that cubie on the cubies[]

			free(obj);
			free(allParts);
		}else{
			myCubies[i]->setObjects(NULL,i,true);
		}
	}
	//sgCObject::DeleteObject(*pcs);
}
////////////////////////////////////////////////////////////////
void puzzle::rotate(SG_VECTOR r){  
	//puzzle tells every cubie to rotate
	rot.x +=  r.x;
	rot.y +=  r.y;
	rot.z +=  r.z;
	for(int i=0;i<numPieces;i++){
		myCubies[i]->rotate(rot);
	}
}
////////////////////////////////////////////////////////////////
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
//---------------------------------------------------------------
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
		myCubies[selected[i]]->faceRotate(axis,dir);
	}
	//myCubies[11]->faceRotate(axis,deg,dir);

	//rearranging
	//do we do this after we complete 90 deg rotation???
	/////it doesnt matter, it can be that instant, since the 3d array is only looked upon before moving
	/////the animation will lock selection of new cubie, so on ly one movement is done at a time
	/////so the re-aranging of numbers can happen "during" the animation
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
		if(axis.x == 1){
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
		}else if(axis.y == 1){
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
		if(axis.x == 1){
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

		}else if(axis.y == 1){
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
//---------------------------------------------------------------
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
	//		delete [] three_dim[i][j];
	//	}
	//	delete [] three_dim[i];
	//}
	//delete [] three_dim;
	free(myCubies);
}