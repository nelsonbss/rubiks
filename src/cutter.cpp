#include "cutter.h"
#include "sgCore.h"

cutter::cutter(float thick, float tamCutter, float tamCuby,float numCutr, ofVec3f pos, int gsize){
	numCutter = numCutr; //to use when we have more than one cutter
	//cutterThick = thick; //only used on planes
	///
	cutterSize = tamCutter; //how big is the cutter cube
	///
	tamCubie = tamCuby;
	tamCubie=3*tamCuby/gsize;

	gridSize=gsize;
	if (gsize==1) {
		gridSize=3;
	}
	numPieces=gridSize*gridSize*gridSize;


	//planes = (sgCObject**)malloc(6*sizeof(sgCObject*));
	cubes = (sgCObject**)malloc(numPieces*sizeof(sgCObject*));
	cc = (sgCBox**)malloc(numPieces*sizeof(sgCBox*));   ////clean this memory && use this to build the group
	////
	posCutter.x = pos.x;
	posCutter.y = pos.y;
	posCutter.z = pos.z;
	////
	//rotCutter.x = rot.x;
	//rotCutter.y = rot.y;
	//rotCutter.z = rot.z;
	//armRotH = armRotHi;
	///
	centerCube.x = -tamCubie/2;
	centerCube.y = -tamCubie/2;
	centerCube.z = -tamCubie/2; 
	///
	infinity = (cutterSize-tamCubie)/2;
}
//--------------------------------------------------------------
void cutter::setup(){
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes///make cubes ///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float tamCubieSpread=tamCubie;
	float tamCubieP=tamCubie-2;
	float infinityP=infinity-2;



	// alternate approach:

	float xsize, ysize, zsize;
	float xtrans,ytrans,ztrans;
	float gridCenter=(float(gridSize)-1.0)/2.0;


	int index=0;

	for (int x=0; x<gridSize; x++) {
		xsize=tamCubieP;
		xtrans=(float(x)-gridCenter)*tamCubie;
		if (x==0 || x==gridSize-1) { 
			xsize=infinityP;
		}
		if (x==0) {
			xtrans-=infinity-tamCubie;
		}
		for (int y=0; y<gridSize; y++) {
			ysize=tamCubieP;
			ytrans=(float(y)-gridCenter)*tamCubie;
			if (y==0 || y==gridSize-1) { 
				ysize=infinityP;
			}
			if (y==0) {
				ytrans-=infinity-tamCubie;
			}

			for (int z=0; z<gridSize; z++) {
				zsize=tamCubieP;
				ztrans=(float(z)-gridCenter)*tamCubie;
				if (z==0 || z==gridSize-1) { 
					zsize=infinityP;
				}
				if (z==0) {
					ztrans-=infinity-tamCubie;
				}

				cc[index] = sgCreateBox(xsize,ysize,zsize); //it expands -> x
				cc[index]->InitTempMatrix()->Translate(centerCube);//center box
				//SG_VECTOR transc2 = {xtrans,ytrans,ztrans};//move to its position
				SG_VECTOR transc2 = {xtrans+1,ytrans+1,ztrans+1};//move to its position
				cc[index]->GetTempMatrix()->Translate(transc2);
				cc[index]->ApplyTempMatrix();  
				cc[index]->DestroyTempMatrix();
				//					cc[index]->SetAttribute(SG_OA_COLOR,index*2);

				cubes[index]=cc[index];
				index++;
			}
		}
	}

	//make middle layer
	//fist put the center cubie, it is the only cubic cubie, the other ones expand towards the outside to "infinity"
	//c1 = sgCreateBox(tamCubie,tamCubie,tamCubie); //only 7 cubes are equilateral
	//c1->InitTempMatrix()->Translate(centerCube);
	//c1->ApplyTempMatrix();  
	//c1->DestroyTempMatrix();
	//c1->SetAttribute(SG_OA_COLOR,0);
	//////
	//c2 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	//c2->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc2 = {tamCubie,0,0};//move to its position
	//c2->GetTempMatrix()->Translate(transc2);
	//c2->ApplyTempMatrix();  
	//c2->DestroyTempMatrix();
	//c2->SetAttribute(SG_OA_COLOR,2);
	//////
	//c3 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z (z is positive, have to handle this with rotations)
	//c3->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc3 = {tamCubie,0,0};//move to its positionnm
	//c3->GetTempMatrix()->Translate(transc3);
	//SG_VECTOR transc31 = {0,0,tamCubie};//move to its position
	//c3->GetTempMatrix()->Translate(transc31);
	//c3->ApplyTempMatrix();  
	//c3->DestroyTempMatrix();
	//c3->SetAttribute(SG_OA_COLOR,4);
	//////
	//c4 = sgCreateBox(tamCubie,tamCubie,infinity); //it expands -> z
	//c4->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc4 = {0,0,tamCubie};//move to its position
	//c4->GetTempMatrix()->Translate(transc4);
	//c4->ApplyTempMatrix();  
	//c4->DestroyTempMatrix();
	//c4->SetAttribute(SG_OA_COLOR,6);
	//////
	//c5 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	//SG_POINT rot5 = {0,0,0};
	//SG_VECTOR rotD5 = {0,-1,0};
	//c5->InitTempMatrix()->Rotate(rot5,rotD5, 1.57079633);
	//c5->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc5 = {0,0,tamCubie};//move to its position
	//c5->GetTempMatrix()->Translate(transc5);
	//c5->ApplyTempMatrix();  
	//c5->DestroyTempMatrix();
	//c5->SetAttribute(SG_OA_COLOR,8);
	//////
	//c6 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x 
	//SG_POINT rot6 = {0,0,0};
	//SG_VECTOR rotD6 = {0,-1,0};
	//c6->InitTempMatrix()->Rotate(rot6,rotD6, 3.14);
	//c6->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc6 = {0,0,tamCubie};//move to its position
	//c6->GetTempMatrix()->Translate(transc6);
	//c6->ApplyTempMatrix();  
	//c6->DestroyTempMatrix();
	//c6->SetAttribute(SG_OA_COLOR,10);
	//////
	//c7 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z 
	//SG_POINT rot7 = {0,0,0};
	//SG_VECTOR rotD7 = {0,-1,0};
	//c7->InitTempMatrix()->Rotate(rot7,rotD7, 3.14);
	//c7->GetTempMatrix()->Translate(centerCube);//center box
	//c7->ApplyTempMatrix();  
	//c7->DestroyTempMatrix();
	//c7->SetAttribute(SG_OA_COLOR,12);
	//////
	//c8 = sgCreateBox(infinity,tamCubie,tamCubie); //it expands -> x
	//SG_POINT rot8 = {0,0,0};
	//SG_VECTOR rotD8 = {0,-1,0};
	//c8->InitTempMatrix()->Rotate(rot8,rotD8, -1.57079633);
	//c8->GetTempMatrix()->Translate(centerCube);//center box
	//c8->ApplyTempMatrix();  
	//c8->DestroyTempMatrix();
	//c8->SetAttribute(SG_OA_COLOR,14);
	//////
	//c9 = sgCreateBox(infinity,tamCubie,infinity); //it expands -> x & -> z
	//SG_POINT rot9 = {0,0,0};
	//SG_VECTOR rotD9 = {0,-1,0};
	//c9->InitTempMatrix()->Rotate(rot9,rotD9, -1.57079633);
	//c9->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc9 = {tamCubie,0,0};//move to its position
	//c9->GetTempMatrix()->Translate(transc9);
	//c9->ApplyTempMatrix();  
	//c9->DestroyTempMatrix();
	//c9->SetAttribute(SG_OA_COLOR,16);
	////////////second layer..on top of the previous one
	////this layer has to expand on -> -y to "infitiy"
	//c10 = sgCreateBox(tamCubie,infinity,tamCubie); //only 7 cubes are equilateral
	//c10->InitTempMatrix()->Translate(centerCube);
	//SG_VECTOR transc10 = {0,-infinity,0};
	//c10->GetTempMatrix()->Translate(transc10);
	//c10->ApplyTempMatrix();  
	//c10->DestroyTempMatrix();
	//c10->SetAttribute(SG_OA_COLOR,18);
	//////
	//c11 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x
	//c11->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc11 = {tamCubie,0,0};//move to its position
	//c11->GetTempMatrix()->Translate(transc11);
	//SG_VECTOR transc111 = {0,-infinity,0};
	//c11->GetTempMatrix()->Translate(transc111);
	//c11->ApplyTempMatrix();  
	//c11->DestroyTempMatrix();
	//c11->SetAttribute(SG_OA_COLOR,20);
	//////
	//c12 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z (z is positive, have to handle this with rotations)
	//c12->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc12 = {tamCubie,0,0};//move to its position
	//c12->GetTempMatrix()->Translate(transc12);
	//SG_VECTOR transc121 = {0,0,tamCubie};//move to its position
	//c12->GetTempMatrix()->Translate(transc121);
	//SG_VECTOR transc122 = {0,-infinity,0};
	//c12->GetTempMatrix()->Translate(transc122);
	//c12->ApplyTempMatrix();  
	//c12->DestroyTempMatrix();
	//c12->SetAttribute(SG_OA_COLOR,30);
	//////
	//c13 = sgCreateBox(tamCubie,infinity,infinity); //it expands -> z
	//c13->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc13 = {0,0,tamCubie};//move to its position
	//c13->GetTempMatrix()->Translate(transc13);
	//SG_VECTOR transc131 = {0,-infinity,0};
	//c13->GetTempMatrix()->Translate(transc131);
	//c13->ApplyTempMatrix();  
	//c13->DestroyTempMatrix();
	//c13->SetAttribute(SG_OA_COLOR,32);
	//////
	//c14 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z 
	//SG_POINT rot14 = {0,0,0};
	//SG_VECTOR rotD14 = {0,-1,0};
	//c14->InitTempMatrix()->Rotate(rot14,rotD14, 1.57079633);
	//c14->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc14 = {0,0,tamCubie};//move to its position
	//c14->GetTempMatrix()->Translate(transc14);
	//SG_VECTOR transc141 = {0,-infinity,0};
	//c14->GetTempMatrix()->Translate(transc141);
	//c14->ApplyTempMatrix();  
	//c14->DestroyTempMatrix();
	//c14->SetAttribute(SG_OA_COLOR,34);
	//////
	//c15 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x 
	//SG_POINT rot15 = {0,0,0};
	//SG_VECTOR rotD15 = {0,-1,0};
	//c15->InitTempMatrix()->Rotate(rot15,rotD15, 3.14);
	//c15->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc15 = {0,0,tamCubie};//move to its position
	//c15->GetTempMatrix()->Translate(transc15);
	//SG_VECTOR transc151 = {0,-infinity,0};
	//c15->GetTempMatrix()->Translate(transc151);
	//c15->ApplyTempMatrix();  
	//c15->DestroyTempMatrix();
	//c15->SetAttribute(SG_OA_COLOR,36);
	//////
	//c16 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z 
	//SG_POINT rot16 = {0,0,0};
	//SG_VECTOR rotD16 = {0,-1,0};
	//c16->InitTempMatrix()->Rotate(rot16,rotD16, 3.14);
	//c16->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc161 = {0,-infinity,0};
	//c16->GetTempMatrix()->Translate(transc161);
	//c16->ApplyTempMatrix();  
	//c16->DestroyTempMatrix();
	//c16->SetAttribute(SG_OA_COLOR,38);
	//////
	//c17 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x
	//SG_POINT rot17 = {0,0,0};
	//SG_VECTOR rotD17 = {0,-1,0};
	//c17->InitTempMatrix()->Rotate(rot17,rotD17, -1.57079633);
	//c17->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc171 = {0,-infinity,0};
	//c17->GetTempMatrix()->Translate(transc171);
	//c17->ApplyTempMatrix();  
	//c17->DestroyTempMatrix();
	//c17->SetAttribute(SG_OA_COLOR,40);
	//////
	//c18 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z
	//SG_POINT rot18 = {0,0,0};
	//SG_VECTOR rotD18 = {0,-1,0};
	//c18->InitTempMatrix()->Rotate(rot18,rotD18, -1.57079633);
	//c18->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc18 = {tamCubie,0,0};//move to its position
	//c18->GetTempMatrix()->Translate(transc18);
	//SG_VECTOR transc181 = {0,-infinity,0};
	//c18->GetTempMatrix()->Translate(transc181);
	//c18->ApplyTempMatrix();  
	//c18->DestroyTempMatrix();
	//c18->SetAttribute(SG_OA_COLOR,42);
	////////////third layer..on botom of the first one
	////this layer has to expand on -> y to "infitiy"
	//c19 = sgCreateBox(tamCubie,infinity,tamCubie); //only 7 cubes are equilateral
	//c19->InitTempMatrix()->Translate(centerCube);
	//SG_VECTOR transc19 = {0,tamCubie,0};
	//c19->GetTempMatrix()->Translate(transc19);
	//c19->ApplyTempMatrix();  
	//c19->DestroyTempMatrix();
	//c19->SetAttribute(SG_OA_COLOR,44);
	//////
	//c20 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x
	//c20->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc20 = {tamCubie,0,0};//move to its position
	//c20->GetTempMatrix()->Translate(transc20);
	//SG_VECTOR transc201 = {0,tamCubie,0};
	//c20->GetTempMatrix()->Translate(transc201);
	//c20->ApplyTempMatrix();  
	//c20->DestroyTempMatrix();
	//c20->SetAttribute(SG_OA_COLOR,46);
	//////
	//c21 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z (z is positive, have to handle this with rotations)
	//c21->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc21 = {tamCubie,0,0};//move to its position
	//c21->GetTempMatrix()->Translate(transc21);
	//SG_VECTOR transc211 = {0,0,tamCubie};//move to its position
	//c21->GetTempMatrix()->Translate(transc211);
	//SG_VECTOR transc212 = {0,tamCubie,0};
	//c21->GetTempMatrix()->Translate(transc212);
	//c21->ApplyTempMatrix();  
	//c21->DestroyTempMatrix();
	//c21->SetAttribute(SG_OA_COLOR,48);
	//////
	//c22 = sgCreateBox(tamCubie,infinity,infinity); //it expands -> z
	//c22->InitTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc22 = {0,0,tamCubie};//move to its position
	//c22->GetTempMatrix()->Translate(transc22);
	//SG_VECTOR transc221 = {0,tamCubie,0};
	//c22->GetTempMatrix()->Translate(transc221);
	//c22->ApplyTempMatrix();  
	//c22->DestroyTempMatrix();
	//c22->SetAttribute(SG_OA_COLOR,50);
	//////
	//c23 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z 
	//SG_POINT rot23 = {0,0,0};
	//SG_VECTOR rotD23 = {0,-1,0};
	//c23->InitTempMatrix()->Rotate(rot23,rotD23, 1.57079633);
	//c23->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc23 = {0,0,tamCubie};//move to its position
	//c23->GetTempMatrix()->Translate(transc23);
	//SG_VECTOR transc231 = {0,tamCubie,0};
	//c23->GetTempMatrix()->Translate(transc231);
	//c23->ApplyTempMatrix();  
	//c23->DestroyTempMatrix();
	//c23->SetAttribute(SG_OA_COLOR,52);
	//////
	//c24 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x 
	//SG_POINT rot24 = {0,0,0};
	//SG_VECTOR rotD24 = {0,-1,0};
	//c24->InitTempMatrix()->Rotate(rot24,rotD24, 3.14);
	//c24->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc24 = {0,0,tamCubie};//move to its position
	//c24->GetTempMatrix()->Translate(transc24);
	//SG_VECTOR transc241 = {0,tamCubie,0};
	//c24->GetTempMatrix()->Translate(transc241);
	//c24->ApplyTempMatrix();  
	//c24->DestroyTempMatrix();
	//c24->SetAttribute(SG_OA_COLOR,54);
	//////
	//c25 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z 
	//SG_POINT rot25 = {0,0,0};
	//SG_VECTOR rotD25 = {0,-1,0};
	//c25->InitTempMatrix()->Rotate(rot25,rotD25, 3.14);
	//c25->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc251 = {0,tamCubie,0};
	//c25->GetTempMatrix()->Translate(transc251);
	//c25->ApplyTempMatrix();  
	//c25->DestroyTempMatrix();
	//c25->SetAttribute(SG_OA_COLOR,56);
	//////
	//c26 = sgCreateBox(infinity,infinity,tamCubie); //it expands -> x
	//SG_POINT rot26 = {0,0,0};
	//SG_VECTOR rotD26 = {0,-1,0};
	//c26->InitTempMatrix()->Rotate(rot26,rotD26, -1.57079633);
	//c26->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc261 = {0,tamCubie,0};
	//c26->GetTempMatrix()->Translate(transc261);
	//c26->ApplyTempMatrix();  
	//c26->DestroyTempMatrix();
	//c26->SetAttribute(SG_OA_COLOR,58);
	//////
	//c27 = sgCreateBox(infinity,infinity,infinity); //it expands -> x & -> z
	//SG_POINT rot27 = {0,0,0};
	//SG_VECTOR rotD27 = {0,-1,0};
	//c27->InitTempMatrix()->Rotate(rot27,rotD27, -1.57079633);
	//c27->GetTempMatrix()->Translate(centerCube);//center box
	//SG_VECTOR transc27 = {tamCubie,0,0};//move to its position
	//c27->GetTempMatrix()->Translate(transc27);
	//SG_VECTOR transc271 = {0,tamCubie,0};
	//c27->GetTempMatrix()->Translate(transc271);
	//c27->ApplyTempMatrix();  
	//c27->DestroyTempMatrix();
	//c27->SetAttribute(SG_OA_COLOR,60);
	//////////////////////////////////////////////////////////////////
	/////put cubes in [] to make group
	//cubes[0] = c1;
	//cubes[1] = c2;
	//cubes[2] = c3;
	//cubes[3] = c4;
	//cubes[4] = c5;
	//cubes[5] = c6;
	//cubes[6] = c7;
	//cubes[7] = c8;
	//cubes[8] = c9;
	/////
	//cubes[9] = c10;
	//cubes[10] = c11;
	//cubes[11] = c12;
	//cubes[12] = c13;
	//cubes[13] = c14;
	//cubes[14] = c15;
	//cubes[15] = c16;
	//cubes[16] = c17;
	//cubes[17] = c18;
	/////
	//cubes[18] = c19;
	//cubes[19] = c20;
	//cubes[20] = c21;
	//cubes[21] = c22;
	//cubes[22] = c23;
	//cubes[23] = c24;
	//cubes[24] = c25;
	//cubes[25] = c26;
	//cubes[26] = c27;
	//////create group
	allCubes = sgCGroup::CreateGroup(cubes,numPieces);
	//move cubes where cuts are going to be made
	SG_VECTOR transCubies = {posCutter.x,posCutter.y,posCutter.z};
	allCubes->InitTempMatrix()->Translate(transCubies);
	////rotating the cutter
	///////////////////////////////////////works for only one rotation
	//SG_POINT rot = {posCutter.x,posCutter.y,posCutter.z};
	//SG_VECTOR rotX = {1,0,0};
	//allCubes->GetTempMatrix()->Rotate(rot,rotX,ofDegToRad(rotCutter.x));
	//SG_VECTOR rotY = {0,1,0};
	//allCubes->GetTempMatrix()->Rotate(rot,rotY,ofDegToRad(rotCutter.y));
	//SG_VECTOR rotZ = {0,0,1};
	//allCubes->GetTempMatrix()->Rotate(rot,rotZ,ofDegToRad(rotCutter.z));

	//using history of rotations to have more than one
	//the order of the rotation doesnt matter

	allCubes->ApplyTempMatrix();  
	//allCubes->DestroyTempMatrix();
}

