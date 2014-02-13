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
	vector< ofVec3f > vert;
	const SG_ALL_TRIANGLES* trngls = obj->GetTriangles();
	numTriangles = trngls->nTr;
	vertex = trngls->allVertex;
	// build vector of vertices from allthe vertices
	for (int i=0; i<(numTriangles*3);i++){ //each triangle has three vertices
		ofVec3f aux;
		aux.x = vertex[i].x;
		aux.y = vertex[i].y;
		aux.z =vertex[i].z;
		vert.push_back(aux);
		//mesh.addColor(ofFloatColor(0.5,0,0.5));
		mesh.addColor(ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));
		mesh.addVertex(aux);// .addVertices(vert);
	}
	//generate ofMesh
	//add vertices
	//mesh.addColor(ofFloatColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
	//mesh.addVertices(vert);
	//setup indices
	mesh.setupIndicesAuto();
	//set normals.. for lighting 
	setNormals(mesh);
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
	cout << "num triangles:" << nT << endl;

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
		
		//have to do a range function because normals are rearly directly on the axis direction

		//take abs of all, look at the highest, and choose that one as the axis
		//ask if its positive or negative

		/*ofPoint dirColor = decideAxis();
		ofPoint x = ofPoint(1,0,0);
		ofPoint y = ofPoint(0,1,0);
		ofPoint z = ofPoint(0,0,1);
		ofPoint xn = ofPoint(-1,0,0);
		ofPoint yn = ofPoint(0,-1,0);
		ofPoint zn = ofPoint(0,0,-1);

		if(dirColor==x){
			cout << "x axis normal " << t << endl;
		}else if(dirColor==y){
			cout << "y axis normal " << t<< endl;
		}else if(dirColor==z){
			cout << "z axis normal "<< t << endl;
		}else if(dirColor==xn){
			cout << "xn axis normal "<< t << endl;
		}else if(dirColor==yn){
			cout << "yn axis normal " << t<< endl;
		}else if(dirColor==zn){
			cout << "zn axis normal " << t << endl;
		}*/

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
//----------------------------------------------------------------------------------
void ofRender::decideAxis(){
}