#ifndef __TmenuPuzzle__menuPuzzle__
#define __TmenuPuzzle__menuPuzzle__

#include "puzzle.h"

class menuPuzzle {
	
public:
	menuPuzzle(puzzle *inpuzzle, int idi);

	void draw();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	puzzle *myMenuPuzzle;
	int id;

	puzzle *getPuzzle();
};

#endif /* defined(__TmenuPuzzle__menuPuzzle__) */