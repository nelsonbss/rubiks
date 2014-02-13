#ifndef __TofRender__myofRender__
#define __TofRender__myofRender__

#include "ofMain.h"
#include "sgCore.h"

class ofRender{

public:
	ofRender();
	void sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh);
	void setNormals( ofMesh &mesh );
	void decideAxis();
};

#endif /* defined(__TofRender__ofRender__) */