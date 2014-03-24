#include "menuPuzzle.h"
#include "puzzle.h"

menuPuzzle::menuPuzzle(puzzle *inpuzzle, int idi){

	myMenuPuzzle = inpuzzle;
	id= idi;
}

void menuPuzzle::draw(){  
	myMenuPuzzle->draw();
}
//-------------------------------------------------------
puzzle * menuPuzzle::getPuzzle(){
	return myMenuPuzzle;
}
void menuPuzzle::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void menuPuzzle::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void menuPuzzle::mouseReleased(int x, int y, int button){
}