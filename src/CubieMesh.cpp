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
//--------------------------------------------------------------------------------
Triangle CubieMesh::getNearest(ofVec3f _pnt){

	if (triangles.size()==0) {
		/*
		This changed so it will generate a triangle if it is empty when the function is called. 
		I was not able to reproduce the error but I think this should cover it.
		*/
		cout << "Triangles has no content ERROR" << endl;
		addTriangle(0,1,2,ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(0,0,1),ofFloatColor(0,1));
	}
	Triangle r = triangles[0];
	int isNew=0;
	float distance = r.getCenter().distance(_pnt);
	for(auto tIter = triangles.begin() + 1; tIter != triangles.end(); tIter++){
		float tDist = tIter->getCenter().distance(_pnt);
		if(tIter->getColor().r != 0 && tIter->getColor().g !=0 && tIter->getColor().b != 0){
			//if its not black.. I'm interested in checking distance
			if(tDist < distance){
				distance = tDist;
				r = *tIter;
			}
		}else{
			//that triangle is painted black
			//so...choose another triangle
			cout << "color!! " << tIter->getColor() << endl;
		}
	}
	return r;
}
//---------------------------------------------------------------------------------
void Triangle::setColor(ofFloatColor colorIn){
	color.r = colorIn.r;
	color.g = colorIn.g;
	color.b = colorIn.b;
	color.a = colorIn.a;
}

//---------------------------------------------------------------------------------
vector<Triangle> CubieMesh::getTrianglesByNormal(ofVec3f _n){
	vector<Triangle> tris;
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		ofVec3f n = tIter->getNormal();
		//float d = n.distance(_n);
		//cout << "compared " << n.x << ", " << n.y << ", " << n.z << " to " << _n.x << ", " << _n.y << ", " << _n.z << " got " << d << endl;
		//if(tIter->getNormal().distance(_n) < 1.05){
		//n.x =  abs(n.x);
		//n.y = abs(n.y);
		//n.z = abs(n.z);

		//_n.x = abs(_n.x);
		//_n.y = abs(_n.y);
		//_n.z = abs(_n.z);
		//if(n.align(_n,2.0)==true){

		if (((n.x - 0.01) <= _n.x) && (_n.x <= (n.x + 0.01)) &&
			((n.y - 0.01) <= _n.y) && (_n.y <= (n.y + 0.01)) &&
			((n.z - 0.01) <= _n.z) && (_n.z <= (n.z + 0.01))
			){
				//cout << "adding to vector." << endl;
				tris.push_back(*tIter);
		}
	}
	return tris;
}
//-----------------------------------------------------------------
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
//---------------------------------------------------------------------------------------
void CubieMesh::setColorToSet(vector<Triangle> tris, ofFloatColor _c){
	vector <ofFloatColor> colorsVectorT;
	colorsVectorT = getColors();
	for(auto tIter = tris.begin(); tIter != tris.end(); tIter++){
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
void CubieMesh::setColorToSetArmature(ofFloatColor _c){
	vector <ofFloatColor> colorsVectorT;
	colorsVectorT = getColors();
	for(int i=0; i< colorsVectorT.size(); i++){
		colorsVectorT.at(i) = _c;
	}
	clearColors();
	addColors(colorsVectorT);
}
void CubieMesh::setColorToCurvedObject(ofFloatColor _c){
	vector <ofFloatColor> colorsVectorT;
	colorsVectorT = getColors();
	for(int i=0; i< colorsVectorT.size(); i++){
		colorsVectorT[i] = _c;
	}
	clearColors();
	addColors(colorsVectorT);
}


//void CubieMesh::rotateNormals(float _angle, ofVec3f _axis){
//	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
//		tIter->setNormal(tIter->getNormal().rotate(_angle, _axis).normalize());
//	}
//}

void CubieMesh::rotateNormalsAround(float _angle, ofVec3f _axis, ofVec3f _pivot){
	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
		tIter->setNormal(tIter->getNormal().rotate(_angle, _pivot, _axis).normalize());
	}
}

//void CubieMesh::rotateVertices(float _angle, ofVec3f _axis){
//	for(auto tIter = triangles.begin(); tIter != triangles.end(); tIter++){
//		//tIter->getNormal().rotate(_angle, _axis);
//		tIter->getVertices()[0].rotate(_angle, _axis);
//		tIter->getVertices()[1].rotate(_angle, _axis);
//		tIter->getVertices()[2].rotate(_angle, _axis);
//	}
//}

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

void CubieMesh::rotateTriangles(ofVec3f _pivot, ofVec3f _axis, float _angle){
	//rotate triangles so taht cubie picking gets also updated on hit test of triangle info
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
