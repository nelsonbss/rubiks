#include "ofRender.h"
#include "ofMain.h"
#include "sgCore.h"

ofRender::ofRender(){
}

void ofRender::sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh){
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
		
		ofPoint myNormal = decideAxis(dir);
		ofPoint x = ofPoint(1,0,0);
		ofPoint y = ofPoint(0,1,0);
		ofPoint z = ofPoint(0,0,1);
		ofPoint xn = ofPoint(-1,0,0);
		ofPoint yn = ofPoint(0,-1,0);
		ofPoint zn = ofPoint(0,0,-1);
		ofFloatColor c;

		if(myNormal==x){
			c = ofFloatColor(0,1,0);
			//cout << "x axis normal " << c << endl;
		}else if(myNormal==y){
			c = ofFloatColor(1.000, 0.549, 0.000);
			//cout << "y axis normal " << c<< endl;
		}else if(myNormal==z){
			c = ofFloatColor(1,1,1);
			//cout << "z axis normal "<< c << endl;
		}else if(myNormal==xn){
			c = ofFloatColor(0,0,1);
			//cout << "xn axis normal "<< c << endl;
		}else if(myNormal==yn){
			c = ofFloatColor(1,0,0);
			
			//cout << "yn axis normal " << c << endl;
		}else if(myNormal==zn){
			c = ofFloatColor(1,1,0);
			//cout << "zn axis normal " << c << endl;
		}
		mesh.addColor(c);
		mesh.addVertex(auxV1);
		mesh.addColor(c);
		mesh.addVertex(auxV2);
		mesh.addColor(c);
		mesh.addVertex(auxV3);

		//make indices & add normals to each index
		mesh.addIndex(i);
		mesh.addNormal(ofVec3f(normals[i].x,normals[i].y,normals[i].z));
		mesh.addIndex(i + 1);
		mesh.addNormal(ofVec3f(normals[i+1].x,normals[i+1].y,normals[i+1].z));
		mesh.addIndex(i + 2);
		mesh.addNormal(ofVec3f(normals[i+2].x,normals[i+2].y,normals[i+2].z));
	}

	//////////setup indices
	////////mesh.setupIndicesAuto();
	//////////set normals.. for lighting 
	////////setNormals(mesh);//before normals were done at the end, now they are done for each created triangle, so we can color it according to the normal
}
//----------------------------------------------------------------------------------------------------------------
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