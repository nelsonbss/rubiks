#include "cutter.h"
#include "sgCore.h"

cutter::cutter(float thick, float tamPlane, float tamCuby){
	cutterThick = thick;
	cutterSize = tamPlane;
	tamCubie = tamCuby;
	posX=0;
	posY=0;

	planes = (sgCObject**)malloc(6*sizeof(sgCObject*));  //allocate memory
}
//--------------------------------------------------------------
void cutter::setup(){
	if(numCutter==1){
		////////////////////// BOX 1 -> 6///////////////////////////////////////////////////////////////////////
		x1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//move plane to the middle of its side
		SG_VECTOR transBox1 = {0,-cutterSize/2,0}; 
		x1->InitTempMatrix()->Translate(transBox1);
		//move plane to the LEFT edge of the tamPiece
		transPlaneX1.x =-tamCubie/2;
		transPlaneX1.y = 0;
		transPlaneX1.z = 0;
		x1->GetTempMatrix()->Translate(transPlaneX1);
		//will be rotated to be parallel to x planes  
		SG_POINT rotCen1 = {-tamCubie/2,0,0};
		//it is orignally parallel to z planes
		//rotate it around y axis 90deg= 1.57079633 rad
		SG_VECTOR rotDir1 = {0,1,0};
		x1->GetTempMatrix()->Rotate(rotCen1,rotDir1, 1.57079633);
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox11 = {0,0,cutterSize/2}; //move towards viwer z 
		x1->GetTempMatrix()->Translate(transBox11);
		//////
		x1->ApplyTempMatrix();  
		x1->DestroyTempMatrix();
		x1->SetAttribute(SG_OA_COLOR,8);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		x2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//move plane to the middle of its side
		SG_VECTOR transBox2 = {0,-cutterSize/2,0}; 
		x2->InitTempMatrix()->Translate(transBox2);
		//move plane to the RIGHT edge of the tamPiece: 
		transPlaneX2.x = tamCubie/2;
		transPlaneX2.y = 0;
		transPlaneX2.z = 0;
		x2->GetTempMatrix()->Translate(transPlaneX2);
		//will be rotated to be parallel to x planes
		SG_POINT rotCen2 = {tamCubie/2,0,0};
		//it is orignally parallel to z planes
		//rotate it around y axis 90deg= 1.57079633 rad
		SG_VECTOR rotDir2 = {0,1,0};
		x2->GetTempMatrix()->Rotate(rotCen2,rotDir2, 1.57079633); 
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox22 = {0,0,cutterSize/2}; //move towards viwer z 
		x2->GetTempMatrix()->Translate(transBox22);
		//////
		x2->ApplyTempMatrix();  
		x2->DestroyTempMatrix();
		x2->SetAttribute(SG_OA_COLOR,9);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		y1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//move plane to the middle of its side
		SG_VECTOR transBox3 = {-cutterSize/2,0,0}; 
		y1->InitTempMatrix()->Translate(transBox3);
		//move plane to the UPPER edge of the tamPiece
		transPlaneY1.x = 0;
		transPlaneY1.y = -tamCubie/2;
		transPlaneY1.z = 0;
		y1->GetTempMatrix()->Translate(transPlaneY1);
		//will be rotated to be parallel to y planes
		SG_POINT rotCen3 = {0,-tamCubie/2,0};
		////it is orignally parallel to z planes
		////rotate it around x axis -90deg= -1.57079633 rad
		SG_VECTOR rotDir3 = {1,0,0};
		y1->GetTempMatrix()->Rotate(rotCen3,rotDir3, -1.57079633); 
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox33 = {0,0,cutterSize/2}; //move towards viwer z 
		y1->GetTempMatrix()->Translate(transBox33);
		//////
		y1->ApplyTempMatrix();  
		y1->DestroyTempMatrix();
		y1->SetAttribute(SG_OA_COLOR,10);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		y2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//move plane to the middle of its side
		SG_VECTOR transBox4 = {-cutterSize/2,0,0}; 
		y2->InitTempMatrix()->Translate(transBox4);
		//move plane to the UPPER edge of the tamPiece 
		transPlaneY2.x = 0;
		transPlaneY2.y = tamCubie/2;
		transPlaneY2.z = 0;
		y2->GetTempMatrix()->Translate(transPlaneY2);
		//will be rotated to be parallel to y planes
		SG_POINT rotCen4 = {0,tamCubie/2,0};
		////it is orignally parallel to z planes
		////rotate it around x axis -90deg= -1.57079633 rad
		SG_VECTOR rotDir4 = {1,0,0};
		y2->GetTempMatrix()->Rotate(rotCen4,rotDir4, -1.57079633); 
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox44 = {0,0,cutterSize/2}; //move towards viwer z 
		y2->GetTempMatrix()->Translate(transBox44);
		//////
		y2->ApplyTempMatrix();  
		y2->DestroyTempMatrix();
		y2->SetAttribute(SG_OA_COLOR,11);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		z1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//will not be rotated
		//move plane so that its center is on 0,0
		//move on x axis
		SG_VECTOR transBox5 = {-cutterSize/2,0,0}; 
		z1->InitTempMatrix()->Translate(transBox5);
		////move on y axis
		SG_VECTOR transBox55 = {0,-cutterSize/2,0};
		z1->GetTempMatrix()->Translate(transBox55);
		////move down (inside of the screen) on z axis
		transPlaneZ1.x = 0;
		transPlaneZ1.y = 0;
		transPlaneZ1.z = -((cutterSize/2)-(tamCubie/2));
		z1->GetTempMatrix()->Translate(transPlaneZ1);
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox55a = {0,0,cutterSize/2}; //move towards viwer z 
		z1->GetTempMatrix()->Translate(transBox55a);
		//update z plane position
		transPlaneZ1.z = transPlaneZ1.z + (cutterSize/2);
		////////
		z1->ApplyTempMatrix();
		z1->DestroyTempMatrix();
		z1->SetAttribute(SG_OA_COLOR,12);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		z2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
		//will not be rotated
		//move plane so that its center is on 0,0
		//move on x axis
		SG_VECTOR transBox6 = {-cutterSize/2,0,0}; 
		z2->InitTempMatrix()->Translate(transBox6);
		////move on y axis
		SG_VECTOR transBox66 = {0,-cutterSize/2,0};
		z2->GetTempMatrix()->Translate(transBox66);
		////move down (inside of the screen) on z axis
		transPlaneZ2.x = 0;
		transPlaneZ2.y = 0;
		transPlaneZ2.z = -((cutterSize/2)+(tamCubie/2));
		z2->GetTempMatrix()->Translate(transPlaneZ2);
		//translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
		//remember -z is towards inside the screen
		SG_VECTOR transBox66a = {0,0,cutterSize/2}; //move towards viwer z 
		z2->GetTempMatrix()->Translate(transBox66a);
		//update z plane position
		transPlaneZ2.z = transPlaneZ2.z + (cutterSize/2);
		////////
		z2->ApplyTempMatrix();  
		z2->DestroyTempMatrix();
		z2->SetAttribute(SG_OA_COLOR,13);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//put boxes/planes on the array of pointer
		//to pass it along and create a group
		//to be able to return it as a group to the main class
		planes[0] = x1;
		planes[1] = x2;
		planes[2] = y1;
		planes[3] = y2;
		planes[4] = z1;
		planes[5] = z2;

		//////create group////////////////////////////////////////////////////////////////////////////////////
		allPlanes = sgCGroup::CreateGroup(planes,6);
	}else{
		///make cubes	


		///put cubes in [] to make group
		cubes[0] = c1;
		cubes[1] = c2;
		cubes[2] = c3;
		cubes[3] = c4;
		cubes[4] = c5;
		cubes[5] = c6;
		cubes[6] = c7;
		cubes[7] = c8;
		cubes[8] = c9;
		cubes[9] = c10;
		cubes[10] = c11;
		cubes[11] = c12;
		cubes[12] = c13;
		cubes[13] = c14;
		cubes[14] = c15;
		cubes[15] = c16;
		cubes[16] = c17;
		cubes[17] = c18;
		cubes[18] = c19;
		cubes[19] = c20;
		cubes[20] = c21;
		cubes[21] = c22;
		cubes[22] = c23;
		cubes[23] = c24;
		cubes[24] = c25;
		cubes[25] = c26;
		cubes[26] = c27;
		//////create group
		allCubes = sgCGroup::CreateGroup(cubes,27);
	}
}

