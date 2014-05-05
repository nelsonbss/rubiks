#ifndef __ThiddenButton__hiddenButton__
#define __ThiddenButton__hiddenButton__

#include "ofMain.h"

class hiddenButton
{
public:
	hiddenButton(ofVec2f posIn, ofVec2f sizeIn);

	void setup();
	void update();
	void draw();
	void exit();

	ofVec2f pos;
	ofVec2f size;

};

#endif /* defined(__ThiddenButton__hiddenButton__) */