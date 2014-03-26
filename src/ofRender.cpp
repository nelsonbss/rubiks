#include "ofRender.h"
#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include <vector>


ofRender::ofRender(){
	mate = ofFloatColor(1.0,1.0,1.0,0.5);
	green = ofFloatColor(0,1,0);
	orange = ofFloatColor(1.000, 0.549, 0.000);
	white = ofFloatColor(1,1,1);
	blue = ofFloatColor(0,0,1);
	red = ofFloatColor(1,0,0);
	yellow = ofFloatColor(1,1,0);
	black = ofFloatColor(0,0,0);
	cyan = ofFloatColor(0,1,1);

	colorsVector.push_back(blue);//0
	colorsVector.push_back(green);//1
	colorsVector.push_back(red);//2
	colorsVector.push_back(white);//3
	colorsVector.push_back(yellow);//4  
	colorsVector.push_back(orange);//5
	colorsVector.push_back(ofFloatColor(0.933, 0.510, 0.933));//6 violet
	colorsVector.push_back(ofFloatColor(0.576, 0.439, 0.859));//7 medium purple
	colorsVector.push_back(cyan);//8
}

void ofRender::sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh,int idCubie,int selectedObjectID){
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
		ofPoint dir = ( (auxV2 - auxV1).crossed( auxV3 - auxV1 ) ).normalized();
		//set colors
		ofFloatColor c;
		//ask if we color an original object with rubiks colors or plain color:: current selection is -2 for plain color
		if(idCubie == -1){
			//original shape with color...use this for pyramid 
			c = cyan;//decideColor(decideAxisRange(dir,0));
		}else if (idCubie == -2){
			//its an original shape  with plain color
			c = mate;//colorsVector2[0];//mate
		}else if (idCubie == -3){
			//plain color, used for bunny
			c = cyan;
		}else{
			//its another object
			//we only want to color PLAIN the sample object
			//the face coloring will be done separately

			if(selectedObjectID == 1){
				//c = decideColorCubieBox(dir,idCubie); //we color the cube here so it gets the official colors on its faces
				c = decideColorCubieDEBUG(dir,idCubie); //this is to test the offset slicing and rotations
				//}else if(selectedObjectID == 3){
				//	//cone(ish)
				//	c = decideColor(decideAxisRange(dir,10.0));
			}
		}

		colorsVector2.push_back(c);
		colorsVector2.push_back(c);
		colorsVector2.push_back(c);
	}
	mesh.addColors(colorsVector2);
	//ofFloatColor * col = mesh.getColorsPointer();
	////setup indices
	//mesh.setupIndicesAuto();
	////set normals.. for lighting 
	//setNormals(mesh);//before normals were done at the end, now they are done for each created triangle, so we can color it according to the normal
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::changeColorToColor(ofFloatColor Sc, ofFloatColor Tc, ofMesh &mesh){
	//it recieves a target color tolook for
	//a target color to change to
	//and the mesh in which to look for it
	vector <ofFloatColor> colorsVectorT;
	colorsVectorT = mesh.getColors();
	//looks on the colorvector for this color
	for(int i=0; i< colorsVectorT.size(); i++){
		ofFloatColor c = colorsVectorT[i];
		if((c.r == Sc.r) && (c.g == Sc.g)  && (c.b == Sc.b)){
			//when it finds it->replace it for the incoming color
			colorsVectorT[i] = Tc;
		}
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(colorsVectorT);
}
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
	//at this point we have the unique normals of the object
	//each of these normals should have a unique color
	vector< ofFloatColor > uniqueColors;
	//build roster of colors for the current object
	for(int i =0; i< uniqueNormals.size(); i++){
		//for each unique normal
		//for now we select from 9 possible colors that we have right now
		ofFloatColor x =  colorsVector[i%9];
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
					}else{
						//arm rotations
						if(objectID == 2){
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
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorBlackSides(ofMesh &mesh, int idCubie, float playRoom){
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
		if(idCubie==0){
			//this is the center piece!! only color black the y and z axis 
			if(tnormals[i]==y){
				c = black;
			}else if(tnormals[i]==z){
				c = black;
			}else if(tnormals[i]==yn){
				c = black;
			}else if(tnormals[i]==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			//tcolors[i] =  black;
			tcolors[i] =  c;
		}else if (idCubie==1){
			//middle center blue
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if (idCubie==2){
			//middle right yellow.blue
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if (idCubie==3){
			//middle center yellow
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==4){
			//middle left yellow/green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==5){
			//middle center green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==6){
			//middle right white/green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==7){
			//middle center white
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==8){
			//center left white/blue
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==9){
			//top center
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==10){
			//top middle red/blue
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==11){
			//top right red/yello/blue
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==12){
			//top middle red/yellow
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==13){
			//top left red/yellow/green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==14){
			//top middle red/green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==15){
			//top right red/white/green
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==16){
			//top middle red/white
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==17){
			//top left red/blue/white
			if(decideAxisRange(tnormals[i],playRoom)==y){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==18){
			//bottom center orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==19){
			//middle blue/orange
			if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==20){
			//bottom right yellow/blue/orange
			if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==21){
			//botom middle yellow/orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==22){
			//bottom left yellow/green/orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}if(idCubie==23){
			//bottom middle green/orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==zn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==24){
			//bottom left white/green/orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==25){
			//bottom middle white/orange
			if(decideAxisRange(tnormals[i],playRoom)==x){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}else if(idCubie==26){
			//bottom right white/blue/orange
			if(decideAxisRange(tnormals[i],playRoom)==z){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==xn){
				c = black;
			}else if(decideAxisRange(tnormals[i],playRoom)==yn){
				c = black;
			}else{
				//leave same color
				c = tcolors[i];
			}
			tcolors[i] =  c;
		}
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorTorus(ofMesh &mesh){
	vector< ofFloatColor > tcolors;
	tcolors = mesh.getColors();

	int r = ofRandom(100);
	ofFloatColor x =  colorsVector[r%9];

	for(int i=0; i<tcolors.size(); i++){
		tcolors[i] =  x;
	}
	//replace the colors vector on the mesh
	mesh.clearColors();
	mesh.addColors(tcolors);
}
//---------------------------------------------------------------------------------------------------------------
void ofRender::colorFacesMenu(ofMesh &mesh,ofVec3f armRot,float playRoom, int objectID){
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
	//each of these normals should have a unique color
	vector< ofFloatColor > uniqueColors;
	//build roster of colors for the current object
	for(int i =0; i< uniqueNormals.size(); i++){
		//for each unique normal
		//for now we select from 9 possible colors that we have right now
		ofFloatColor x =  colorsVector[i%9];
		uniqueColors.push_back(x);
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
			}else{
				//arm rotations
				if(objectID == 202){
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
			c = cyan;
		}else if(normal==y){
			c = cyan;
		}else if(normal==z){
			c = cyan;
		}else if(normal==xn){
			c = cyan;
		}else if(normal==yn){
			c = cyan;
		}else if(normal==zn){
			c = cyan;
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
		c = cyan;
	}else if (idCubie==1){
		c = blue;
	}else if (idCubie==2){
		c = yellow;
	}else if (idCubie==3){
		c = cyan;
	}else if(idCubie==4){
		c = green;
	}else if(idCubie==5){
		c = cyan;
	}else if(idCubie==6){
		c = white;
	}else if(idCubie==7){
		c = cyan;
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
		c = cyan;
	}else if(idCubie==17){
		c = white;
	}else if(idCubie==18){
		c = cyan;
	}else if(idCubie==19){
		c = orange;
	}else if(idCubie==20){
		c = yellow;
	}else if(idCubie==21){
		c = cyan;
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