//--------------------------------------------------------------
void cutter::update(){
	if(numCutter==1){
		planes[0] = x1;
		planes[1] = x2;
		planes[2] = y1;
		planes[3] = y2;
		planes[4] = z1;
		planes[5] = z2;
		//////create group
		allPlanes = sgCGroup::CreateGroup(planes,6);
	}else if(numCutter==2){
		cubes[0] = c1;
		cubes[1] = c2;
		cubes[2] = c3;
		cubes[3] = c4;
		cubes[4] = c5;
		cubes[5] = c6;
		cubes[6] = c7;
		cubes[7] = c8;
		cubes[8] = c9;
		cubes[9] = c10;
		cubes[10] = c11;
		cubes[11] = c12;
		cubes[12] = c13;
		cubes[13] = c14;
		cubes[14] = c15;
		cubes[15] = c16;
		cubes[16] = c17;
		cubes[17] = c18;
		cubes[18] = c19;
		cubes[19] = c20;
		cubes[20] = c21;
		cubes[21] = c22;
		cubes[22] = c23;
		cubes[23] = c24;
		cubes[24] = c25;
		cubes[25] = c26;
		cubes[26] = c27;
		//////create group
		allCubes = sgCGroup::CreateGroup(cubes,27);
	}
}

//--------------------------------------------------------------
void cutter::draw(){  
	//allPlanes = sgCGroup::CreateGroup(planes,6);

}
/////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
sgCGroup* cutter::getCutter(){
	if(numCutter==1){
		return allPlanes;
	}else{
		return allCubes;
	}
}