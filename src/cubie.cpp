#include "cubie.h"
#include "sgCore.h"

cubie::cubie(){
}
//--------------------------------------------------------------
void cubie::setup(){
	objects = NULL;
}
//--------------------------------------------------------------
void cubie::update(){
}
//--------------------------------------------------------------
void cubie::draw(){  

}

void cubie::setObjects(sgCGroup *objs){
	objects = objs;
}
