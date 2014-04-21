#include "GuiDropArea.h"

GuiDropArea::GuiDropArea(){
	SubObMediator::Instance()->addObserver("object-moved", this);
}

void GuiDropArea::update(string _eventName, SubObEvent* _event){
	if(_eventName == "object-moved"){
		cout << "got object moved" << endl;
		ofVec2f objPos = _event->getArg("position")->getVec2();
		if(isInside(objPos.x, objPos.y)){
			cout << "DROP AREA ACTIVATED." << endl;
			SubObEvent *ev = new SubObEvent();
			ev->setName("object-intercepted");
			ev->addArg("object-name", _event->getArg("object-name")->getString());
			SubObMediator::Instance()->sendEvent(ev->getName(), ev);
			delete ev;
		}
	}
}