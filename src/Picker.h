#ifndef PICKER_H
#define PICKER_H

#include "ofMain.h"

class Picker{

public:
	Picker();
	ofVec3f project(ofVec3f _pnt, ofRectangle* _vp);
	ofVec3f unproject(ofVec3f _pnt, ofRectangle* _vp);
};

#endif