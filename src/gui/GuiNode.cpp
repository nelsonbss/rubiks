#include "GuiNode.h"
#include "Utils.h"

bool GuiNode::isInside(int _x, int _y){
    //cout << name << " checking insides." << pos.x << ", " << size.x << endl;
    if((_x > drawPos.x && _x < (drawPos.x + size.x)) &&
       (_y > drawPos.y && _y < (drawPos.y + size.y))){
           return true;
       }
    return false;
}

GuiNode::GuiNode(){
    attrs.get_allocator().allocate(100);
}

void GuiNode::initialize(){
    type = attrs["type"];
    name = attrs["name"];
    pos = stringToVec2f(attrs["pos"]);
    scale = ofToFloat(attrs["scale"]);
	setPosition();
	//cout << "have " << events.size() << " events." << endl;
	//size = stringToVec2f(attrs["size"]);
}

void GuiNode::setPosition(){
	drawPos.x = ofGetWidth() * pos.x;
	drawPos.y = ofGetHeight() * pos.y;
}

void GuiNode::_windowResized(){
	setPosition();
	windowResized();
}

void GuiNode::execute(){
	cout << "sending " << events.size() << " events." << endl;
	for(vector<SubObEvent*>::iterator sIter = events.begin(); sIter != events.end(); ++sIter){
		SubObMediator::Instance()->sendEvent((*sIter)->getName(), (*sIter));
	}
}