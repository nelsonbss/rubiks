#ifndef CUBIEMESH_H
#define CUBIEMESH_H

#include "ofMain.h"

class Triangle{
	
public:
	Triangle(){}
	Triangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c);
	ofVec3f getNormal(){return normal;}
	ofVec3f getCenter(){return center;}
	ofFloatColor getColor(){return color;}
	vector<int> getIndeces(){return indeces;}

private:
	vector<int> indeces;
	vector<ofVec3f> vertices;
	ofVec3f center;
	ofVec3f normal;
	ofFloatColor color;
};

class CubieMesh : public ofMesh{

public:
	CubieMesh();  
	void addTriangle(int _in1, int _in2, int _in3, ofVec3f _v1, ofVec3f _v2, ofVec3f _v3, ofVec3f _dir, ofFloatColor _c);
	Triangle getNearest(ofVec3f _pnt);
	vector<Triangle> getTrianglesByNormal(ofVec3f _n);
	vector<Triangle> getTrianglesByColor(ofVec3f _c);
	void setColorToSet(vector<Triangle>, ofFloatColor _c);

private:
	vector<Triangle> triangles;
};    

#endif;