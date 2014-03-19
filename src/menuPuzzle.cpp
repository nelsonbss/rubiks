#include "menuPuzzle.h"
#include "puzzle.h"

menuPuzzle::menuPuzzle(puzzle *inpuzzle, int idi){

	myMenuPuzzle = inpuzzle;
	id= idi;
}

void menuPuzzle::draw(){  
	myMenuPuzzle->draw();
}