#include "GuiButton.h"
#include "Utils.h"

GuiButton::GuiButton(map<string, string> &_attrs, vector<SubObEvent*> _events) : GuiNode(){
	attrs = _attrs;
	events = _events;
	//cout << "button has " << events.size() << endl;
	if(attrs["image"] != "none"){
		inactive.loadImage(attrs["image"]);
		haveImage = true;
		size.x = inactive.getWidth() * scale;
		size.y = inactive.getHeight() * scale;
	} else {
		haveImage = false;
		size = stringToVec2f(attrs["size"]);
	}
	haveActive = false;
	if(attrs["clicked"] != "none"){
		active.loadImage(attrs["clicked"]);
		haveActive = true;
	}
	bDraggable = false;
	bTacky = false;
	bSelected = false;
	if(attrs.count("draggable")){
		if(attrs["draggable"] == "true"){
			bDraggable = true;
		}
		if(attrs.count("tacky")){
			if(attrs["tacky"] == "true"){
				bTacky = true;
			}
		}
	}
	drawActive = false;
	setName("button");
	setChannel("button");
	haveArabic = false;
	map<string,string>::iterator mIter;
	mIter = attrs.find("arabic");
	if(mIter != attrs.end()){
		haveArabic = true;
		arabic.loadImage(attrs["arabic"]);
	}
}

GuiButton::GuiButton(string _img) : GuiNode(){
	inactive.loadImage(_img);
	haveArabic = false;
	size.x = inactive.getWidth();
	size.y = inactive.getHeight();
	haveImage = true;
	drawActive = false;
	setName("button");
	setChannel("button");
}

void GuiButton::nodeInit(){
	if(params.count("image")){
		//cout << "loading image - " << params["image"] << endl;;
		inactive.loadImage(params["image"]);
		drawSize.x = inactive.getWidth();
		drawSize.y = inactive.getHeight();
		inactive.mirror(bFlipped, bFlipped);
		haveImage = true;
	} else {
		haveImage = false;
	}
	drawActive = false;
	haveActive = false;
	haveArabic = false;
	bSendActions = false;
	if(params.count("droppable")){
		if(params["droppable"] == "true"){
			//cout << "setting droppable on " << name << " to true." << endl;
			bSendActions = true;
			SubObMediator::Instance()->addObserver("object-intercepted",this);
		}
	}
	timeOfLastInteraction = 0;
	bWatchTime = false;
	timer = new ofxTimer();
	bSendSample = false;
	if(params.count("attach-sample")){
		if(params["attach-sample"] == "true"){
			sampleImage();
			bSendSample = true;
		}
	}
	bSend = false;
	SubObMediator::Instance()->addObserver(prefix + ":language-changed", this);
	//showLangBar = false;
}

void GuiButton::nodeExecute(){
	bReadyForInput = false;
	timer->addTimer(200, (int*)&bReadyForInput, 1);
	if(bSendSample){
		//cout << "sending sample from" << getPrefix() << endl;
		SubObEvent ev;
		ev.setName(getPrefix() + ":new-color");
		ev.addArg("color", sampleColor);
		ev.addArg("pos", dragPos);
		SubObMediator::Instance()->sendEvent(ev.getName(), ev);
	}
	//if(showLangBar == true){
	//	SubObEvent ev;
	//	ev.setName(getPrefix() + ":toggle");
	//	SubObMediator::Instance()->sendEvent(ev.getName(), ev);
	//	showLangBar = false;
	//}
	//cout << name << " executing." << endl;
}

void GuiButton::nodeSetPosition(){
	if(bMirrored){
		//cout << name << " adjusting." << endl;
		drawPos.x -= (inactive.getWidth() * scale);
	}
	if(bFlipped){
		drawPos.y -= (inactive.getHeight() * scale);
	}
}

