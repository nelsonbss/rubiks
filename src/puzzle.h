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
	void draw();

	void loadPieces(slicer &pcs);

	cubie **myCubies; //malloc 27 of these
};

#endif /* defined(__Tpuzzle__puzzle__) */