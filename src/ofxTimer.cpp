#include "ofxTimer.h"

timerContainer::timerContainer(int _duration, int* _target, int _val){
	duration = _duration;
	target = _target;
	val = _val;
	startTime = ofGetElapsedTimeMillis();
}

ofxTimer::ofxTimer(){
	bHaveNewTimer = false;
	start();
}

void ofxTimer::threadedFunction(){
	while(isThreadRunning()){
		if(lock()){
 			for(vector<timerContainer>::iterator tIter = tContainers.begin(); tIter != tContainers.end();){
				int now = ofGetElapsedTimeMillis();
				if(now - tIter->startTime > tIter->duration){
					*tIter->target = tIter->val;
					tIter = tContainers.erase(tIter);
					//cout << "timer size = " << tContainers.size() << endl;
				} else {
					++tIter;
				}
			}
			if(bHaveNewTimer){
				tContainers.push_back(timerContainer(newDuration, newTarget, newVal));
				bHaveNewTimer = false;
			}
		}
		ofSleepMillis(100);
	}
}

void ofxTimer::addTimer(int _duration, int* _target, int _val){
	//tContainers.push_back(timerContainer(_duration, _target, _val));
	//cout << "adding timer - " << _duration << endl;
	while(bHaveNewTimer){}
	newDuration = _duration;
	newTarget = _target;
	newVal = _val;
	newStartTime = ofGetElapsedTimeMillis();
	bHaveNewTimer = true;
}
