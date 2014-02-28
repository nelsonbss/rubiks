#include "GuiButton.h"
#include "Utils.h"

GuiButton::GuiButton(map<string, string> &_attrs, vector<SubObEvent*> _events) : GuiNode(){
	attrs = _attrs;
	events = _events;
	//cout << "button has " << events.size() << endl;
	initialize();
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

void GuiButton::init(){
	if(params.count("image")){
		cout << "loading image - " << params["image"] << endl;;
		inactive.loadImage(params["image"]);
		size.x = inactive.getWidth();
		size.y = inactive.getHeight();
		haveImage = true;
	} else {
		haveImage = false;
	}
	setPosition();
	drawActive = false;
	haveActive = false;
	haveArabic = false;
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
		cout << "deselecting." << endl;
		bSelected = false;
	}
	return false;
}

void GuiButton::update(string _subName, Subject* _sub){
}

void GuiButton::draw(){
    if(haveImage){
        if(!drawActive){
            //if(haveArabic && SceneManager::Instance()->getDisplayArabic()){
             //   arabic.draw(pos.x,pos.y);
            //} else {
                inactive.draw(drawPos.x,drawPos.y, scale * size.x,scale * size.y);
            //}
        } else {
            active.draw(drawPos.x,drawPos.y, size.x, size.y);
        }
    } else {
        //ofRect(pos.x, pos.y, size.x, size.y);
    }
}
