#include "GuiNode.h"
#include "Utils.h"
#include "GuiConfigurator.h"

#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Latin9Encoding.h"

GuiNode::GuiNode(){
	drawColor.set(0,0,0);
	bDrawArea = false;
	bHaveText = false;
	bHaveText2 = false;
	bHidden = false;
	bActive = false;
	pos.set(0,0);
	bMirrored = false;
	bFlipped = false;
	bSelected = false;
	currentLanguage = "english";
	bUseHomePos = false;
	bHaveCustomArea = false;
	bControlled = false;
}

void GuiNode::draw(ofVec2f _pnt){
	ofVec2f tPos = drawPos;
	drawPos = _pnt;
	draw();
	drawPos = tPos;
}

void GuiNode::sendInteraction(){
	inter.setName(prefix + ":interaction");
	SubObMediator::Instance()->sendEvent(inter.getName(), inter);
}

void GuiNode::draw(){
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofDisableDepthTest();
	//if(!bHidden){
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
		string currentText = GuiConfigurator::Instance()->getText(text, currentLanguage);
		//string str = currentText;
		//string str_tex_ready;
		//Poco::TextConverter(Poco::UTF8Encoding(), Poco::Latin9Encoding()).convert(str, str_tex_ready);
		//currentText = str_tex_ready;
		//cout << "drawing text - " << currentText << endl;
		if(textAlign == "position"){
			if(bFlipped){
				ofRectangle bounds = font.getStringBoundingBox(currentText, 0, 0);
				ofPushMatrix();
				ofTranslate(drawPos.x + textPositionFlipped.x, drawPos.y + textPositionFlipped.y, 0);
				//ofTranslate(drawPos.x + textPositionFlipped.x  - bounds.width / 2, drawPos.y + textPositionFlipped.y - bounds.height / 2, 0);
				ofRotateZ(180.0);
				font.drawString(currentText, 0, 0);
				ofPopMatrix();
			} else {
				font.drawString(currentText, drawPos.x + textPosition.x, drawPos.y + textPosition.y);
			}
			if(bHaveText2){
				currentText = GuiConfigurator::Instance()->getText(text2, currentLanguage);
				//string str2 = currentText;
				//string str_tex_ready2;
				//Poco::TextConverter(Poco::UTF8Encoding(), Poco::Latin9Encoding()).convert(str2, str_tex_ready2);
				//currentText = str_tex_ready2;
				if(bFlipped){
					ofRectangle bounds = font.getStringBoundingBox(currentText, 0, 0);
					ofPushMatrix();
					//ofTranslate(drawPos.x + textPosition2Flipped.x  - bounds.width / 2, drawPos.y + textPosition2Flipped.y - bounds.height / 2, 0);
					ofTranslate(drawPos.x + textPosition2Flipped.x, drawPos.y + textPosition2Flipped.y, 0);
					ofRotateZ(180.0);
					//font.drawString(currentText, bounds.width / 2, bounds.height / 2);
					font.drawString(currentText, 0, 0);
					ofPopMatrix();
				} else {
					font.drawString(currentText, drawPos.x + textPosition2.x, drawPos.y + textPosition2.y);
				}
			}
		} else {
			ofRectangle box = font.getStringBoundingBox(currentText,0,0);
			float cx = 0;
			float cy = drawPos.y + (drawSize.y / 2 + box.height / 2);
			if(textAlign == "center"){
				cx = drawPos.x + (drawSize.x / 2 - box.width / 2);
			}
			if(bFlipped){
				ofPushMatrix();
				ofTranslate(cx + box.width, cy - box.height, 0);
				ofRotateZ(180.0);
				font.drawString(currentText, 0, 0);
				ofPopMatrix();
			} else {
				font.drawString(currentText, cx, cy);
			}
		}
	}
	//}
	ofEnableDepthTest();
}

