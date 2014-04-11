#include "GestureManager.h"
#include "GuiConfigurator.h"

GestureManager::GestureManager(){
	bLoadedDLL = true;
	bLoadedGML = false;
	if(loadGestureWorks("GestureworksCore32.dll")) { 
		std::cout << "Error loading gestureworks dll" << std::endl;  
		bLoadedDLL = false;
	}
	setFrameRate(30);
	timeOfLastUpdate = 0;
	SubObMediator::Instance()->addObserver("add-gesture-receiver",this);
	SubObMediator::Instance()->addObserver("add-object",this);
	SubObMediator::Instance()->addObserver("add-gesture",this);
	SubObMediator::Instance()->addObserver("add-object-gestures",this);
	SubObMediator::Instance()->addObserver("remove-object",this);
	SubObMediator::Instance()->addObserver("add-touch-point",this);
	SubObMediator::Instance()->addObserver("update-touch-point",this);
	SubObMediator::Instance()->addObserver("remove-touch-point",this);
	SubObMediator::Instance()->addObserver("assign-touch-point",this);
	receivers = 0;
}

GestureManager::GestureManager(string _gwcDLL){
	bLoadedDLL = true;
	bLoadedGML = false;
	if(loadGestureWorks(_gwcDLL)) { 
		std::cout << "Error loading gestureworks dll" << std::endl; 
		bLoadedDLL = false;
	}
}

void GestureManager::loadGMLFile(string _gml){
	bLoadedGML = true;
	if(!loadGML(_gml)) { 
		std::cout << "Could not find gml file" << std::endl; 
		bLoadedGML = false;
	}
}

void GestureManager::init(string _window, int _width, int _height){
	initializeGestureWorks(_width,_height);
	bRegisteredWindow = true;
	if(!registerWindowForTouchByName(_window)) { 
		std::cout << "Could not register target window for touch." << std::endl; 
		bRegisteredWindow = false;
	}
}

void GestureManager::addObjectAndGestures(string _obj, vector<string>* _gestures){
	registerTouchObject(_obj);
	for(vector<string>::iterator gIter = _gestures->begin(); gIter != _gestures->end(); ++gIter){
		addGesture(_obj, *gIter);
	}
}

void GestureManager::addObjectToGesture(string _obj, string _gs){
	addGesture(_obj, _gs);
}

void GestureManager::removeObject(string _obj){
	deregisterTouchObject(_obj);
}

void GestureManager::threadedFunction(){
	while(isThreadRunning() != 0){
		if(ofGetElapsedTimeMillis() - timeOfLastUpdate > frameDelay){
			getGestures();
			timeOfLastUpdate = ofGetElapsedTimeMillis();
		}
		ofSleepMillis(100);
	}
}	

void GestureManager::update(){
	//if(ofGetElapsedTimeMillis() - timeOfLastUpdate > frameDelay){
	getGestures();
	//	timeOfLastUpdate = ofGetElapsedTimeMillis();
	//}
}

