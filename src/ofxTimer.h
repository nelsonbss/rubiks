#ifndef OFXTIMER_H
#define OFXTIMER_H

#include "ofMain.h"

class timerContainer{
public:
	timerContainer(int _duration, int* _target, int _val);
	int startTime;
	int duration;
	int* target;
	int val;
};

class ofxTimer: public ofThread{

public:
	ofxTimer();
	~ofxTimer(){}
	void start(){startThread(true, false);}
	void stop(){stopThread();}
	void threadedFunction();
	void addTimer(int _duration, int* _target, int _val);

private:
	vector<timerContainer> tContainers;
	bool bHaveNewTimer;
	int newDuration;
	int* newTarget;
	int newVal;
	int newStartTime;
};

#endif