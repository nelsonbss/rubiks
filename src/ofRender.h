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
	ofPoint decideAxis(ofPoint dir);
	ofPoint decideAxisRange(ofPoint dir);
	ofColor decideColor(ofPoint normal);
	ofColor decideColorCubieBox(ofPoint normal,int numCubie);

	ofColor  decideColorCubiePyramid(ofPoint normal,int numCubie);
	ofColor  decideColorCubieDodecahedron(ofPoint normal,int numCubie);
	ofColor  decideColorCubieIcosahedron(ofPoint normal,int numCubie);
	ofColor  decideColorCubieOctahedron(ofPoint normal,int numCubie);

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
	void colorBlackSides(ofMesh &mesh, int idCubie);
	void colorFaces(cubie **myCubies, int numPieces);
	double playRoom; //to make a less strong axis deciding algorith
};

#endif /* defined(__TofRender__ofRender__) */