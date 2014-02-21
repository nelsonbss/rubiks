#include "ofRender.h"
#include "ofMain.h"
#include "sgCore.h"
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
	
	//colorsVector.push_back(mate);//0
	//colorsVector.push_back(green);//1
	//colorsVector.push_back(orange);//2
	//colorsVector.push_back(white);//3
	//colorsVector.push_back(blue);//4
	//colorsVector.push_back(red);//5
	//colorsVector.push_back(yellow);//6
	//colorsVector.push_back(black);//7
	//colorsVector.push_back(cyan);//8
}

void ofRender::sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh,int idCubie){
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
		//look at normal
		////Compute the triangle's normal
		ofPoint dir = ( (auxV2 - auxV1).crossed( auxV3 - auxV1 ) ).normalized();

		//set colors
		ofFloatColor c;
		//ask if it is for a cubie
		if(idCubie == -1){
			//its not a cubie // but we want to see color
			c = decideColor(decideAxis(dir));
		}else if (idCubie == -2){
			//its an original shape// with plain color
			c = mate;//colorsVector[0];//mate
		}else{
			//it is a cubie
			c = decideColorCubie(decideAxis(dir),idCubie);
		}

		//mesh.addColor(c);
		colorsVector.push_back(c);
		mesh.addVertex(auxV1);

		//mesh.addColor(c);
		colorsVector.push_back(c);
		mesh.addVertex(auxV2);
		
		//mesh.addColor(c);
		colorsVector.push_back(c);
		mesh.addVertex(auxV3);
		

		//make indices & add normals to each index
		mesh.addIndex(i);
		mesh.addNormal(ofVec3f(normals[i].x,normals[i].y,normals[i].z));
		/*ofIndexType index = 5;
		ofFloatColor col = mesh.getColor(5);
		mesh.setColor(0,col);*/

		mesh.addIndex(i + 1);
		mesh.addNormal(ofVec3f(normals[i+1].x,normals[i+1].y,normals[i+1].z));
		/*mesh.setColor(0,col);*/
		mesh.addIndex(i + 2);
		mesh.addNormal(ofVec3f(normals[i+2].x,normals[i+2].y,normals[i+2].z));
		/*mesh.setColor(0,col);*/
	}
	mesh.addColors(colorsVector);
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
ofPoint ofRender::decideAxis(ofPoint dir){
	//looks at a point (normal vector) and decides which axis is closer to the most prominent component of the vector

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
		chosen =1;
	}else if((absY > absX)&& (absY > absZ)){
		chosen =2;
	}else{
		chosen = 3;
	}
	//ask if its positive or negative
	if(chosen == 1){
		//x is chosen
		if(ofSign(dir.x)==1){
			//positive
			simple = ofPoint(1,0,0);
		}else if(ofSign(dir.x)==-1){
			//negative
			simple = ofPoint(-1,0,0);
		}else{
			//zero
		}
	}else if (chosen==2){
		//its y
		if(ofSign(dir.y)==1){
			//positive
			simple = ofPoint(0,1,0);
		}else if(ofSign(dir.y)==-1){
			//negative
			simple = ofPoint(0,-1,0);
		}else{
			//zero
		}
	}else{
		//its z
		if(ofSign(dir.z)==1){
			//positive
			simple = ofPoint(0,0,1);
		}else if(ofSign(dir.z)==-1){
			//negative
			simple = ofPoint(0,0,-1);
		}else{
			//zero
		}
	}

	return simple;
}
//-----------------------------------------------------------------------
ofColor ofRender::decideColor(ofPoint normal){
	//decides a color according to a normal direction
	// 6 colores are taken as base color (normal rubiks color)
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
//-----------------------------------------------------------------------
ofColor ofRender::decideColorCubie(ofPoint normal, int idCubie){
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