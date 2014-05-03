#include "ofRender.h"
#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include <vector>


ofRender::ofRender(){
	float r;
	float g;
	float b;

	r = (float)113 / (float)255;
	g = (float)126 / (float)255;
	b = (float)145 / (float)255;
	mate = ofFloatColor( r,g,b,0.8); //select obj color 113,126,145
	matePuzzle = ofFloatColor( r,g,b,0.9); //select obj color 113,126,145
	mateSolid = ofFloatColor(150,150,150,255);

	r = (float)236 / (float)255;
	g = (float)33 / (float)255;
	b = (float)36 / (float)255;
	red = ofFloatColor(r,g,b);
	r = (float)7 / (float)255;
	g = (float)122 / (float)255;
	b = (float)189 / (float)255;
	blue = ofFloatColor(r,g,b);
	r = (float)247/ (float)255;
	g = (float)151 / (float)255;
	b = (float)31 / (float)255;
	orange = ofFloatColor(r,g,b);
	white = ofFloatColor(1,1,1);
	r = (float)1/ (float)255;
	g = (float)172 / (float)255;
	b = (float)104/ (float)255;
	green = ofFloatColor(r,g,b);
	r = (float)250/ (float)255;
	g = (float)207 / (float)255;
	b = (float)6 / (float)255;
	yellow = ofFloatColor(r,g,b);
	r = (float)230/ (float)255;
	g = (float)109 / (float)255;
	b = (float)248 / (float)255;
	pink = ofFloatColor(r,g,b);
	r = (float)132/ (float)255;
	g = (float)45 / (float)255;
	b = (float)235 / (float)255;
	purple = ofFloatColor(r,g,b);
	r = (float)147/ (float)255;
	g = (float)209 / (float)255;
	b = (float)12 / (float)255;
	lightGreen = ofFloatColor(r,g,b);
	r = (float)74/ (float)255;
	g = (float)43 / (float)255;
	b = (float)38 / (float)255;
	brown = ofFloatColor(r,g,b);
	r = (float)152/ (float)255;
	gray = ofFloatColor(r,r,r);
	r = (float)213/ (float)255;
	g = (float)226 / (float)255;
	b = (float)24 / (float)255;
	yellowGreen = ofFloatColor(r,g,b);
	r = (float)39/ (float)255;
	g = (float)170 / (float)255;
	b = (float)190 / (float)255;
	aqua = ofFloatColor(r,g,b);
	r = (float)72/ (float)255;
	g = (float)71 / (float)255;
	b = (float)201 / (float)255;
	indigo = ofFloatColor(r,g,b);
	r = (float)244/ (float)255;
	g = (float)94 / (float)255;
	b = (float)33 / (float)255;
	redOrange = ofFloatColor(244,94,33);

	black = ofFloatColor(0,0,0);

	colorsVector.push_back(red);//0
	colorsVector.push_back(blue);//1
	colorsVector.push_back(orange);//2
	colorsVector.push_back(white);//3
	colorsVector.push_back(green);//4  
	colorsVector.push_back(yellow);//5
	colorsVector.push_back(pink);//6 violet
	colorsVector.push_back(purple);//7 medium purple
	colorsVector.push_back(lightGreen);//8
	colorsVector.push_back(brown);//9
	colorsVector.push_back(gray);//10
	colorsVector.push_back(yellowGreen);//11
	colorsVector.push_back(aqua);//12
	colorsVector.push_back(yellow);//13  
	colorsVector.push_back(indigo);//14
	colorsVector.push_back(redOrange);//15
}
//-----------------------------------------------------------------------------------------------------------------------
void ofRender::sgCoretoOFmesh(sgC3DObject *obj, CubieMesh&mesh,int idCubie,int selectedObjectID){
	vector <ofFloatColor> colorsVector2;
	//convert to of mesh and draw as of
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	int numTriangles =0;
	const SG_POINT *vertex;
	const SG_VECTOR *normals;
	//vector< ofVec3f > vert;
	const SG_ALL_TRIANGLES* trngls = obj->GetTriangles();
	numTriangles = trngls->nTr;
	vertex = trngls->allVertex;
	normals = trngls->allNormals;

	//generate ofMesh
	// build vector of vertices from allthe vertices

	for (int i=0; i<(numTriangles*3);i+=3){ //each triangle has three vertices
		ofVec3f auxV1;
		ofVec3f auxV2;
		ofVec3f auxV3;

		auxV1.x = vertex[i].x;
		auxV1.y = vertex[i].y;
		auxV1.z =vertex[i].z;
		//vert.push_back(aux);
		//
		auxV2.x = vertex[i+1].x;
		auxV2.y = vertex[i+1].y;
		auxV2.z =vertex[i+1].z;
		//
		auxV3.x = vertex[i+2].x;
		auxV3.y = vertex[i+2].y;
		auxV3.z =vertex[i+2].z;
		//
		mesh.addVertex(auxV1);
		mesh.addVertex(auxV2);
		mesh.addVertex(auxV3);

		//make indices & add normals to each index
		mesh.addIndex(i);
		mesh.addNormal(ofVec3f((normals[i].x)*1,(normals[i].y)*1,(normals[i].z)*1));

		mesh.addIndex(i + 1);
		mesh.addNormal(ofVec3f((normals[i+1].x)*1,(normals[i+1].y)*1,(normals[i+1].z)*1));

		mesh.addIndex(i + 2);
		mesh.addNormal(ofVec3f((normals[i+2].x)*1,(normals[i+2].y)*1,(normals[i+2].z)*1));

		//look at normal
		////Compute the triangle's normal
		ofVec3f dir = ( (auxV2 - auxV1).crossed( auxV3 - auxV1 ) ).normalized();
		//ofVec3f dir;
		//dir.x = normals[i].x;
		//dir.y = normals[i].y;
		//dir.z = normals[i].z;
		//set colors
		ofFloatColor c;


		//ask if we color an original object with rubiks colors or plain color:: current selection is -2 for plain color
		if(idCubie == -1){
			//original shape with color...use this for pyramid 
			//c = redOrange;//decideColor(decideAxisRange(dir,0));
		}else if (idCubie == -2){
			//its an original shape  with plain color
			c = mate;//colorsVector2[0];//mate
		}else if (idCubie == -3){
			//plain color, used for bunny
			//c = redOrange;
		}else{
			//its another object
			//we only want to color PLAIN the sample object
			//the face coloring will be done separately

			//if(selectedObjectID >= 8){
			//c = decideColorCubieBox(dir,idCubie); //we color the cube here so it gets the official colors on its faces
			c = matePuzzle; //decideColorCubieDEBUG(dir,idCubie); //this is to test the offset slicing and rotations
			//}else if(selectedObjectID == 3){
			//	//cone(ish)
			//	c = decideColor(decideAxisRange(dir,10.0));
			//}
		}

		colorsVector2.push_back(c);
		colorsVector2.push_back(c);
		colorsVector2.push_back(c);
		mesh.addTriangle(i, i+1, i+2, auxV1, auxV2, auxV3, dir, c);
	}
	mesh.addColors(colorsVector2);
	//ofFloatColor * col = mesh.getColorsPointer();
	////setup indices
	//mesh.setupIndicesAuto();
	////set normals.. for lighting 
	//setNormals(mesh);//before normals were done at the end, now they are done for each created triangle, so we can color it according to the normal
}
//---------------------------------------------------------------------------------------------------------------
//void ofRender::changeColorToColor(ofFloatColor Sc, ofFloatColor Tc, ofMesh &mesh){
//	//it recieves a target color tolook for
//	//a target color to change to
//	//and the mesh in which to look for it
//	vector <ofFloatColor> colorsVectorT;
//	colorsVectorT = mesh.getColors();
//	//looks on the colorvector for this color
//	for(int i=0; i< colorsVectorT.size(); i++){
//		ofFloatColor c = colorsVectorT[i];
//		if((c.r == Sc.r) && (c.g == Sc.g)  && (c.b == Sc.b)){
//			//when it finds it->replace it for the incoming color
//			colorsVectorT[i] = Tc;
//		}
//	}
//	//replace the colors vector on the mesh
//	mesh.clearColors();
//	mesh.addColors(colorsVectorT);
//}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFaces(cubie **myCubies, int numPieces, float playRoom, int objectID){
	//goes through each cubie and makes sets of normals.. to determine all different normals in the object
	//i.e. this will give 8 + 6 faces for octahedor
	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;
	//create sets of distitnct normals from all the meshes of all the cubies of the puzzle
	vector< ofVec3f > uniqueNormals;

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	if(objectID < 8){//(objectID != 1 && objectID != 4){

		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			armX = myCubies[i]->armRotations.x;
			armY = myCubies[i]->armRotations.y;
			armZ = myCubies[i]->armRotations.z;
			for (int j = 0 ; j< meshesCubie; j++){
				//get the normals of the mesh!
				tnormals = myCubies[i]->myMeshs[j].getNormals();
				//verify each normal value on unique normals vector
				for(int n=0; n< tnormals.size() ; n++){
					if(uniqueNormals.size() == 0){
						//the first normal of all the normals
						//push the first one
						uniqueNormals.push_back (tnormals[n]);
					}else{
						//it has at least one normal
						//compare current normal with all other normals
						for(int un = 0; un < uniqueNormals.size(); un ++){

							if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
								(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
								((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
								(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
								((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
								(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
								){
									//we already have that type of normal
									//stop looking through the vector
									un = uniqueNormals.size();
							}else{
								//its different
								//keep going until the last element
								if(un == uniqueNormals.size()-1){
									//its the las element on the vector of unique normals
									//if we got here its because the current normal (cn) is new to the set
									uniqueNormals.push_back(tnormals[n]);
									//stop this for.. we just changed the size
									un = uniqueNormals.size();
								}
							}
						}
					}
				}
			}
		}
		//at this point we have the unique normals of the object
		//each of these normals should have a unique color
		vector< ofFloatColor > uniqueColors;
		//build roster of colors for the current object
		for(int i =0; i< uniqueNormals.size(); i++){
			//for each unique normal
			//for now we select from 9 possible colors that we have right now
			ofFloatColor x =  colorsVector[i%16];
			uniqueColors.push_back(x);
		}
		//now -> uniqueColors.size = uniqueNormals.size


		//got through each cubie again
		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			for (int j = 0 ; j< meshesCubie; j++){
				//go through each cubies meshes again
				tnormals = myCubies[i]->myMeshs[j].getNormals();
				tcolors = myCubies[i]->myMeshs[j].getColors();
				//compare this normals to the uniqueNormals(index) to get the color from that uniqueColors(index)
				//go through uniqueNormals
				for(int un = 0; un<uniqueNormals.size();un++){
					//compare each t normal with each unique normal
					for(int n=0; n< tnormals.size() ; n++){
						if(armX==0 && armY==0 && armZ == 0){
							//if there are no arm rotations.. this works for cube official colors
							if(objectID == 1){
								//have to use the official colors
								///rotate normal vectors to compensate for armature rotations z-y-x
								//ask direction to color faces of cube 
								if(tnormals[n].align(x, 2.0)){
									tcolors[n] = blue; 
								}else if(tnormals[n].align(y, 2.0)){
									tcolors[n] = orange; 
								}else if(tnormals[n].align(z, 2.0)){
									tcolors[n] = yellow;
								}else if(tnormals[n].align(xn, 2.0)){
									tcolors[n] = green; 
								}else if(tnormals[n].align(yn, 2.0)){
									tcolors[n] = red; 
								}else if(tnormals[n].align(zn, 2.0)){
									tcolors[n] = white;
								}
							}else {
								if (((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && (tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
									((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && (tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
									((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && (tnormals[n].z <= (uniqueNormals[un].z + playRoom))
									){
										//if the cubies meshs normal is one of the unique normals
										//we assign a color to that normal on the cubie
										//the index of the tnormal that we are looking at, is the same on the tcolors vector
										//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
										tcolors[n] = uniqueColors[un];
								}
							}
						}else{
							//arm rotations
							if(objectID == 1){
								//have to use the official colors
								///rotate normal vectors to compensate for armature rotations z-y-x
								//ask direction to color faces of cube 
								ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
								ofVec3f t2 = t.getRotated(armY,ofVec3f(0,1,0));
								ofVec3f t3 = t2.getRotated(armX,ofVec3f(1,0,0));

								if(t3.align(x, 2.0)){
									tcolors[n] = blue; 
								}else if(t3.align(y, 2.0)){
									tcolors[n] = orange; 
								}else if(t3.align(z, 2.0)){
									tcolors[n] = yellow;
								}else if(t3.align(xn, 2.0)){
									tcolors[n] = green; 
								}else if(t3.align(yn, 2.0)){
									tcolors[n] = red; 
								}else if(t3.align(zn, 2.0)){
									tcolors[n] = white;
								}
							}
							else{
								//another object, do as if there where no arm rotations
								if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
									(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
									((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
									(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
									((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
									(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
									){
										//if the cubies meshs normal is one of the unique normals
										//we assign a color to that normal on the cubie
										//the index of the tnormal that we are looking at, is the same on the tcolors vector
										//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
										tcolors[n] = uniqueColors[un];
								}
							}
						}
					}
				}
				//we now have a colors Vector with new colors assigned
				//put that colorVector on the current mesh of the current cubie
				myCubies[i]->myMeshs[j].clearColors();
				myCubies[i]->myMeshs[j].addColors(tcolors);
				//have to replace the vbo
				ofVbo tempVbo;
				tempVbo.setMesh(myCubies[i]->myMeshs[j], GL_STATIC_DRAW);
				myCubies[i]->myVbos[j]=tempVbo;
			}
		}
	}else{
		//got through each cubie
		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			for (int j = 0 ; j< meshesCubie; j++){
				//go through each cubies meshes 
				tcolors = myCubies[i]->myMeshs[j].getColors();
				//change colors
				for (int i = 0; i< tcolors.size(); i++){
					tcolors[i] = matePuzzle;
				}

				//we now have a colors Vector with new colors assigned
				//put that colorVector on the current mesh of the current cubie
				myCubies[i]->myMeshs[j].clearColors();
				myCubies[i]->myMeshs[j].addColors(tcolors);
				//have to replace the vbo
				ofVbo tempVbo;
				tempVbo.setMesh(myCubies[i]->myMeshs[j], GL_STATIC_DRAW);
				myCubies[i]->myVbos[j]=tempVbo;
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesOneByOne(cubie *myCubie, float playRoom, int objectID,vector< ofVec3f > ObjectUniqueNormals){
	//cout << "in COLOR FACES oneBone: " << ofGetElapsedTimeMillis() << endl;
	//goes through each cubie and makes sets of normals.. to determine all different normals in the object
	//i.e. this will give 8 + 6 faces for octahedor
	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;
	//create sets of distitnct normals from all the meshes of all the cubies of the puzzle
	vector< ofVec3f > uniqueNormals;

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);
	float meshesCubie =0;

	if(objectID < 8){//(objectID != 1 && objectID != 4){
		meshesCubie =  myCubie->getNumObjs();
		uniqueNormals = ObjectUniqueNormals;
		armX = myCubie->armRotations.x;
		armY = myCubie->armRotations.y;
		armZ = myCubie->armRotations.z;
		////create unique normals
		//for (int j = 0 ; j< meshesCubie; j++){
		//	//get the normals of the mesh!
		//	tnormals = myCubie->myMeshs[j].getNormals();
		//	//verify each normal value on unique normals vector
		//	for(int n=0; n< tnormals.size() ; n++){
		//		if(uniqueNormals.size() == 0){
		//			//the first normal of all the normals
		//			//push the first one
		//			uniqueNormals.push_back (tnormals[n]);
		//		}else{
		//			//it has at least one normal
		//			//compare current normal with all other normals
		//			for(int un = 0; un < uniqueNormals.size(); un ++){

		//				if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
		//					(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
		//					((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
		//					(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
		//					((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
		//					(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
		//					){
		//						//we already have that type of normal
		//						//stop looking through the vector
		//						un = uniqueNormals.size();
		//				}else{
		//					//its different
		//					//keep going until the last element
		//					if(un == uniqueNormals.size()-1){
		//						//its the las element on the vector of unique normals
		//						//if we got here its because the current normal (cn) is new to the set
		//						uniqueNormals.push_back(tnormals[n]);
		//						//stop this for.. we just changed the size
		//						un = uniqueNormals.size();
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		//at this point we have the unique normals of the object
		//each of these normals should have a unique color
		vector< ofFloatColor > uniqueColors;
		//build roster of colors for the current object
		for(int i =0; i< uniqueNormals.size(); i++){
			//for each unique normal
			//for now we select from 9 possible colors that we have right now
			ofFloatColor x =  colorsVector[i%16];
			uniqueColors.push_back(x);
		}
		//now -> uniqueColors.size = uniqueNormals.size


		//got through each cubie again

		meshesCubie =  myCubie->getNumObjs();
		for (int j = 0 ; j< meshesCubie; j++){
			//go through each cubies meshes again
			tnormals = myCubie->myMeshs[j].getNormals();
			tcolors = myCubie->myMeshs[j].getColors();
			//compare this normals to the uniqueNormals(index) to get the color from that uniqueColors(index)
			//go through uniqueNormals
			for(int un = 0; un<uniqueNormals.size();un++){
				//compare each t normal with each unique normal
				for(int n=0; n< tnormals.size() ; n++){
					if(armX==0 && armY==0 && armZ == 0){
						//if there are no arm rotations.. this works for cube official colors
						if(objectID == 1){
							//have to use the official colors
							///rotate normal vectors to compensate for armature rotations z-y-x
							//ask direction to color faces of cube 
							if(tnormals[n].align(x, 2.0)){
								tcolors[n] = blue; 
							}else if(tnormals[n].align(y, 2.0)){
								tcolors[n] = orange; 
							}else if(tnormals[n].align(z, 2.0)){
								tcolors[n] = yellow;
							}else if(tnormals[n].align(xn, 2.0)){
								tcolors[n] = green; 
							}else if(tnormals[n].align(yn, 2.0)){
								tcolors[n] = red; 
							}else if(tnormals[n].align(zn, 2.0)){
								tcolors[n] = white;
							}
						}else {
							if (((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && (tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
								((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && (tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
								((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && (tnormals[n].z <= (uniqueNormals[un].z + playRoom))
								){
									//if the cubies meshs normal is one of the unique normals
									//we assign a color to that normal on the cubie
									//the index of the tnormal that we are looking at, is the same on the tcolors vector
									//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
									tcolors[n] = uniqueColors[un];
							}
						}
					}else{
						//arm rotations
						ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
						ofVec3f t2 = t.getRotated(armX,ofVec3f(0,1,0));
						ofVec3f t3 = t2.getRotated(-armY,ofVec3f(1,0,0));

						if(objectID == 1){
							//have to use the official colors
							///rotate normal vectors to compensate for armature rotations z-y-x
							//ask direction to color faces of cube 
							if(t3.align(x, 2.0)){
								tcolors[n] = blue; 
							}else if(t3.align(y, 2.0)){
								tcolors[n] = orange; 
							}else if(t3.align(z, 2.0)){
								tcolors[n] = yellow;
							}else if(t3.align(xn, 2.0)){
								tcolors[n] = green; 
							}else if(t3.align(yn, 2.0)){
								tcolors[n] = red; 
							}else if(t3.align(zn, 2.0)){
								tcolors[n] = white;
							}
						}
						else{
							//another object, do as if there where no arm rotations
							if(((uniqueNormals[un].x - playRoom) <= t3.x) && 
								(t3.x <= (uniqueNormals[un].x + playRoom)) &&
								((uniqueNormals[un].y - playRoom) <= t3.y) && 
								(t3.y <= (uniqueNormals[un].y + playRoom)) &&
								((uniqueNormals[un].z - playRoom) <= t3.z) && 
								(t3.z <= (uniqueNormals[un].z + playRoom))
								){
									//if the cubies meshs normal is one of the unique normals
									//we assign a color to that normal on the cubie
									//the index of the tnormal that we are looking at, is the same on the tcolors vector
									//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
									tcolors[n] = uniqueColors[un];
							}
						}
					}
				}
			}
			//we now have a colors Vector with new colors assigned
			//put that colorVector on the current mesh of the current cubie
			myCubie->myMeshs[j].clearColors();
			myCubie->myMeshs[j].addColors(tcolors);
			//have to replace the vbo
			ofVbo tempVbo;
			tempVbo.setMesh(myCubie->myMeshs[j], GL_STATIC_DRAW);
			myCubie->myVbos[j]=tempVbo;
		}
	}else{
		//got a rounded shape
		//got through each cubie
		float meshesCubie =  myCubie->getNumObjs();
		for (int j = 0 ; j< meshesCubie; j++){
			//go through each cubies meshes 
			tcolors = myCubie->myMeshs[j].getColors();
			//change colors
			for (int i = 0; i< tcolors.size(); i++){
				tcolors[i] = matePuzzle;
			}

			//we now have a colors Vector with new colors assigned
			//put that colorVector on the current mesh of the current cubie
			myCubie->myMeshs[j].clearColors();
			myCubie->myMeshs[j].addColors(tcolors);
			//have to replace the vbo
			ofVbo tempVbo;
			tempVbo.setMesh(myCubie->myMeshs[j], GL_STATIC_DRAW);
			myCubie->myVbos[j]=tempVbo;
		}
	}
	//cout << "out COLOR FACES oneBone: " << ofGetElapsedTimeMillis() << endl;
}
//---------------------------------------------------------------------------------------------------------------
//void ofRender::colorBlackSides(ofMesh &mesh, int idCubie, float playRoom, int objectID){
////color black the correct sides of each cubie
//vector< ofVec3f > tnormals;
//vector< ofFloatColor > tcolors;
//ofPoint x = ofPoint(1,0,0);
//ofPoint y = ofPoint(0,1,0);
//ofPoint z = ofPoint(0,0,1);
//ofPoint xn = ofPoint(-1,0,0);
//ofPoint yn = ofPoint(0,-1,0);
//ofPoint zn = ofPoint(0,0,-1);
//ofFloatColor c;

//tnormals = mesh.getNormals();
//tcolors = mesh.getColors();

////check each normal
////decide according to cubie[num]
//for(int i=0; i<tnormals.size(); i++){
//	if(idCubie==0){
//		if(objectID == 200){
//			tcolors[i] = mateSolid;
//		}else{
//			//this is the center piece!! only color black the y and z axis
//			if(tnormals[i]==y){
//				c = black;
//			}else if(tnormals[i]==z){
//				c = black;
//			}else if(tnormals[i]==yn){
//				c = black;
//			}else if(tnormals[i]==zn){
//				c = black;
//			}else{
//				//leave same color
//				c = tcolors[i];
//			}
//			//tcolors[i] =  black;
//			tcolors[i] =  c;
//		}
//	}else if (idCubie==1){
//		//middle center blue
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if (idCubie==2){
//		//middle right yellow.blue
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if (idCubie==3){
//		//middle center yellow
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==4){
//		//middle left yellow/green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==5){
//		//middle center green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==6){
//		//middle right white/green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==7){
//		//middle center white
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==8){
//		//center left white/blue
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==9){
//		//top center
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==10){
//		//top middle red/blue
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==11){
//		//top right red/yello/blue
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==12){
//		//top middle red/yellow
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==13){
//		//top left red/yellow/green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==14){
//		//top middle red/green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==15){
//		//top right red/white/green
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==16){
//		//top middle red/white
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==17){
//		//top left red/blue/white
//		if(decideAxisRange(tnormals[i],playRoom)==y){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==18){
//		//bottom center orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==19){
//		//middle blue/orange
//		if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==20){
//		//bottom right yellow/blue/orange
//		if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==21){
//		//botom middle yellow/orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==22){
//		//bottom left yellow/green/orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}if(idCubie==23){
//		//bottom middle green/orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==zn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==24){
//		//bottom left white/green/orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==25){
//		//bottom middle white/orange
//		if(decideAxisRange(tnormals[i],playRoom)==x){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}else if(idCubie==26){
//		//bottom right white/blue/orange
//		if(decideAxisRange(tnormals[i],playRoom)==z){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==xn){
//			c = black;
//		}else if(decideAxisRange(tnormals[i],playRoom)==yn){
//			c = black;
//		}else{
//			//leave same color
//			c = tcolors[i];
//		}
//		tcolors[i] =  c;
//	}
//}
////replace the colors vector on the mesh
//mesh.clearColors();
//mesh.addColors(tcolors);
//}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorBlackSidesFromAxes(ofMesh &mesh, int xp, int yp, int zp, int gridSize, float playRoom){
	//color black the correct sides of each cubie
	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);
	ofFloatColor c;

	tnormals = mesh.getNormals();
	tcolors = mesh.getColors();

	//check each normal
	//decide according to cubie[num]
	for(int i=0; i<tnormals.size(); i++){
		c = tcolors[i];
		if (xp>0) {
			if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}
		} 
		if (xp<gridSize-1) {
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}
		}
		if (yp>0) {
			if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}
		} 
		if (yp<gridSize-1) {
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}
		}
		if (zp>0) {
			if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}
		} 
		if (zp<gridSize-1) {
			if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}
		}
		tcolors[i] =  c;
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorTorusMenu(ofMesh &mesh,vector< ofFloatColor > &vcolors){
	vector< ofFloatColor > tcolors;
	tcolors = mesh.getColors();
	ofFloatColor x;

	int r = ofRandom(100);
	int r2 = r%16;
	if(abs(r2) != 3){
		//to avoid white
		x =  colorsVector[r2];
		vcolors.push_back(x);//we only need one color sample, not all the vertices colors
	}else{
		x =  colorsVector[7];
		vcolors.push_back(x);
	}

	for(int i=0; i<tcolors.size(); i++){
		tcolors[i] =  x;
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorTorus(ofMesh &mesh){
	vector< ofFloatColor > tcolors;
	tcolors = mesh.getColors();
	ofFloatColor x;

	int r = ofRandom(100);
	if((r%16) != 3){
		//to avoid white
		x =  colorsVector[r%16];
	}

	for(int i=0; i<tcolors.size(); i++){
		tcolors[i] =  x;
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesMenu(ofMesh &mesh,ofVec3f armRot,float playRoom, int objectID, vector< ofFloatColor > &vcolorsMenu, vector< ofVec3f > &menuUniqueNormals){
	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;

	//create sets of distitnct normals from all the meshes of all the cubies of the puzzle
	vector< ofVec3f > uniqueNormals;

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	//get the normals of the mesh!
	tnormals = mesh.getNormals();
	armX = armRot.x;
	armY = armRot.y;
	armZ = armRot.z;
	//verify each normal value on unique normals vector
	for(int n=0; n< tnormals.size() ; n++){
		if(uniqueNormals.size() == 0){
			//the first normal of all the normals
			//push the first one
			uniqueNormals.push_back (tnormals[n]);
		}else{
			//now it has at least one normal
			//compare current normal with all other normals
			for(int un = 0; un < uniqueNormals.size(); un ++){

				if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
					(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
					((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
					(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
					((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
					(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
					){
						//we already have that type of normal
						//stop looking through the vector
						un = uniqueNormals.size();
				}else{
					//its different
					//keep going until the last element
					if(un == uniqueNormals.size()-1){
						//its the las element on the vector of unique normals
						//if we got here its because the current normal (cn) is new to the set
						uniqueNormals.push_back(tnormals[n]);
						//stop this for.. we just changed the size
						un = uniqueNormals.size();
					}
				}
			}
		}
	}

	//at this point we have the unique normals of the object
	menuUniqueNormals = uniqueNormals; //this is so that the menu object, can color the faces of its puzzle version, the same color as the displayed object.

	//each of these normals should have a unique color
	vector< ofFloatColor > uniqueColors;
	//build roster of colors for the current object
	for(int i =0; i< uniqueNormals.size(); i++){
		//for each unique normal
		//for now we select from 9 possible colors that we have right now
		ofFloatColor x =  colorsVector[i%16];
		if(x.r == 1 && x.g == 1 && x.b == 1){
			x = lightGreen;
		}
		uniqueColors.push_back(x);
		vcolorsMenu.push_back(x); //to remember the colors used on the main object,  to be used on the puzzle, so both have the same colors

	}
	//now -> uniqueColors.size = uniqueNormals.size

	//go through each cubies meshes again
	tnormals = mesh.getNormals();
	tcolors = mesh.getColors();
	//compare this normals to the uniqueNormals(index) to get the color from that uniqueColors(index)
	//go through uniqueNormals
	for(int un = 0; un<uniqueNormals.size();un++){
		//compare each t normal with each unique normal
		for(int n=0; n< tnormals.size() ; n++){
			if(armX==0 && armY==0 && armZ == 0){
				//if there are no arm rotations.. this works for cube official colors
				if(objectID == 1){
					//have to use the official colors
					///rotate normal vectors to compensate for armature rotations z-y-x
					//ask direction to color faces of cube 
					if(tnormals[n].align(x, 4.0)){
						tcolors[n] = blue; 
					}else if(tnormals[n].align(y, 4.0)){
						tcolors[n] = orange; 
					}else if(tnormals[n].align(z, 4.0)){
						tcolors[n] = yellow;
					}else if(tnormals[n].align(xn, 4.0)){
						tcolors[n] = green; 
					}else if(tnormals[n].align(yn, 4.0)){
						tcolors[n] = red; 
					}else if(tnormals[n].align(zn, 4.0)){
						tcolors[n] = white;
					}
				}else {
					if (((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && (tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
						((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && (tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
						((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && (tnormals[n].z <= (uniqueNormals[un].z + playRoom))
						){
							//if the cubies meshs normal is one of the unique normals
							//we assign a color to that normal on the cubie
							//the index of the tnormal that we are looking at, is the same on the tcolors vector
							//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
							tcolors[n] = uniqueColors[un];
					}
				}
			}else{
				//arm rotations
				if(objectID == 1){
					//have to use the official colors
					///rotate normal vectors to compensate for armature rotations z-y-x
					//ask direction to color faces of cube 
					ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
					ofVec3f t2 = t.getRotated(armY,ofVec3f(0,1,0));
					ofVec3f t3 = t2.getRotated(armX,ofVec3f(1,0,0));

					if(t3.align(x, 2.0)){
						tcolors[n] = blue; 
					}else if(t3.align(y, 2.0)){
						tcolors[n] = orange; 
					}else if(t3.align(z, 2.0)){
						tcolors[n] = yellow;
					}else if(t3.align(xn, 2.0)){
						tcolors[n] = green; 
					}else if(t3.align(yn, 2.0)){
						tcolors[n] = red; 
					}else if(t3.align(zn, 2.0)){
						tcolors[n] = white;
					}
				}
				else{
					//another object, do as if there where no arm rotations
					if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
						(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
						((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
						(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
						((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
						(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
						){
							//if the cubies meshs normal is one of the unique normals
							//we assign a color to that normal on the cubie
							//the index of the tnormal that we are looking at, is the same on the tcolors vector
							//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
							tcolors[n] = uniqueColors[un];
					}
				}
			}
		}
	}
	//we now have a colors Vector with new colors assigned
	//put that colorVector on the current mesh of the current cubie
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesMenuPuzzle(cubie **myCubies, int numPieces,float playRoom, int objectID, vector< ofFloatColor > &vcolors,vector< ofVec3f > &menuUniqueNormals){
	//goes through each cubie and makes sets of normals.. to determine all different normals in the object
	//i.e. this will give 8 + 6 faces for octahedron

	//this one compares those normals to the normals vector and gets the color for that normal from the menuPuzzle colorsVector

	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;
	//create sets of distitnct normals from all the meshes of all the cubies of the puzzle
	vector< ofVec3f > uniqueNormals;

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	if(objectID < 8){//(objectID != 1 && objectID != 4){
		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			for (int j = 0 ; j< meshesCubie; j++){
				//get the normals of the mesh!
				tnormals = myCubies[i]->myMeshs[j].getNormals();
				armX = myCubies[i]->armRotations.x;
				armY = myCubies[i]->armRotations.y;
				armZ = myCubies[i]->armRotations.z;
				//verify each normal value on unique normals vector
				for(int n=0; n< tnormals.size() ; n++){
					if(uniqueNormals.size() == 0){
						//the first normal of all the normals
						//push the first one
						uniqueNormals.push_back (tnormals[n]);
					}else{
						//it has at least one normal
						//compare current normal with all other normals
						for(int un = 0; un < uniqueNormals.size(); un ++){

							if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
								(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
								((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
								(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
								((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
								(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
								){
									//we already have that type of normal
									//stop looking through the vector
									un = uniqueNormals.size();
							}else{
								//its different
								//keep going until the last element
								if(un == uniqueNormals.size()-1){
									//its the las element on the vector of unique normals
									//if we got here its because the current normal (cn) is new to the set
									uniqueNormals.push_back(tnormals[n]);
									//stop this for.. we just changed the size
									un = uniqueNormals.size();
								}
							}
						}
					}
				}
			}
		}

		vector< ofFloatColor > uniqueColors;
		//build roster of colors for the current object
		for(int i =0; i< uniqueNormals.size(); i++){
			//for each unique normal
			//for now we select from 9 possible colors that we have right now
			ofFloatColor x =  colorsVector[i%16];
			uniqueColors.push_back(x);
		}
		//now -> uniqueColors.size = uniqueNormals.size

		////////at this point we have the unique normals of the object
		////////each of these normals should have a unique color
		//////vector< ofFloatColor > uniqueColors;
		////////build roster of colors for the current object
		//////for(int i =0; i< uniqueNormals.size(); i++){
		//////	//for each unique normal
		//////	//we search for that normal on the normal vector
		//////	for(int y=0; y<menuUniqueNormals.size();y++){
		//////		//if(uniqueNormals[i].align(menuUniqueNormals[y], 6.0)){
		//////		if (((uniqueNormals[y].x - playRoom) <= menuUniqueNormals[i].x) && (menuUniqueNormals[i].x <= (uniqueNormals[y].x + playRoom)) &&
		//////			((uniqueNormals[y].y - playRoom) <= menuUniqueNormals[i].y) && (menuUniqueNormals[i].y <= (uniqueNormals[y].y + playRoom)) &&
		//////			((uniqueNormals[y].z - playRoom) <= menuUniqueNormals[i].z) && (menuUniqueNormals[i].z <= (uniqueNormals[y].z + playRoom))
		//////			){
		//////				//when found
		//////				//we get the color from the colorVector on that same uniqueNormals index
		//////				uniqueColors.push_back(vcolors[y]);
		//////		}
		//////		else{
		//////			uniqueColors.push_back(white);
		//////		}
		//////	}

		//////}
		//now -> uniqueColors.size = uniqueNormals.size

		//got through each cubie again
		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			for (int j = 0 ; j< meshesCubie; j++){
				//go through each cubies meshes again
				tnormals = myCubies[i]->myMeshs[j].getNormals();
				tcolors = myCubies[i]->myMeshs[j].getColors();
				//compare this normals to the uniqueNormals(index) to get the color from that uniqueColors(index)
				//go through uniqueNormals
				for(int un = 0; un<uniqueNormals.size();un++){
					//compare each t normal with each unique normal
					for(int n=0; n< tnormals.size() ; n++){
						if(armX==0 && armY==0 && armZ == 0){
							//if there are no arm rotations.. this works for cube official colors
							if(objectID == 1){
								//have to use the official colors
								///rotate normal vectors to compensate for armature rotations z-y-x
								//ask direction to color faces of cube 
								if(tnormals[n].align(x, 2.0)){
									tcolors[n] = blue; 
								}else if(tnormals[n].align(y, 2.0)){
									tcolors[n] = orange; 
								}else if(tnormals[n].align(z, 2.0)){
									tcolors[n] = yellow;
								}else if(tnormals[n].align(xn, 2.0)){
									tcolors[n] = green; 
								}else if(tnormals[n].align(yn, 2.0)){
									tcolors[n] = red; 
								}else if(tnormals[n].align(zn, 2.0)){
									tcolors[n] = white;
								}
							}else {
								if (((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && (tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
									((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && (tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
									((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && (tnormals[n].z <= (uniqueNormals[un].z + playRoom))
									){
										//if the cubies meshs normal is one of the unique normals
										//we assign a color to that normal on the cubie
										//the index of the tnormal that we are looking at, is the same on the tcolors vector
										//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
										tcolors[n] = uniqueColors[un];
								}
							}
						}else{
							//arm rotations
							if(objectID == 1){
								//have to use the official colors for cube
								///rotate normal vectors to compensate for armature rotations z-y-x
								//ask direction to color faces of cube 
								ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
								ofVec3f t2 = t.getRotated(armY,ofVec3f(0,1,0));
								ofVec3f t3 = t2.getRotated(armX,ofVec3f(1,0,0));

								if(t3.align(x, 2.0)){
									tcolors[n] = blue; 
								}else if(t3.align(y, 2.0)){
									tcolors[n] = orange; 
								}else if(t3.align(z, 2.0)){
									tcolors[n] = yellow;
								}else if(t3.align(xn, 2.0)){
									tcolors[n] = green; 
								}else if(t3.align(yn, 2.0)){
									tcolors[n] = red; 
								}else if(t3.align(zn, 2.0)){
									tcolors[n] = white;
								}
							}
							else{
								//another object, do as if there where no arm rotations
								if(((uniqueNormals[un].x - playRoom) <= tnormals[n].x) && 
									(tnormals[n].x <= (uniqueNormals[un].x + playRoom)) &&
									((uniqueNormals[un].y - playRoom) <= tnormals[n].y) && 
									(tnormals[n].y <= (uniqueNormals[un].y + playRoom)) &&
									((uniqueNormals[un].z - playRoom) <= tnormals[n].z) && 
									(tnormals[n].z <= (uniqueNormals[un].z + playRoom))
									){
										//if the cubies meshs normal is one of the unique normals
										//we assign a color to that normal on the cubie
										//the index of the tnormal that we are looking at, is the same on the tcolors vector
										//the color that we want is the one that corresponds to the uniqueNormals(index) that matched-> that same index is used to get color from uniqueColors(index) vector
										tcolors[n] = uniqueColors[un];
								}
							}
						}
					}
				}
				//we now have a colors Vector with new colors assigned
				//put that colorVector on the current mesh of the current cubie
				myCubies[i]->myMeshs[j].clearColors();
				myCubies[i]->myMeshs[j].addColors(tcolors);
				//have to replace the vbo
				ofVbo tempVbo;
				tempVbo.setMesh(myCubies[i]->myMeshs[j], GL_STATIC_DRAW);
				myCubies[i]->myVbos[j]=tempVbo;
			}
		}
	}else{
		//shapes with non flat surfaces
		//got through each cubie
		for(int i=0;i<numPieces;i++){
			float meshesCubie =  myCubies[i]->getNumObjs();
			for (int j = 0 ; j< meshesCubie; j++){
				//go through each cubies meshes 
				tcolors = myCubies[i]->myMeshs[j].getColors();
				//change colors

				for (int i = 0; i< tcolors.size(); i++){
					tcolors[i] = vcolors[0];
				}

				//we now have a colors Vector with new colors assigned
				//put that colorVector on the current mesh of the current cubie
				myCubies[i]->myMeshs[j].clearColors();
				myCubies[i]->myMeshs[j].addColors(tcolors);
				//have to replace the vbo
				ofVbo tempVbo;
				tempVbo.setMesh(myCubies[i]->myMeshs[j], GL_STATIC_DRAW);
				myCubies[i]->myVbos[j]=tempVbo;
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesExtruded(cubie **myCubies, int numPieces, float playRoom, int objectID){
	//goes through each cubie and makes sets of normals.. to determine all different normals in the object
	//i.e. this will give 8 + 6 faces for octahedor
	vector< ofVec3f > tnormals;
	vector< ofFloatColor > tcolors;

	//create sets of distitnct normals from all the meshes of all the cubies of the puzzle
	vector< ofVec3f > uniqueNormals;

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	//got through each cubie again
	for(int i=0;i<numPieces;i++){
		float meshesCubie =  myCubies[i]->getNumObjs();
		for (int j = 0 ; j< meshesCubie; j++){
			//go through each cubies meshes again
			tnormals = myCubies[i]->myMeshs[j].getNormals();
			armX = myCubies[i]->armRotations.x;
			armY = myCubies[i]->armRotations.y;
			armZ = myCubies[i]->armRotations.z;
			tcolors = myCubies[i]->myMeshs[j].getColors();
			//compare this normals to the uniqueNormals(index) to get the color from that uniqueColors(index)
			//go through uniqueNormals
			//for(int un = 0; un<uniqueNormals.size();un++){
			//compare each t normal with each unique normal
			for(int n=0; n< tnormals.size() ; n++){
				if(armX==0 && armY==0 && armZ == 0){
					if(tnormals[n].align(y, 2.0)){
						tcolors[n] = orange; 
					}
					else if(tnormals[n].align(yn, 2.0)){
						tcolors[n] = red; 
					}
					else{
						tcolors[n] = mateSolid; 
					}
				}else{
					//arm rotations
					///rotate normal vectors to compensate for armature rotations z-y-x
					//ask direction to color faces of cube 
					ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
					ofVec3f t2 = t.getRotated(armY,ofVec3f(0,1,0));
					ofVec3f t3 = t2.getRotated(armX,ofVec3f(1,0,0));

					if(t3.align(y, 2.0)){
						tcolors[n] = orange; 
					}else if(t3.align(yn, 2.0)){
						tcolors[n] = red; 
					}
					else{
						tcolors[n] = mateSolid; 
					}
				}
			}

			//we now have a colors Vector with new colors assigned
			//put that colorVector on the current mesh of the current cubie
			myCubies[i]->myMeshs[j].clearColors();
			myCubies[i]->myMeshs[j].addColors(tcolors);
			//have to replace the vbo
			ofVbo tempVbo;
			tempVbo.setMesh(myCubies[i]->myMeshs[j], GL_STATIC_DRAW);
			myCubies[i]->myVbos[j]=tempVbo;
		}
	}
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesExtrudedMenu(ofMesh &mesh,ofVec3f armRot){
	vector< ofFloatColor > tcolors;
	vector< ofVec3f > tnormals;
	tcolors = mesh.getColors();
	tnormals = mesh.getNormals();

	float armX;
	float armY;
	float armZ;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	armX = armRot.x;
	armY = armRot.y;
	armZ = armRot.z;

	for(int n=0; n< tnormals.size() ; n++){
		if(armX==0 && armY==0 && armZ == 0){
			if(tnormals[n].align(y, 2.0)){
				tcolors[n] = orange; 
			}
			else if(tnormals[n].align(yn, 2.0)){
				tcolors[n] = red; 
			}else{
				tcolors[n] = mateSolid; 
			}
		}else{
			//arm rotations
			///rotate normal vectors to compensate for armature rotations z-y-x
			//ask direction to color faces of cube 
			ofVec3f t = tnormals[n].getRotated(armZ,ofVec3f(0,0,1));
			ofVec3f t2 = t.getRotated(armY,ofVec3f(0,1,0));
			ofVec3f t3 = t2.getRotated(armX,ofVec3f(1,0,0));

			if(t3.align(y, 2.0)){
				tcolors[n] = orange; 
			}else if(t3.align(yn, 2.0)){
				tcolors[n] = red; 
			}else{
				tcolors[n] = mateSolid; 
			}
		}
	}


	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
ofPoint ofRender::decideAxisRange(ofPoint dir,float playRoom){
	//looks at a point (normal vector) and decides which axis is closer according to the most prominent component of the vector
	/////////
	///////THS VERSION: uses a small range to do inequality check so that the selected axis is not so strict, and doesnt color black faces that are not the inside faces
	//////////
	ofPoint simple = ofPoint(0,0,0);
	int chosen=0;//1=x, 2=y, 3=z
	float absX;
	bool sx = false;
	float absY;
	bool sy = false;
	float absZ;
	bool sz = false;
	//take abs of all
	if(ofSign(dir.x)==-1){
		absX = (dir.x)*(-1);
	}else{
		absX = dir.x;
	}
	if(ofSign(dir.y)==-1){
		absY = (dir.y)*(-1);
	}else{
		absY = dir.y;
	}
	if(ofSign(dir.z)==-1){
		absZ = (dir.z)*(-1);
	}else{
		absZ = dir.z;
	}
	//look at the highest, and choose that one as the axis
	if((absX > absY)&& (absX > absZ)){
		//x is bigger
		chosen = 1;
	}else if((absY > absX)&& (absY > absZ)){
		//y is bigger
		chosen = 2;
	}else{
		//z is bigger
		chosen = 3;
	}
	//ask if its positive or negative
	if(chosen == 1){
		//x is chosen
		if(ofSign(dir.x)==1){
			//positive
			double xn = 1.0-playRoom;
			double xp = 1.0+playRoom;
			if((xn <= dir.x) && (dir.x <= xp)){
				simple = ofPoint(1,0,0);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else if(ofSign(dir.x)==-1){
			//negative
			double xn = -1.0-playRoom;
			double xp = -1.0+playRoom;
			if((xn <= dir.x) && (dir.x <= xp)){
				simple = ofPoint(-1,0,0);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else{
			//zero
		}
	}else if (chosen==2){
		//its y
		if(ofSign(dir.y)==1){
			//positive
			double yn = 1.0-playRoom;
			double yp = 1.0+playRoom;
			if((yn < dir.y) && (dir.y < yp)){
				simple = ofPoint(0,1,0);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else if(ofSign(dir.y)==-1){
			//negative
			double yn = -1.0-playRoom;
			double yp = -1.0+playRoom;
			if((yn < dir.y) && (dir.y < yp)){
				simple = ofPoint(0,-1,0);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else{
			//zero
		}
	}else{
		//its z
		if(ofSign(dir.z)==1){
			double zn = 1.0-playRoom;
			double zp = 1.0+playRoom;
			//positive
			if((zn < dir.z) && (dir.z < zp)){
				simple = ofPoint(0,0,1);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else if(ofSign(dir.z)==-1){
			//negative
			double zn = -1.0-playRoom;
			double zp = -1.0+playRoom;
			if((zn < dir.z) && (dir.z < zp)){
				simple = ofPoint(0,0,-1);
			}else{
				//return the original
				//dont want to consider it an "axis" vector
				simple = dir;
			}
		}else{
			//zero
		}
	}

	return simple;
}
//---------------------------------------------------------------------------------------------------------------
ofColor ofRender::decideColor(ofPoint normal){
	//decides a color according to a normal direction
	// 6 colores are taken as base color (normal rubiks color)
	//no inside face coloring
	//it colors the whole cubie 
	ofFloatColor c;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	if(normal==x){
		c = green; //green
	}else if(normal==y){
		c = orange; //orange
	}else if(normal==z){
		c = white; //white
	}else if(normal==xn){
		c = blue; //blue
	}else if(normal==yn){
		c = red; //red
	}else if(normal==zn){
		c = yellow; //yellow
	}
	return c;
}
//---------------------------------------------------------------------------------------------------------------
ofColor ofRender::decideColorCubieBox(ofPoint normal, int idCubie){
	//just for cube
	//special coloring function for TRADITIONAL rubicks coloring
	//takes into account the number of the cubie son it can decide the colors for that specific cubie
	//decides a color according to a normal direction
	// 6 colores are taken as base color (normal rubiks color)
	//1 more color is used on cubies : BLACK for the inside of each cubie

	ofFloatColor c;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	if(idCubie==0){
		//this is the center piece!! what to do with this??
		if(normal==x){
			c = redOrange;
		}else if(normal==y){
			c = redOrange;
		}else if(normal==z){
			c = redOrange;
		}else if(normal==xn){
			c = redOrange;
		}else if(normal==yn){
			c = redOrange;
		}else if(normal==zn){
			c = redOrange;
		}
	}else if (idCubie==1){
		//middle center blue

		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if (idCubie==2){
		//middle right yellow.blue
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if (idCubie==3){
		//middle center yellow
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==4){
		//middle left yellow/green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==5){
		//middle center green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==6){
		//middle right white/green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==7){
		//middle center white
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==8){
		//center left white/blue
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==9){
		//top center
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==10){
		//top middle red/blue
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==11){
		//top right red/yello/blue
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==12){
		//top middle red/yellow
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==13){
		//top left red/yellow/green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==14){
		//top middle red/green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==15){
		//top right red/white/green
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==16){
		//top middle red/white
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==17){
		//top left red/blue/white
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = black;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = red;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==18){
		//bottom center orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==19){
		//middle blue/orange
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==20){
		//bottom right yellow/blur/orange
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==21){
		//botom middle yellow/orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==22){
		//bottom left yellow/green/orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = yellow;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}if(idCubie==23){
		//bottom middle green/orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = black;
		}
	}else if(idCubie==24){
		//bottom left white/green/orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = green;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==25){
		//bottom middle white/orange
		if(normal==x){
			c = black;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}else if(idCubie==26){
		//bottom right white/blue/orange
		if(normal==x){
			c = blue;
		}else if(normal==y){
			c = orange;
		}else if(normal==z){
			c = black;
		}else if(normal==xn){
			c = black;
		}else if(normal==yn){
			c = black;
		}else if(normal==zn){
			c = white;
		}
	}
	return c;
}
/////-------------------------------------------------------------------------------------------------------------
ofColor ofRender::decideColorCubieDEBUG(ofPoint normal, int idCubie){
	//just for cube
	//special coloring function for TRADITIONAL rubicks coloring
	//takes into account the number of the cubie son it can decide the colors for that specific cubie
	//decides a color according to a normal direction
	// 6 colores are taken as base color (normal rubiks color)
	//1 more color is used on cubies : BLACK for the inside of each cubie

	ofFloatColor c;
	ofPoint x = ofPoint(1,0,0);
	ofPoint y = ofPoint(0,1,0);
	ofPoint z = ofPoint(0,0,1);
	ofPoint xn = ofPoint(-1,0,0);
	ofPoint yn = ofPoint(0,-1,0);
	ofPoint zn = ofPoint(0,0,-1);

	if(idCubie==0){
		c = redOrange;
	}else if (idCubie==1){
		c = blue;
	}else if (idCubie==2){
		c = yellow;
	}else if (idCubie==3){
		c = redOrange;
	}else if(idCubie==4){
		c = green;
	}else if(idCubie==5){
		c = redOrange;
	}else if(idCubie==6){
		c = white;
	}else if(idCubie==7){
		c = redOrange;
	}else if(idCubie==8){
		c = blue;
	}else if(idCubie==9){
		c = red;
	}else if(idCubie==10){
		c = blue;
	}else if(idCubie==11){
		c = yellow;
	}else if(idCubie==12){
		c = red;
	}else if(idCubie==13){
		c = green;
	}else if(idCubie==14){
		c = red;
	}else if(idCubie==15){
		c = green;
	}else if(idCubie==16){
		c = redOrange;
	}else if(idCubie==17){
		c = white;
	}else if(idCubie==18){
		c = redOrange;
	}else if(idCubie==19){
		c = orange;
	}else if(idCubie==20){
		c = yellow;
	}else if(idCubie==21){
		c = redOrange;
	}else if(idCubie==22){
		c = yellow;
	}else if(idCubie==23){
		c = orange;
	}else if(idCubie==24){
		c = green;
	}else if(idCubie==25){
		c = white;
	}else if(idCubie==26){
		c = blue;
	}
	return c;
}
//----------------------------------------------------------------------------------------------------------------
void ofRender::setNormals( ofMesh &mesh ){
	//this function is to produce normals for the triangle mesh
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	vector<ofPoint> norm( nV ); //Array for the normals

	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
	//cout << "num triangles:" << nT << endl;

	for (int t=0; t<nT; t++) {
		//Get indices of the triangle t
		int i1 = mesh.getIndex( 3 * t );
		int i2 = mesh.getIndex( 3 * t + 1 );
		int i3 = mesh.getIndex( 3 * t + 2 );

		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex( i1 );
		const ofPoint &v2 = mesh.getVertex( i2 );
		const ofPoint &v3 = mesh.getVertex( i3 );

		//Compute the triangle's normal
		ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();

		//Accumulate it to norm array for i1, i2, i3
		norm[ i1 ] += dir;
		norm[ i2 ] += dir;
		norm[ i3 ] += dir;
	}

	//Normalize the normal's length
	for (int i=0; i<nV; i++) {
		norm[i].normalize();
	}

	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals( norm );
}