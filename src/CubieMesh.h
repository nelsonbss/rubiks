#ifndef CUBIEMESH_H
#define CUBIEMESH_H

#include "ofMain.h"

class Triangle{
	
public:
	Triangle(){}
	Triangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c);
	ofVec3f getNormal(){return normal;}
	void setNormal(ofVec3f _n){normal = _n;}
	ofVec3f getCenter(){return center;}
	ofFloatColor getColor(){return color;}
	vector<int> getIndeces(){return indeces;}
	vector<ofVec3f>getVertices(){return vertices;}
	void setVertex(int _i, ofVec3f _v);
	void resetCenter();

	vector<int> indeces;
	vector<ofVec3f> vertices;
	ofVec3f center;
	ofVec3f normal;
	ofFloatColor color;

private:
};

class CubieMesh : public ofMesh{

public:
	CubieMesh();  
	void addTriangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c);
	Triangle getNearest(ofVec3f _pnt);
	vector<Triangle> getTrianglesByNormal(ofVec3f _n);
	vector<Triangle> getTrianglesByColor(ofVec3f _c);
	void setColorToSet(vector<Triangle>, ofFloatColor _c);
	void setColorToCurvedObject(ofFloatColor _c);

	//void rotateNormals(float _angle, ofVec3f _axis);
	void rotateNormalsAround(float _angle, ofVec3f _axis, ofVec3f _pivot);
	//void rotateVertices(float _angle, ofVec3f _axis);
	void rotateVerticesAround(float _angle, ofVec3f _axis, ofVec3f _pivot);
	void updatePosition(ofVec3f _pivot, ofVec3f _axis, float _angle);
	void resetVertices();
	vector<Triangle> triangles;
	void rotateTriangles(ofVec3f _pivot, ofVec3f _axis, float _angle);

private:
	
};    

#endif;