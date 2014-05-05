#include "hiddenButton.h"


hiddenButton::hiddenButton(ofVec2f posIn, ofVec2f sizeIn){
	pos.x = posIn.x;
	pos.y = posIn.y;

	size.x = sizeIn.x;
	size.y = sizeIn.y;
}

void hiddenButton::setup(){
}
void hiddenButton::update(){
}
void hiddenButton::draw(){
	ofPushMatrix();
	ofColor(255,0,0);
	ofRect(pos.x,pos.y,size.x,size.y);
	ofPopMatrix();
}
void hiddenButton::exit(){
}
