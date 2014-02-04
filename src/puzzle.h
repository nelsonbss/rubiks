#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"
#include "slicer.h"

class puzzle {
	
public:
	puzzle();
    
	void setup();
	void update();
	void draw(sgCScene *scn);
	void draw1(sgCScene *scn);

	void rotate();

	void loadPieces(sgCGroup **pcs);
	int giveNumCubies();

	float numPieces;
	cubie **myCubies; //malloc numPieces of these
};

#endif /* defined(__Tpuzzle__puzzle__) */