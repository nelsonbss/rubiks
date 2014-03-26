#ifndef __TmenuPuzzle__menuPuzzle__
#define __TmenuPuzzle__menuPuzzle__

#include "puzzle.h"
#include "sgCore.h"

class menuPuzzle {

public:
	menuPuzzle (SG_VECTOR pos, SG_VECTOR t);
	menuPuzzle (sgC3DObject *object, puzzle *puzzle);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	void loadPuzzle(puzzle *inpuzzle);
	puzzle *myMenuPuzzle;
	int id;
	puzzle *getPuzzle();
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw();
	void setup();
	void update();
	void exit();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void loadObject(sgC3DObject *obj, int ID); //it will load a sgCore lib object: torus, box...
	sgC3DObject* getObject();//it will return the object, i.e to be used in boolean

	int objectId; //the id of the 3d selected object, the id comes from the main app
	sgC3DObject *object;
	sgC3DObject *temp;//the original will be in 0,0,0 to be cut. the temp will be moved around and be drawn.
	ofMesh myMesh;// this will be used to draw the object, 
	ofVbo myVbo;

	SG_VECTOR pos;
	SG_VECTOR tempPos;

	void colorFacesMenu();
	ofVec3f armRot;
	void applyArmRotations(ofVec3f v);
};

#endif /* defined(__TmenuPuzzle__menuPuzzle__) */