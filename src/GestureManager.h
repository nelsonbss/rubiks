#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include "ofMain.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObEvent.h"
#include "SubObMediator.h"
#include "GestureWorksCore.h"

class GestureManager : public ofThread, public Observer{

public:
	GestureManager();
	GestureManager(string _gwcDLL);
	//virtual ~GestureManager(){}
	void loadGMLFile(string _gml);
	bool isLoadedDLL(){return bLoadedDLL;}
	bool isLoadedGML(){return bLoadedGML;}
	bool isRegisteredWindow(){return bRegisteredWindow;}
	void init(string _window, int _width, int _height);
	void addObjectAndGestures(string _obj, vector<string>* _gestures);
	void addObjectToGesture(string _obj, string _gs);
	void removeObject(string _obj);

	void start(){startThread(true, false);}
	void stop(){stopThread();}
	
	void threadedFunction();

	void setFrameRate(int _rate){frameDelay = 1000 / _rate;}

	void update(string _eventName, SubObEvent _event);
	void update();

	SubObEvent gEvent;
	SubObEvent tEvent;

	std::map<int,gwc::Point> activePoints;

private:

	void getGestures();

	bool bLoadedDLL;
	bool bLoadedGML;
	bool bRegisteredWindow;

	
	int frameDelay;
	int timeOfLastUpdate;

	int receivers;
};

#endif