#ifndef __TofRender__myofRender__
#define __TofRender__myofRender__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"

#include <vector>
class ofRender{

public:
	ofRender();
	void sgCoretoOFmesh(sgC3DObject *obj, CubieMesh &mesh,int numCubie,int selectedObjectID);
	void setNormals(ofMesh &mesh);

	vector <ofFloatColor> colorsVector;
	ofFloatColor mate;
	ofFloatColor mateSolid;
	ofFloatColor green;
	ofFloatColor orange;
	ofFloatColor white;
	ofFloatColor blue;
	ofFloatColor red;
	ofFloatColor yellow;
	ofFloatColor black;
	ofFloatColor redOrange;
	ofFloatColor pink;
	ofFloatColor purple;
	ofFloatColor lightGreen;
	ofFloatColor brown;
	ofFloatColor gray;
	ofFloatColor yellowGreen;
	ofFloatColor aqua;
	ofFloatColor indigo;

	void changeColorToColor(ofFloatColor Sc, ofFloatColor Tc, ofMesh &mesh); //from a source color to a target color of the mesh

	void colorFaces(cubie **myCubies, int numPieces,float playRoom, int objectID); //to color faces of any polyhedra (not cube - not tetrahedron)
	void colorBlackSides(ofMesh &mesh, int idCubie, float playRoom, int objectID); //to color black side faces of polyhedra  (not cube - not tetrahedron)
	void colorTorus(ofMesh &mesh);
	void colorFacesExtruded(cubie **myCubies, int numPieces,float playRoom, int objectID);
	void colorFacesExtrudedMenu(ofMesh &mesh,ofVec3f armRot);

	ofPoint decideAxisRange(ofPoint dir,float playRoom); // tho look at normals and decide sets of normals, to color faces

	ofColor decideColorCubieBox(ofPoint normal,int numCubie); //only used for cube
	ofColor decideColorCubieDEBUG(ofPoint normal,int numCubie);
	///////////////////////////////////////////////////////////////////////////////////////////
	ofColor decideColor(ofPoint normal); 
	//this function is ONLY being used by tetrahedron
	////////////////////////////////////////////////////////////////////////////////////////////

	void colorFacesMenu(ofMesh &mesh,ofVec3f armRot,float playRoom, int objectID);
};

#endif /* defined(__TofRender__ofRender__) */