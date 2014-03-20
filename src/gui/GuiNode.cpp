#include "GuiNode.h"
#include "Utils.h"
#include "GuiConfigurator.h"


GuiNode::GuiNode(){
    drawColor.set(0,0,0);
	bDrawArea = false;
	bHaveText = false;
	bHidden = false;
	bActive = true;
}

void GuiNode::draw(){
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if(!bHidden){
		if(bDrawArea){
			//cout << "drawing from " << drawPos.x << ", " << drawPos.y << " to " << drawSize.x << ", " << drawSize.y << endl;
			ofNoFill();
			ofSetColor(drawColor.x, drawColor.y, drawColor.z);
			ofRect(drawPos.x, drawPos.y, drawSize.x, drawSize.y);
		}
		ofSetColor(255,255,255);
		nodeDraw();
		if(bHaveText){
			ofSetColor(textColor.x, textColor.y, textColor.z);
			string currentText = GuiConfigurator::Instance()->getText(text);
			//cout << "drawing text - " << currentText << endl;
			if(textAlign == "position"){
				font.drawString(currentText, drawPos.x + textPosition.x, drawPos.y + textPosition.y);
			} else {
				ofRectangle box = font.getStringBoundingBox(currentText,0,0);
				float cx = 0;
				float cy = drawPos.y + (drawSize.y / 2 + box.height / 2);
				if(textAlign == "center"){
					cx = drawPos.x + (drawSize.x / 2 - box.width / 2);
				}
				font.drawString(currentText, cx, cy);
			}
		}
	}
}

bool GuiNode::isInside(int _x, int _y){
    cout << name << " checking insides " << drawPos.x << ", " << drawPos.x + (scale * drawSize.x) << " - " << drawPos.y << ", " << drawPos.y + (scale * drawSize.y);
	cout << " against " << _x << ", " << _y << endl;
    if((_x > drawPos.x && _x < (drawPos.x + (scale * drawSize.x)) &&
       (_y > drawPos.y && _y < (drawPos.y + (scale * drawSize.y))))){
		   if(getParam("send-select") == "true"){
			   input("select", 0, 0, 0, ofVec2f(_x, _y), ofVec2f(0,0));
		   }
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
	if(params.count("text")){
		setupText();
		bHaveText = true;
	}
	if(getParam("active") == "false"){
		bActive = false;
	}
	if(getParam("hidden") == "true"){
		bHidden = true;
	}
	nodeInit();
	//timer = new ofxTimer();
}

void GuiNode::setupText(){
	int fSize = 12;
	text = params["text"];
	if(params.count("font-size")){
		fSize = ofToInt(params["font-size"]);
	}
	if(params.count("font")){
		font.loadFont(params["font"], fSize, true, true);
	} else {
		font.loadFont("comic.ttf", fSize, true, true);
	}
	textColor.set(255,255,255);
	if(params.count("text-color")){
		textColor = stringToVec3f(params["text-color"]);
	}
	textAlign = "position";
	if(params.count("text-align")){
		textAlign = params["text-align"];
	}
	textPosition.set(0,0);
	if(params.count("text-position")){
		textPosition = stringToVec2f(params["text-position"]);
	}
}

void GuiNode::activate(){
	GuiConfigurator::Instance()->addActive(this);
	nodeActivate();
}

void GuiNode::deactivate(){
	GuiConfigurator::Instance()->removeActive(this);
	nodeDeactivate();
}

void GuiNode::hide(){
	bHidden = true;
	bActive = false;
	//deactivate();
}

void GuiNode::unhide(){
	bHidden = false;
	bActive = true;
	//activate();
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