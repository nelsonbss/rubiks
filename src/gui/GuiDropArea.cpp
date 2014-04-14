#include "GuiDropArea.h"

GuiDropArea::GuiDropArea(){
	SubObMediator::Instance()->addObserver("object-moved", this);
	SubObMediator::Instance()->addObserver("menupuzzle-dropped", this);
}

void GuiDropArea::nodeInit(){
	if(bMirrored){
		drawPos.x -= drawSize.x;
	}
	if(bFlipped){
		drawPos.y -= drawSize.y;
	}
}

void GuiDropArea::update(string _eventName, SubObEvent _event){
	if(_eventName == "object-moved"){
		//cout << "got object moved" << endl;
		ofVec2f objPos = _event.getArg("position")->getVec2();
		if(isInside(objPos.x, objPos.y)){
			//cout << "DROP AREA ACTIVATED." << endl;
			SubObEvent ev; 
			ev.setName("object-intercepted");
			ev.addArg("object-name", _event.getArg("object-name")->getString());
			ev.addArg("intercepter", getName());
			SubObMediator::Instance()->sendEvent(ev.getName(), ev);
		}
	}
	if(_eventName == "menupuzzle-dropped"){
		cout << "got menupuzzle" << endl;
		ofVec2f objPos = _event.getArg("position")->getVec2();
		if(isInside(objPos.x, objPos.y)){
			//cout << "DROP AREA ACTIVATED." << endl;
			SubObEvent ev; 
			ev.setName("menupuzzle-intercepted");
			ev.addArg("object-name", _event.getArg("object-name")->getString());
			ev.addArg("intercepter", getName());
			SubObMediator::Instance()->sendEvent(ev.getName(), ev);
		}
	}
}