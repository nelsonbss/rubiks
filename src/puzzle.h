#ifndef __Tpuzzle__puzzle__
#define __Tpuzzle__puzzle__

#include "ofMain.h"
#include "sgCore.h"
#include "cubie.h"

class puzzle {
	
public:
	puzzle();
    
	void setup();
	void update();
	void draw();

	void loadPieces(sgCGroup **pcs);

	cubie **myCubies; //malloc 27 of these
};

#endif /* defined(__Tpuzzle__puzzle__) */