//--------------------------------------------------------------
void cutter::update(){
}
//--------------------------------------------------------------
void cutter::draw(){
	//ofSetColor(ofColor(255,255,0,150));
	//ofPushMatrix();
	//if (allCubes->GetTempMatrix()!=0)
	//	glMultMatrixd(allCubes->GetTempMatrix()->GetTransparentData());
	////allCubes->DestroyTempMatrix();
	//ofPopMatrix();
}
//-------------------------------------------------------------------
void cutter::exit(){
	/*sgCObject** allParts = (sgCObject**)malloc(6*sizeof(sgCObject*));
	allPlanes->BreakGroup(allParts);
	sgCObject::DeleteObject(allPlanes);
	for (int j=0; j < 6; j++){
	sgDeleteObject(allParts[j]);
	}
	free(allParts);*/

	sgCObject** allParts2 = (sgCObject**)malloc(numPieces*sizeof(sgCObject*));
	allCubes->BreakGroup(allParts2);
	sgCObject::DeleteObject(allCubes);
	for (int j=0; j < numPieces; j++){
		sgDeleteObject(allParts2[j]);
	}
	free(allParts2);
	//free(planes);
	free(cubes);
}









/////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
//sgCGroup* cutter::getCutterPlanes(){
//	return allPlanes;
//}
//
//sgCGroup* cutter::getCutterCubes(){
//	return allCubes; 
//}
//=------------------------------------------------------------------------