bool GuiNode::isInside(int _x, int _y){
	//cout << name << " checking insides " << drawPos.x << ", " << drawPos.x + (scale * drawSize.x) << " - " << drawPos.y << ", " << drawPos.y + (scale * drawSize.y);
	//cout << " against " << _x << ", " << _y << endl;
	if(bHaveCustomArea){
		if(/*(drawPos.x + drawSize.x) < customArea.x || */(drawPos.y + drawSize.y) < customArea.y /*|| (drawPos.x + drawSize.x) > (customArea.x + customSize.x)*/ || (drawPos.y) > (customArea.y + customSize.y)){
			return false;
		}
	}
	if((_x > drawPos.x && _x < (drawPos.x + (scale * drawSize.x)) &&
		(_y > drawPos.y && _y < (drawPos.y + (scale * drawSize.y))))){
			if(bActive){
				//if its visible (specially done for language nodes)
				if(getParam("send-select") == "true"){
					input("select", 0, 0, 0, ofVec2f(_x, _y), ofVec2f(0,0));
				}
				lastMouse.set(_x, _y);
				bSelected = true;
				return true;
			}
	}
	return false;
}

void GuiNode::setCustomArea(ofVec2f _a, ofVec2f _s){
	bHaveCustomArea = true;
	customArea = _a;
	customSize = _s;
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
	if(getPages() != "none"){
		registerPages(getPages());
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
		bHidden= true;
	}
	nodeInit();
	nodeSetPosition();
	//timer = new ofxTimer();
}

void GuiNode::registerPages(string _pages){
	//	cout << getName() << " registering pages - " << _pages << endl;
	vector<string> pages = ofSplitString(_pages, ",");
	for(auto pIter = pages.begin(); pIter != pages.end(); pIter++){
		if(*pIter == "all"){
			prefix = "all";
		}
		GuiConfigurator::Instance()->addNodeToPage(prefix, *pIter, this);
	}
}

void GuiNode::setupText(){
	int fSize = 12;
	text = params["text"];
	if(params.count("font-size")){
		fSize = ofToInt(params["font-size"]);
	}
	if(params.count("font")){
		font.loadFont(params["font"], fSize*1.333, true, true,false,.3,72);
		font.setSpaceSize(.5);
		font.setLetterSpacing(.9);
		font.setLineHeight(ceil(fSize*1.25));
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
		if(bFlipped){
			if(params.count("text-position-flipped")){
				textPositionFlipped = stringToVec2f(params["text-position-flipped"]);
			} else {
				textPositionFlipped = textPosition;
			}
		}
	}
	if(params.count("text-position-mirrored")){
		cout << "THIS BUTTON IS..........." << bMirrored << " , " << bFlipped << endl;
		if (bMirrored && !bFlipped) {
			textPosition += stringToVec2f(params["text-position-mirrored"]);
		} 
		if (bMirrored && bFlipped) {
			textPositionFlipped += stringToVec2f(params["text-position-mirrored"]);
		}
	}

	if(params.count("text2")){
		bHaveText2 = true;
		text2 = params["text2"];
		textPosition2 = stringToVec2f(params["text-position2"]);
		if(bFlipped){
			if(params.count("text-position2-flipped")){
				textPosition2Flipped = stringToVec2f(params["text-position2-flipped"]);
			} else {
				textPosition2Flipped = textPosition;
			}
		}
	}
}

void GuiNode::activate(){
	bActive = true;
	GuiConfigurator::Instance()->addActive(this);
	//cout << name << " activating" << end;
	nodeActivate();
}

void GuiNode::deactivate(){
	bActive = false;
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
	//cout << name << " unhiding." << endl;
	//activate();
}

void GuiNode::setPosition(){
	if(bUseHomePos){
		drawPos = homePos;
		bSelected = false;
	} else {
		drawPos.x = ofGetWidth() * pos.x;
		drawPos.y = ofGetHeight() * pos.y;
		drawSize.x = ofGetWidth() * size.x;
		drawSize.y = ofGetHeight() * size.y;
	}
	nodeSetPosition();
}

void GuiNode::_windowResized(){
	setPosition();
	windowResized();
}

void GuiNode::execute(){
	//cout << "sending " << events.size() << " events." << endl;
	for(auto sIter = events.begin(); sIter != events.end(); ++sIter){
		SubObMediator::Instance()->sendEvent((*sIter)->getName(), *(*sIter));
	}
	nodeExecute();
	bSelected = false;
}