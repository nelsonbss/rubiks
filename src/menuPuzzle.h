#ifndef __TmenuPuzzle__menuPuzzle__
#define __TmenuPuzzle__menuPuzzle__

#include "puzzle.h"

class menuPuzzle {
	
public:
	menuPuzzle(puzzle *inpuzzle, int idi);

	void draw();

	puzzle *myMenuPuzzle;
	int id;
};

#endif /* defined(__TmenuPuzzle__menuPuzzle__) */