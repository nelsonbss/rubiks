#ifndef __Tslicer__slicer__
#define __Tslicer__slicer__

#include "ofMain.h"
#include "sgCore.h"
#include "cutter.h"
#include "myobject3D.h"

class slicer {
public:
	slicer(cutter *c, float x, float y);
    
	void setup();
	void update();
	void draw();

	void exit();

	//slicer coordinates
	float posX;
	float posY;
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	float rotH;
	float rotV;
	void rotateHright();
	void rotateHleft();
	void rotateVup();
	void rotateVdown();

	cutter *myCutter;
	sgCGroup  **pieces;//this array of GROUP pointers that will have the object for the 27 "pieces" for the puzzle
	//!!!important
	//its an array of groups because in one siliced piece, there might be 1 or more 3Dobjects
	//as the result of the boolean operation!!!
	sgCGroup** getPieces(); //returns the array of * of groups of objects for each volume(smallCube) of the puzzle
	int countPieces();

	//// algorithms
	///cubes
	sgCGroup **aux;
	void intersectCubes(sgCObject *obj);
	//planes
	void xSlicing(cutter &cut, sgCObject *obj, int turn, int cubePart);
	void ySlicing(cutter &cut, sgCGroup *grp, int turn, int cubePart);
	void zSlicing(cutter &cut, sgCGroup *grp, int turn, int cubePart);
};
#endif /* defined(__Tslicer__slicer__) */
