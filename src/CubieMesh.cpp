#include "CubieMesh.h"

Triangle::Triangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c){
	center = (_v1 + _v2 + _v3) / 3;
	normal = _dir;
	color = _c;
	indeces.push_back(_in1);
	indeces.push_back(_in2);
	indeces.push_back(_in3);
	vertices.push_back(_v1);
	vertices.push_back(_v2);
	vertices.push_back(_v3);
}

void Triangle::setVertex(int _i, ofVec3f _v){
	vertices[_i] = _v;
}

void Triangle::resetCenter(){
	center = (vertices[0] + vertices[1] + vertices[2]) / 3;
}

CubieMesh::CubieMesh() : ofMesh(){
}

void CubieMesh::addTriangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c){
	Triangle tri(_in1, _in2, _in3, _v1, _v2, _v3, _dir, _c);
	triangles.push_back(tri);
}

Triangle CubieMesh::getNearest(ofVec3f _pnt){
	Triangle r = triangles[0];
	float distance = r.getCenter().distance(_pnt);
	for(auto tIter = triangles.begin() + 1; tIter != triangles.end(); tIter++){
		float tDist = tIter->getCenter().distance(_pnt);
		if(tDist < distance){
			distance = tDist;
			r = *tIter;
		}
	}
	return r;
}

vector<Triangle> CubieMesh::getTrianglesByNormal(ofVec3f _n){
	vector<Triangle> tris;
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		ofVec3f n = tIter->getNormal();
		float d = n.distance(_n);
		cout << "compared " << n.x << ", " << n.y << ", " << n.z << " to " << _n.x << ", " << _n.y << ", " << _n.z << " got " << d << endl;
		if(tIter->getNormal().distance(_n) < 0.05){
			cout << "adding to vector." << endl;
			tris.push_back(*tIter);
		}
	}
	return tris;
}

vector<Triangle> CubieMesh::getTrianglesByColor(ofVec3f _c){
	vector<Triangle> tris;
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		//tris.push_back(*tIter);
		ofVec3f tc;
		tc.x = tIter->getColor().r;
		tc.y = tIter->getColor().g;
		tc.z = tIter->getColor().b;
		if(tc.distance(_c) < 0.05){
			tris.push_back(*tIter);
		}
	}
	return tris;
}
 void CubieMesh::setColorToSet(vector<Triangle> tris, ofFloatColor _c){
	vector <ofFloatColor> colorsVectorT;
	colorsVectorT = getColors();
	for(auto tIter = tris.begin(); tIter != tris.end(); tIter++){
		cout << "resetting color" << endl;
		//setColorForIndices(tIter->getIndeces()[0], tIter->getIndeces()[2], _c);
		//setColor(tIter->getIndeces()[0], _c);
		//setColor(tIter->getIndeces()[1], _c);
		//setColor(tIter->getIndeces()[2], _c);
		colorsVectorT[tIter->getIndeces()[0]] = _c;
		colorsVectorT[tIter->getIndeces()[1]] = _c;
		colorsVectorT[tIter->getIndeces()[2]] = _c;
	}
	clearColors();
	addColors(colorsVectorT);
}

 void CubieMesh::rotateNormals(float _angle, ofVec3f _axis){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		tIter->setNormal(tIter->getNormal().rotate(_angle, _axis).normalize());
	}
 }

 void CubieMesh::rotateNormalsAround(float _angle, ofVec3f _axis, ofVec3f _pivot){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		tIter->setNormal(tIter->getNormal().rotate(_angle, _pivot, _axis).normalize());
	}
 }

 void CubieMesh::rotateVertices(float _angle, ofVec3f _axis){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		//tIter->getNormal().rotate(_angle, _axis);
		tIter->getVertices()[0].rotate(_angle, _axis);
		tIter->getVertices()[1].rotate(_angle, _axis);
		tIter->getVertices()[2].rotate(_angle, _axis);
	}
 }

 void CubieMesh::rotateVerticesAround(float _angle, ofVec3f _axis, ofVec3f _pivot){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		ofVec3f r = tIter->getVertices()[0].rotate(_angle, _pivot, _axis);
		tIter->setVertex(0,r);
		r = tIter->getVertices()[1].rotate(_angle, _pivot, _axis);
		tIter->setVertex(1,r);
		r = tIter->getVertices()[2].rotate(_angle, _pivot, _axis);
		tIter->setVertex(2, r);
		ofVec3f v1 = tIter->getVertices()[0];
		ofVec3f v2 = tIter->getVertices()[1];
		ofVec3f v3 = tIter->getVertices()[2];
		tIter->resetCenter();
	}
 }

void CubieMesh::updatePosition(ofVec3f _pivot, ofVec3f _axis, float _angle){
	rotateVerticesAround(_angle, _axis, _pivot);
	rotateNormalsAround(_angle, _axis, _pivot);
	resetVertices();
}

void CubieMesh::resetVertices(){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		//tIter->getNormal().rotate(_angle, _axis);
		vector<int>indeces = tIter->getIndeces();
		vector<ofVec3f> vertices = tIter->getVertices(); 
		for(int i = 0; i < indeces.size(); i++){
			ofVec3f currV = getVertex(indeces[i]);
			setVertex(indeces[i], vertices[i]);
			ofVec3f newV = getVertex(indeces[i]);
			//cout << "Set " << currV.x << ", " << currV.y << ", " << currV.z << " to " << newV.x << ", " << newV.y << ", " << newV.z << endl;
		}
	}
}