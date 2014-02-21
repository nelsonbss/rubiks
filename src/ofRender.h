#ifndef __TofRender__myofRender__
#define __TofRender__myofRender__

#include "ofMain.h"
#include "sgCore.h"

#include <vector>
class ofRender{

public:
	ofRender();
	void sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh,int numCubie);
	void setNormals( ofMesh &mesh );
	ofPoint decideAxis(ofPoint dir);
	ofColor decideColor(ofPoint normal);
	ofColor decideColorCubie(ofPoint normal,int numCubie);

	vector <ofFloatColor> colorsVector;
	ofFloatColor c;
	ofFloatColor mate;
	ofFloatColor green;
	ofFloatColor orange;
	ofFloatColor white;
	ofFloatColor blue;
	ofFloatColor red;
	ofFloatColor yellow;
	ofFloatColor black;
	ofFloatColor cyan;

	const vector <ofFloatColor> colorsVectorC;
};

#endif /* defined(__TofRender__ofRender__) */