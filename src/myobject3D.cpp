#include "myobject3D.h"
#include "sgCore.h"

myobject3D::myobject3D(float x, float y){
	posX = x;
	posY = y;
	deg =0;
}
//--------------------------------------------------------------
void myobject3D::setup(){
	temp = (sgC3DObject *) object->Clone();
	/*SG_VECTOR rotD = {posX,posY,0};
	temp->InitTempMatrix()->Translate(rotD);
	temp->ApplyTempMatrix();  */
	//temp->DestroyTempMatrix();
	//object->Triangulate(SG_VERTEX_TRIANGULATION);


	temp->Triangulate(SG_VERTEX_TRIANGULATION);
	temp->SetAttribute(SG_OA_COLOR,2);
	//convert to of mesh and draw as of
	int numTriangles =0;
	const SG_POINT *vertex;
	vector< ofVec3f > vert;
	//ofMatrix4x4 *m= temp->GetTempMatrix()->GetTransparentData();
	sgC3DObject *temp2 = (sgC3DObject*) temp;
	const SG_ALL_TRIANGLES* trngls = temp2->GetTriangles();
	numTriangles = trngls->nTr;
	//vertex = (SG_POINT*)malloc((numTriangles*3)*sizeof(SG_POINT*));
	//sgDeleteObject(temp2);

	vertex = trngls->allVertex;
	// build vector of vertices from allthe vertices
	for (int i=0; i<(numTriangles*3);i++){ //each triangle has three vertices
		ofVec3f aux;
		aux.x = vertex[i].x;
		aux.y = vertex[i].y;
		aux.z =vertex[i].z;
		vert.push_back(aux);
	}
	//generate ofMesh

	//add vertices
	myMesh.addVertices(vert);
	//setup indices
	myMesh.setupIndicesAuto();
	setNormals(myMesh);
}
//--------------------------------------------------------------
void myobject3D::update(){
	//SG_POINT rotP = {posX,posY,0};
	SG_POINT rotP = {0,0,0};
	SG_POINT rotPobject = {posX,posY,0};
	SG_VECTOR rotV = {0,1,0};
	SG_VECTOR rotD = {posX,posY,0};
	SG_VECTOR rotDobject = {0,0,0};
	deg += 0.01;
	temp->InitTempMatrix()->Rotate(rotP,rotV,deg);
	temp->GetTempMatrix()->Translate(rotD);
	temp->ApplyTempMatrix();  
	//temp->DestroyTempMatrix();

	//object->DestroyTempMatrix();
	//object->InitTempMatrix()->Rotate(rotPobject,rotV,0.01);
	//object->GetTempMatrix()->Translate(rotD);
	//object->GetTempMatrix()->Translate(rotDobject);
	//object->ApplyTempMatrix();
	//object->DestroyTempMatrix();

}
//--------------------------------------------------------------
void myobject3D::draw(){
	//ofTranslate(0,0);
	ofSetColor(ofColor(255,0,0));
	ofCircle(ofPoint(100,100),5);


	//sgGetScene()->AttachObject(temp);
	//instead of attaching to scene
	//ofPushMatrix();
	//ofTranslate(posX,posY);
	glMultMatrixd(temp->GetTempMatrix()->GetTransparentData());
	//ofMultMatrix(m);
	myMesh.draw();
	//ofPopMatrix();
	temp->DestroyTempMatrix();

	//object->SetAttribute(SG_OA_COLOR,5);
	////object->Triangulate(SG_VERTEX_TRIANGULATION);
	////SG_VECTOR rotD = {posX,posY,0};
	////object->InitTempMatrix()->Translate(rotD);
	////object->ApplyTempMatrix();  
	////object->DestroyTempMatrix();
	//sgGetScene()->AttachObject(object);
}
//--------------------------------------------------------------
void myobject3D::unDraw(){  
	sgGetScene()->DetachObject(temp);
	//sgGetScene()->DetachObject(object);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void myobject3D::loadObjectFromFile(const char* pathTofile){ 
	//it will load a stl file into object
	sgFileManager::ImportSTL(sgCScene::GetScene(), pathTofile);
	cout << "object from file: " << sgGetScene()->GetObjectsList()->GetCount() << endl;
}
//----------------------------------------------------------------
void myobject3D::loadObject(sgC3DObject *obj){
	//it will load a sgCore lib object: torus, box
	object = obj;
}
//----------------------------------------------------------------
sgCObject* myobject3D::getObject(){
	//it returns a clone of the object
	//NEVER return the real object
	return object;
}
//----------------------------------------------------------------
void myobject3D::exit(){
	sgCObject::DeleteObject(temp);
	sgCObject::DeleteObject(object);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void myobject3D::setNormals( ofMesh &mesh ){
	//this function is to normals for the triangle mesh
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	vector<ofPoint> norm( nV ); //Array for the normals

	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
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