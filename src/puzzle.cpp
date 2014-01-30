#include "puzzle.h"
#include "sgCore.h"
#include "cubie.h"

puzzle::puzzle(){
	myCubies = (cubie**)malloc(27*sizeof(cubie*));
}
//--------------------------------------------------------------
void puzzle::setup(){
}
//--------------------------------------------------------------
void puzzle::update(){
}
//--------------------------------------------------------------
void puzzle::draw(){  

}
///////////////////////////////////////////////////////////////
void loadPieces(sgCGroup **pcs){
	//it loads the pieces that the slicer made
	//it loads them into its own cubies


	for(int i=0;i<27;i++){
		cubie *auxCubie = new cubie();

		sgCGroup *parts = (sgCGroup*)pcs[i];
		const int ChCnt = parts->GetChildrenList()->GetCount();
		sgCObject** allParts = (sgCObject**)malloc(ChCnt*sizeof(sgCObject*));
		parts->BreakGroup(allParts);// ->BreakGroup(allChilds3);
		sgCObject::DeleteObject(parts);

		for (int j=0; j < ChCnt; j++){
			//clone each part
			allParts[j]->Clone();
			//make them a group

			//put that gorup inside a cubie

			//put that cubie on the cubies[]
		}
		free(allParts);

	}
	//myCubies[0] = pcs[0];
}
