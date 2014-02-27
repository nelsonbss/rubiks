#ifndef __TofRender__myofRender__
#define __TofRender__myofRender__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"

#include <vector>
class ofRender{

public:
	ofRender();
	void sgCoretoOFmesh(sgC3DObject *obj, ofMesh &mesh,int numCubie,int selectedObjectID);
	void setNormals( ofMesh &mesh );
	ofPoint decideAxisRange(ofPoint dir,float playRoom);
	ofColor decideColor(ofPoint normal);
	ofColor decideColorCubieBox(ofPoint normal,int numCubie);

	vector <ofFloatColor> colorsVector;
	ofFloatColor mate;
	ofFloatColor green;
	ofFloatColor orange;
	ofFloatColor white;
	ofFloatColor blue;
	ofFloatColor red;
	ofFloatColor yellow;
	ofFloatColor black;
	ofFloatColor cyan;

	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc, ofMesh &mesh);
	
	void colorFaces(cubie **myCubies, int numPieces,float playRoom);
	void colorBlackSides(ofMesh &mesh, int idCubie, float playRoom);
};

#endif /* defined(__TofRender__ofRender__) */