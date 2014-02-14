#ifndef __TofRender__myofRender__
#define __TofRender__myofRender__

#include "ofMain.h"
#include "sgCore.h"

class ofRender{

public:
	ofRender();
	void sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh,int numCubie);
	void setNormals( ofMesh &mesh );
	ofPoint decideAxis(ofPoint dir);
	ofColor decideColor(ofPoint normal);
	ofColor decideColorCubie(ofPoint normal,int numCubie);
};

#endif /* defined(__TofRender__ofRender__) */