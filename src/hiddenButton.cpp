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
	//ofColor(255,0,0);
	//ofRect(pos.x,pos.y,size.x,size.y);
	ofPopMatrix();
}
void hiddenButton::exit(){
}
//-----------------------------
bool hiddenButton::hitTest(int mx, int my){
	if(mx>=pos.x && mx<= pos.x+size.x){
		if(my>=pos.y && my<=pos.y+size.y){
			return true;
		}
		else{
			return false;
		}
	}else{
		return false;
	}
}
