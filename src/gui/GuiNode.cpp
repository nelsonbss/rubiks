#include "GuiNode.h"
#include "Utils.h"
#include "GuiConfigurator.h"


GuiNode::GuiNode(){
    drawColor.set(0,0,0);
	bDrawArea = false;
}

void GuiNode::draw(){
	if(bDrawArea){
		//cout << "drawing from " << drawPos.x << ", " << drawPos.y << " to " << drawSize.x << ", " << drawSize.y << endl;
		ofNoFill();
		ofSetColor(drawColor.x, drawColor.y, drawColor.z);
		ofRect(drawPos.x, drawPos.y, drawSize.x, drawSize.y);
	}
	nodeDraw();
}

bool GuiNode::isInside(int _x, int _y){
    cout << name << " checking insides " << drawPos.x << ", " << drawPos.x + (scale * drawSize.x) << " - " << drawPos.y << ", " << drawPos.y + (scale * drawSize.y);
	cout << " against " << _x << ", " << _y << endl;
    if((_x > drawPos.x && _x < (drawPos.x + (scale * drawSize.x)) &&
       (_y > drawPos.y && _y < (drawPos.y + (scale * drawSize.y))))){
		   return true;
       }
    return false;
}

void GuiNode::init(){
	setPosition();
	if(params.count("draw-color")){
		drawColor = stringToVec3f(params["draw-color"]);
	}
	if(params.count("draw-area")){
		if(params["draw-area"] == "true"){
			bDrawArea = true;
		}
	}
	drawZ = 0.0;
	if(params.count("z")){
		drawZ = ofToFloat(params["z"]);
	}
	bReadyForInput = true;
	nodeInit();
	timer = new ofxTimer();
}

void GuiNode::activate(){
	GuiConfigurator::Instance()->addActive(this);
}

void GuiNode::deactivate(){
	GuiConfigurator::Instance()->removeActive(this);
}

void GuiNode::setPosition(){
	drawPos.x = ofGetWidth() * pos.x;
	drawPos.y = ofGetHeight() * pos.y;
	drawSize.x = ofGetWidth() * size.x;
	drawSize.y = ofGetHeight() * size.y;
}

void GuiNode::_windowResized(){
	setPosition();
	windowResized();
}

void GuiNode::execute(){
	//cout << "sending " << events.size() << " events." << endl;
	for(vector<SubObEvent*>::iterator sIter = events.begin(); sIter != events.end(); ++sIter){
		SubObMediator::Instance()->sendEvent((*sIter)->getName(), (*sIter));
	}
	nodeExecute();
}