void GestureManager::update(string _eventName, SubObEvent _event){
	SubObEvent ev = _event;
	if(_eventName == "add-gesture-receiver"){
		cout << "adding gesture receiver." << endl;
		receivers += 1;
	}
	if(_eventName == "add-object"){
		string objName = ev.getArg("objName")->getString();
		registerTouchObject(objName);
		//cout << "registered " << objName << endl;
	}
	if(_eventName == "add-gesture"){
		string objName = ev.getArg("objName")->getString();
		string gesture = ev.getArg("gesture")->getString();
		addGesture(objName, gesture);
		//cout << "added gesture " << gesture << " to " << objName << endl;
	}
	if(_eventName == "remove-object"){
		string objName = ev.getArg("objName")->getString();
		deregisterTouchObject(objName);
	}
	if(_eventName == "add-touch-point"){
		ofVec3f pos = ev.getArg("position")->getVec3();
		int tId = ev.getArg("touch-id")->getInt();
		gwc::touchpoint nPoint;
		nPoint.init(tId, pos.x, pos.y, 0, 1, 1);
		nPoint.status = gwc::TOUCHADDED;
		addEvent(nPoint);
		//cout << "adding touch point - " << tId << endl;
	}
	if(_eventName == "update-touch-point"){
		ofVec3f pos = ev.getArg("position")->getVec3();
		int tId = ev.getArg("touch-id")->getInt();
		gwc::touchpoint nPoint;
		nPoint.init(tId, pos.x, pos.y, 0, 1, 1);
		nPoint.status = gwc::TOUCHUPDATE;
		addEvent(nPoint);
		//cout << "updating touch point - " << tId << endl;
	}
	if(_eventName == "remove-touch-point"){
		ofVec3f pos = ev.getArg("position")->getVec3();
		int tId = ev.getArg("touch-id")->getInt();
		gwc::touchpoint nPoint;
		nPoint.init(tId, pos.x, pos.y, 0, 1, 1);
		nPoint.status = gwc::TOUCHREMOVED;
		addEvent(nPoint);
		//cout << "removing touch point - " << tId << endl;
	}
	if(_eventName == "assign-touch-point"){
		int id = ev.getArg("id")->getInt();
		string target = ev.getArg("target")->getString();
		assignTouchPoint(target, id);
		cout << "assigning " << id << " to " << target << endl;
	}
}

void GestureManager::getGestures(){
	//cout << "gs running." << endl;
	processFrame();
	std::vector<gwc::PointEvent> pointEvents = consumePointEvents();
	//cout << pointEvents.size() << endl;
	for(auto eIter = pointEvents.begin(); eIter != pointEvents.end(); eIter++) {
		//cout << "Touch Point = " << event_it->point_id << endl;
		switch(eIter->status) {
			case gwc::TOUCHADDED:
				{
				//assignTouchPoint("touchReceiver", event_it->point_id);
				cout << "new touch point - " << eIter->position.getX() << ", " << eIter->position.getY() << endl;
				SubObEvent e;
				e.setName("touch-point");
				e.setActive(true);
				e.addArg("hit",0);
				e.addArg("receivers",0);
				e.addArg("id", eIter->point_id);
				e.addArg("point", ofVec2f(eIter->position.getX(), eIter->position.getY()));
				e.addArg("x",eIter->position.getX());
				e.addArg("y",eIter->position.getY());
				//cout << "sending touch event" << endl;
				SubObMediator::Instance()->sendEvent("touch-point",e);
				//while(e.getArg("receivers")->getInt() < receivers){}
				//cout << "hit = " << e->getArg("hit")->getInt() << endl;
				/*
				if(e.getArg("hit")->getInt()){
					if(e.hasArg("target")){
						cout << "assigning " << e.getArg("target")->getString() << " to gesture" << endl;
						assignTouchPoint(e.getArg("target")->getString(), eIter->point_id);
					}
				}
				*/
				}
				break;
			case gwc::TOUCHUPDATE:
				break;
			case gwc::TOUCHREMOVED:
				break;
			default:
				break;
		}
	}
	std::vector<gwc::GestureEvent> gestureEvents = consumeGestureEvents();
	for(auto gIter = gestureEvents.begin(); gIter != gestureEvents.end(); gIter++) {
		cout << gIter->gesture_type << " - " << gIter->x << ", " << gIter->y << " - " << gIter->phase << endl;
		if(gIter->gesture_type == "tap"){
			cout << "tap to " << gIter->target << endl;
		}
		//SubObEvent gEvent;
		gEvent.setName("gesture");
		gEvent.addArg("ID", gIter->ID);
		gEvent.addArg("absPos", ofVec2f(gIter->x, gIter->y));
		gEvent.addArg("deltaPos", ofVec2f(gIter->values["drag_dx"], gIter->values["drag_dy"]));
		gEvent.addArg("type", gIter->gesture_type);
		gEvent.addArg("target",gIter->target);
		gEvent.addArg("phase",gIter->phase);
		gEvent.addArg("id", gIter->gesture_id);
		gEvent.addArg("n", gIter->n);
		//if(gIter->phase < 3){
		SubObMediator::Instance()->sendEvent("gesture", gEvent);
		//}
	}
}