//////////////////////// planes 1 -> 6////////////////////////////////////
//x1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////move plane to the middle of its side
//SG_VECTOR transBox1 = {0,-cutterSize/2,0}; 
//x1->InitTempMatrix()->Translate(transBox1);
////move plane to the LEFT edge of the tamPiece
//transPlaneX1.x =-tamCubie/2;
//transPlaneX1.y = 0;
//transPlaneX1.z = 0;
//x1->GetTempMatrix()->Translate(transPlaneX1);
////will be rotated to be parallel to x planes  
//SG_POINT rotCen1 = {-tamCubie/2,0,0};
////it is orignally parallel to z planes
////rotate it around y axis 90deg= 1.57079633 rad
//SG_VECTOR rotDir1 = {0,1,0};
//x1->GetTempMatrix()->Rotate(rotCen1,rotDir1, 1.57079633);
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox11 = {0,0,cutterSize/2}; //move towards viwer z 
//x1->GetTempMatrix()->Translate(transBox11);
////////
//x1->ApplyTempMatrix();  
//x1->DestroyTempMatrix();
//x1->SetAttribute(SG_OA_COLOR,8);
/////////////////////////////////////////////////////////////////////
//x2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////move plane to the middle of its side
//SG_VECTOR transBox2 = {0,-cutterSize/2,0}; 
//x2->InitTempMatrix()->Translate(transBox2);
////move plane to the RIGHT edge of the tamPiece: 
//transPlaneX2.x = tamCubie/2;
//transPlaneX2.y = 0;
//transPlaneX2.z = 0;
//x2->GetTempMatrix()->Translate(transPlaneX2);
////will be rotated to be parallel to x planes
//SG_POINT rotCen2 = {tamCubie/2,0,0};
////it is orignally parallel to z planes
////rotate it around y axis 90deg= 1.57079633 rad
//SG_VECTOR rotDir2 = {0,1,0};
//x2->GetTempMatrix()->Rotate(rotCen2,rotDir2, 1.57079633); 
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox22 = {0,0,cutterSize/2}; //move towards viwer z 
//x2->GetTempMatrix()->Translate(transBox22);
////////
//x2->ApplyTempMatrix();  
//x2->DestroyTempMatrix();
//x2->SetAttribute(SG_OA_COLOR,9);
//////////////////////////////////////////////////////////////////////
//y1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////move plane to the middle of its side
//SG_VECTOR transBox3 = {-cutterSize/2,0,0}; 
//y1->InitTempMatrix()->Translate(transBox3);
////move plane to the UPPER edge of the tamPiece
//transPlaneY1.x = 0;
//transPlaneY1.y = -tamCubie/2;
//transPlaneY1.z = 0;
//y1->GetTempMatrix()->Translate(transPlaneY1);
////will be rotated to be parallel to y planes
//SG_POINT rotCen3 = {0,-tamCubie/2,0};
//////it is orignally parallel to z planes
//////rotate it around x axis -90deg= -1.57079633 rad
//SG_VECTOR rotDir3 = {1,0,0};
//y1->GetTempMatrix()->Rotate(rotCen3,rotDir3, -1.57079633); 
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox33 = {0,0,cutterSize/2}; //move towards viwer z 
//y1->GetTempMatrix()->Translate(transBox33);
////////
//y1->ApplyTempMatrix();  
//y1->DestroyTempMatrix();
//y1->SetAttribute(SG_OA_COLOR,10);
///////////////////////////////////////////////////////////////////
//y2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////move plane to the middle of its side
//SG_VECTOR transBox4 = {-cutterSize/2,0,0}; 
//y2->InitTempMatrix()->Translate(transBox4);
////move plane to the UPPER edge of the tamPiece 
//transPlaneY2.x = 0;
//transPlaneY2.y = tamCubie/2;
//transPlaneY2.z = 0;
//y2->GetTempMatrix()->Translate(transPlaneY2);
////will be rotated to be parallel to y planes
//SG_POINT rotCen4 = {0,tamCubie/2,0};
//////it is orignally parallel to z planes
//////rotate it around x axis -90deg= -1.57079633 rad
//SG_VECTOR rotDir4 = {1,0,0};
//y2->GetTempMatrix()->Rotate(rotCen4,rotDir4, -1.57079633); 
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox44 = {0,0,cutterSize/2}; //move towards viwer z 
//y2->GetTempMatrix()->Translate(transBox44);
////////
//y2->ApplyTempMatrix();  
//y2->DestroyTempMatrix();
//y2->SetAttribute(SG_OA_COLOR,11);
///////////////////////////////////////////////////////////////////
//z1 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////will not be rotated
////move plane so that its center is on 0,0
////move on x axis
//SG_VECTOR transBox5 = {-cutterSize/2,0,0}; 
//z1->InitTempMatrix()->Translate(transBox5);
//////move on y axis
//SG_VECTOR transBox55 = {0,-cutterSize/2,0};
//z1->GetTempMatrix()->Translate(transBox55);
//////move down (inside of the screen) on z axis
//transPlaneZ1.x = 0;
//transPlaneZ1.y = 0;
//transPlaneZ1.z = -((cutterSize/2)-(tamCubie/2));
//z1->GetTempMatrix()->Translate(transPlaneZ1);
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox55a = {0,0,cutterSize/2}; //move towards viwer z 
//z1->GetTempMatrix()->Translate(transBox55a);
////update z plane position
//transPlaneZ1.z = transPlaneZ1.z + (cutterSize/2);
//////////
//z1->ApplyTempMatrix();
//z1->DestroyTempMatrix();
//z1->SetAttribute(SG_OA_COLOR,12);
//////////////////////////////////////////////////////////////////
//z2 = sgCreateBox(cutterSize,cutterSize,cutterThick); 
////will not be rotated
////move plane so that its center is on 0,0
////move on x axis
//SG_VECTOR transBox6 = {-cutterSize/2,0,0}; 
//z2->InitTempMatrix()->Translate(transBox6);
//////move on y axis
//SG_VECTOR transBox66 = {0,-cutterSize/2,0};
//z2->GetTempMatrix()->Translate(transBox66);
//////move down (inside of the screen) on z axis
//transPlaneZ2.x = 0;
//transPlaneZ2.y = 0;
//transPlaneZ2.z = -((cutterSize/2)+(tamCubie/2));
//z2->GetTempMatrix()->Translate(transPlaneZ2);
////translate plane 1.5Tamcubie on each axix,so that the center of the cutter is on the origin
////remember -z is towards inside the screen
//SG_VECTOR transBox66a = {0,0,cutterSize/2}; //move towards viwer z 
//z2->GetTempMatrix()->Translate(transBox66a);
////update z plane position
//transPlaneZ2.z = transPlaneZ2.z + (cutterSize/2);
//////////
//z2->ApplyTempMatrix();  
//z2->DestroyTempMatrix();
//z2->SetAttribute(SG_OA_COLOR,13);
//////////////////////////////////////////////////////////////////
////put boxes/planes on the array of pointer
////to pass it along and create a group
////to be able to return it as a group to the main class
//planes[0] = x1;
//planes[1] = x2;
//planes[2] = y1;
//planes[3] = y2;
//planes[4] = z1;
//planes[5] = z2;
////////create group////////////////////////////////////////////////
//allPlanes = sgCGroup::CreateGroup(planes,6);
////move planes to display position, where cuts are going to be made
//SG_VECTOR transPlanes = {posCutter.x,posCutter.y,posCutter.z};
//allPlanes->InitTempMatrix()->Translate(transPlanes);
//allPlanes->ApplyTempMatrix();  
//allPlanes->DestroyTempMatrix();