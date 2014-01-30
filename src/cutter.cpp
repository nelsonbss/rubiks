#include "cutter.h"
#include "sgCore.h"

cutter::cutter(float thick, float tamPlane, float tamCuby,float numCutr){
	numCutter = numCutr;
	cutterThick = thick;
	cutterSize = tamPlane;
	tamCubie = tamCuby;
	posX=0;
	posY=0;
	planes = (sgCObject**)malloc(6*sizeof(sgCObject*));  //allocate memory
	cubes = (sgCObject**)malloc(27*sizeof(sgCObject*));  //allocate memory
	///
	centerCube.x = -tamCubie/2;
	centerCube.y = -tamCubie/2;
	centerCube.z = tamCubie/2; 
	infinity = (cutterSize-tamCubie)/2;
}

//--------------------------------------------------------------
void cutter::setup(){
	////////////////////// BOX 1 -> 6////////////////////////////////////
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
	///////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////
	//put boxes/planes on the array of pointer
	//to pass it along and create a group
	//to be able to return it as a group to the main class
	planes[0] = x1;
	planes[1] = x2;
	planes[2] = y1;
	planes[3] = y2;
	planes[4] = z1;
	planes[5] = z2;
	//////create group////////////////////////////////////////////////
	allPlanes = sgCGroup::CreateGroup(planes,6);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes ///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//the "cube" will be constructed by planes. the cube has three planes.
	//the first plane constucted is the middle plane

	//make middle layer
	//fist put the center cubie, it is the only cubic cubie, the other ones expand towards the outside to "infinity"
	c1 = sgCreateBox(tamCubie,tamCubie,tamCubie); //only 7 cubes are equilateral
	c1->InitTempMatrix()->Translate(centerCube);
	c1->ApplyTempMatrix();  
	c1->DestroyTempMatrix();
	c1->SetAttribute(SG_OA_COLOR,0);
	////
	c2 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	c2->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc2 = {tamCubie,0,0};//move to its position
	c2->GetTempMatrix()->Translate(transc2);
	c2->ApplyTempMatrix();  
	c2->DestroyTempMatrix();
	c2->SetAttribute(SG_OA_COLOR,2);
	////
	c3 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z (z is positive, have to handle this with rotations)
	c3->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc3 = {tamCubie,0,0};//move to its position
	c3->GetTempMatrix()->Translate(transc3);
	SG_VECTOR transc31 = {0,0,tamCubie};//move to its position
	c3->GetTempMatrix()->Translate(transc31);
	c3->ApplyTempMatrix();  
	c3->DestroyTempMatrix();
	c3->SetAttribute(SG_OA_COLOR,4);
	////
	c4 = sgCreateBox(tamCubie,tamCubie,infinity); //it expands -> z
	c4->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc4 = {0,0,tamCubie};//move to its position
	c4->GetTempMatrix()->Translate(transc4);
	c4->ApplyTempMatrix();  
	c4->DestroyTempMatrix();
	c4->SetAttribute(SG_OA_COLOR,6);
	////
	c5 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	SG_POINT rot5 = {0,0,0};
	SG_VECTOR rotD5 = {0,-1,0};
	c5->InitTempMatrix()->Rotate(rot5,rotD5, 1.57079633);
	c5->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc5 = {0,0,tamCubie};//move to its position
	c5->GetTempMatrix()->Translate(transc5);
	c5->ApplyTempMatrix();  
	c5->DestroyTempMatrix();
	c5->SetAttribute(SG_OA_COLOR,8);
	////
	c6 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x 
	SG_POINT rot6 = {0,0,0};
	SG_VECTOR rotD6 = {0,-1,0};
	c6->InitTempMatrix()->Rotate(rot6,rotD6, 3.14);
	c6->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc6 = {0,0,tamCubie};//move to its position
	c6->GetTempMatrix()->Translate(transc6);
	c6->ApplyTempMatrix();  
	c6->DestroyTempMatrix();
	c6->SetAttribute(SG_OA_COLOR,10);
	////
	c7 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	SG_POINT rot7 = {0,0,0};
	SG_VECTOR rotD7 = {0,-1,0};
	c7->InitTempMatrix()->Rotate(rot7,rotD7, 3.14);
	c7->GetTempMatrix()->Translate(centerCube);//center box
	c7->ApplyTempMatrix();  
	c7->DestroyTempMatrix();
	c7->SetAttribute(SG_OA_COLOR,12);
	////
	c8 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	SG_POINT rot8 = {0,0,0};
	SG_VECTOR rotD8 = {0,-1,0};
	c8->InitTempMatrix()->Rotate(rot8,rotD8, -1.57079633);
	c8->GetTempMatrix()->Translate(centerCube);//center box
	c8->ApplyTempMatrix();  
	c8->DestroyTempMatrix();
	c8->SetAttribute(SG_OA_COLOR,14);
	////
	c9 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z
	SG_POINT rot9 = {0,0,0};
	SG_VECTOR rotD9 = {0,-1,0};
	c9->InitTempMatrix()->Rotate(rot9,rotD9, -1.57079633);
	c9->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc9 = {tamCubie,0,0};//move to its position
	c9->GetTempMatrix()->Translate(transc9);
	c9->ApplyTempMatrix();  
	c9->DestroyTempMatrix();
	c9->SetAttribute(SG_OA_COLOR,16);
	//////////second layer..on top of the previous one
	c10 = sgCreateBox(tamCubie,tamCubie,tamCubie); //only 7 cubes are equilateral
	c10->InitTempMatrix()->Translate(centerCube);
	SG_VECTOR transc10 = {0,-tamCubie,0};
	c10->GetTempMatrix()->Translate(transc10);
	c10->ApplyTempMatrix();  
	c10->DestroyTempMatrix();
	c10->SetAttribute(SG_OA_COLOR,18);
	////
	c11 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	c11->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc11 = {tamCubie,0,0};//move to its position
	c11->GetTempMatrix()->Translate(transc11);
	SG_VECTOR transc111 = {0,-tamCubie,0};
	c11->GetTempMatrix()->Translate(transc111);
	c11->ApplyTempMatrix();  
	c11->DestroyTempMatrix();
	c11->SetAttribute(SG_OA_COLOR,20);
	////
	c12 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z (z is positive, have to handle this with rotations)
	c12->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc12 = {tamCubie,0,0};//move to its position
	c12->GetTempMatrix()->Translate(transc12);
	SG_VECTOR transc121 = {0,0,tamCubie};//move to its position
	c12->GetTempMatrix()->Translate(transc121);
	SG_VECTOR transc122 = {0,-tamCubie,0};
	c12->GetTempMatrix()->Translate(transc122);
	c12->ApplyTempMatrix();  
	c12->DestroyTempMatrix();
	c12->SetAttribute(SG_OA_COLOR,30);
	////
	c13 = sgCreateBox(tamCubie,tamCubie,infinity); //it expands -> z
	c13->InitTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc13 = {0,0,tamCubie};//move to its position
	c13->GetTempMatrix()->Translate(transc13);
	SG_VECTOR transc131 = {0,-tamCubie,0};
	c13->GetTempMatrix()->Translate(transc131);
	c13->ApplyTempMatrix();  
	c13->DestroyTempMatrix();
	c13->SetAttribute(SG_OA_COLOR,32);
	////
	c14 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	SG_POINT rot14 = {0,0,0};
	SG_VECTOR rotD14 = {0,-1,0};
	c14->InitTempMatrix()->Rotate(rot14,rotD14, 1.57079633);
	c14->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc14 = {0,0,tamCubie};//move to its position
	c14->GetTempMatrix()->Translate(transc14);
	SG_VECTOR transc141 = {0,-tamCubie,0};
	c14->GetTempMatrix()->Translate(transc141);
	c14->ApplyTempMatrix();  
	c14->DestroyTempMatrix();
	c14->SetAttribute(SG_OA_COLOR,34);
	////
	c15 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x 
	SG_POINT rot15 = {0,0,0};
	SG_VECTOR rotD15 = {0,-1,0};
	c15->InitTempMatrix()->Rotate(rot15,rotD15, 3.14);
	c15->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc15 = {0,0,tamCubie};//move to its position
	c15->GetTempMatrix()->Translate(transc15);
	SG_VECTOR transc151 = {0,-tamCubie,0};
	c15->GetTempMatrix()->Translate(transc151);
	c15->ApplyTempMatrix();  
	c15->DestroyTempMatrix();
	c15->SetAttribute(SG_OA_COLOR,36);
	////
	c16 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	SG_POINT rot16 = {0,0,0};
	SG_VECTOR rotD16 = {0,-1,0};
	c16->InitTempMatrix()->Rotate(rot16,rotD16, 3.14);
	c16->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc161 = {0,-tamCubie,0};
	c16->GetTempMatrix()->Translate(transc161);
	c16->ApplyTempMatrix();  
	c16->DestroyTempMatrix();
	c16->SetAttribute(SG_OA_COLOR,38);
	////
	c17 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	SG_POINT rot17 = {0,0,0};
	SG_VECTOR rotD17 = {0,-1,0};
	c17->InitTempMatrix()->Rotate(rot17,rotD17, -1.57079633);
	c17->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc171 = {0,-tamCubie,0};
	c17->GetTempMatrix()->Translate(transc171);
	c17->ApplyTempMatrix();  
	c17->DestroyTempMatrix();
	c17->SetAttribute(SG_OA_COLOR,40);
	////
	c18 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z
	SG_POINT rot18 = {0,0,0};
	SG_VECTOR rotD18 = {0,-1,0};
	c18->InitTempMatrix()->Rotate(rot18,rotD18, -1.57079633);
	c18->GetTempMatrix()->Translate(centerCube);//center box
	SG_VECTOR transc18 = {tamCubie,0,0};//move to its position
	c18->GetTempMatrix()->Translate(transc18);
	SG_VECTOR transc181 = {0,-tamCubie,0};
	c18->GetTempMatrix()->Translate(transc181);
	c18->ApplyTempMatrix();  
	c18->DestroyTempMatrix();
	c18->SetAttribute(SG_OA_COLOR,42);
	////////////////////////////////////////////////////////////////
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
	///
	cubes[9] = c10;
	cubes[10] = c11;
	cubes[11] = c12;
	cubes[12] = c13;
	cubes[13] = c14;
	cubes[14] = c15;
	cubes[15] = c16;
	cubes[16] = c17;
	cubes[17] = c18;
	//////create group
	allCubes = sgCGroup::CreateGroup(cubes,18);
	//translate to be able to see it
	SG_VECTOR center = {400,400,0};
	allCubes->InitTempMatrix()->Translate(center);
	SG_POINT rot = {400,400,0};
	SG_VECTOR rotD = {1,0,0};
	allCubes->GetTempMatrix()->Rotate(rot,rotD,-0.3); 
	allCubes->ApplyTempMatrix();  
	allCubes->DestroyTempMatrix();
}

//--------------------------------------------------------------
void cutter::update(){
	//	planes[0] = x1;
	//	planes[1] = x2;
	//	planes[2] = y1;
	//	planes[3] = y2;
	//	planes[4] = z1;
	//	planes[5] = z2;
	//	//////create group
	allPlanes = sgCGroup::CreateGroup(planes,6);

	//why do I have to do this update... or else allPlanes & allCubes are forgotten and cannot draw ?????????????????????
	//cubes[0] = c1; 
	//////create group
	allCubes = sgCGroup::CreateGroup(cubes,18);

}

//--------------------------------------------------------------
void cutter::draw(){  
	//allPlanes = sgCGroup::CreateGroup(planes,6);

}
/////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
sgCGroup* cutter::getCutterPlanes(){
	return allPlanes;
}

sgCGroup* cutter::getCutterCubes(){
	return allCubes; 
}