bool GuiButton::processMouse(int _x, int _y, int _state){
	//cout << name << " being checked." << endl;
	//cout << name << " checking isInside." << endl;
	if(isInside(_x,_y)){
		//cout << "button clicked." << endl;
		if(_state == MOUSE_STATE_DOWN){
			if(!bDraggable){
				execute();
				return true;
			} else {
				bSelected = true;
				selectionLocation.set(_x, _y);
				dragStartLocation = drawPos;
				return true;
			}
		}
	}
	if(_state == MOUSE_STATE_DRAG){
		if(bSelected){
			ofVec2f mouseMovement = ofVec2f(_x, _y) - selectionLocation;
			drawPos = dragStartLocation + mouseMovement;
			//cout << "moving to " << drawPos.x << ", " << drawPos.y << endl;
			return true;
		}
	}
	if(_state == MOUSE_STATE_UP){
		if(bSelected){
			execute();
			if(bTacky){
				drawPos = dragStartLocation;
			} else {
				//pos = drawPos;
			}
			bSelected = false;
			return true;
		}
	}
	if(bSelected){
		//cout << "deselecting." << endl;
		bSelected = false;
	}
	return false;
}

void GuiButton::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	//cout << "Type = " << _type << " dX, dY = " << _deltaPos.x << ", " << _deltaPos.y << endl;
	if(_type == "drag"){
		ofVec2f currentMouse = _absPos;
		drawPos += currentMouse - lastMouse;
		//drawPos += _deltaPos;
		dragPos = drawPos;
		lastMouse = currentMouse;
		if(!bWatchTime){
			bWatchTime = true;
		}
	}
	if(_type == "tap"){
		//cout << name << " - executing" << endl;
		execute();
	}
	if(!bSend){
		bSend = true;
	}
	timeOfLastInteraction = ofGetElapsedTimeMillis();
	sendInteraction();
}

void GuiButton::update(string _subName, Subject* _sub){
}

void GuiButton::update(string _eventName, SubObEvent _event){
	if(_eventName == "object-intercepted"){
		if(_event.getArg("object-name")->getString() == name){
			setPosition();
			drawSize.x = inactive.getWidth();
			drawSize.y = inactive.getHeight();
			execute();
		}
	}
	if(_eventName == prefix + ":language-changed"){
		string lang = _event.getArg("lang")->getString();
		currentLanguage = lang;
		//toggle of the language bar
		//showLangBar = true;
	}
}

void GuiButton::sampleImage(){
	int w = inactive.width;
	int h = inactive.height;
	int bpp = inactive.bpp / 8;
	unsigned char * pixels = inactive.getPixels();
	int p = (h / 2) * w + (w / 2);
	//cout << "p = " << p << endl;
	float r = pixels[p*bpp+0];
	float g = pixels[p*bpp+1];
	float b = pixels[p*bpp+2];
	sampleColor.set(r,g,b);
}

void GuiButton::nodeDraw(){
	if(haveImage){
		if(!drawActive){
			//if(haveArabic && SceneManager::Instance()->getDisplayArabic()){
			//   arabic.draw(pos.x,pos.y);
			//} else {
			//cout << name << " drawing image at " << drawPos.x << ", " << drawPos.y << endl;
			inactive.draw(drawPos.x,drawPos.y, scale * drawSize.x,scale * drawSize.y);
			//}
		} else {
			//cout << name << " drawing active image at " << drawPos.x << ", " << drawPos.y << endl;
			active.draw(drawPos.x,drawPos.y, drawSize.x, drawSize.y);
		}
	} else {
		//ofRect(pos.x, pos.y, size.x, size.y);
		//cout << getName() << " has no image." << endl;
	}
	if(bWatchTime){
		if(ofGetElapsedTimeMillis() - timeOfLastInteraction > 400){
			setPosition();
			drawSize.x = inactive.getWidth();
			drawSize.y = inactive.getHeight();
			bWatchTime = 0;
			bSelected = false;
			SubObEvent ev;
			ev.setName(prefix + ":object-released");
			SubObMediator::Instance()->sendEvent(ev.getName(), ev);
		}
		if(ofGetElapsedTimeMillis() - timeOfLastInteraction > 200){
			if(bSendActions && bSend){
				SubObEvent ev;
				ev.setName("object-moved");
				ev.addArg("object-name", name);
				ev.addArg("position", ofVec2f(drawPos.x, drawPos.y));
				SubObMediator::Instance()->sendEvent("object-moved", ev);
				bSend = false;
				bSelected = false;
				ev.setName(prefix + ":object-released");
				SubObMediator::Instance()->sendEvent(ev.getName(), ev);
			}
		}
	}
}
