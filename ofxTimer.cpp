#include "ofxTimer.h"

timerContainer::timerContainer(int _duration, int* _target, int _val){
	duration = _duration;
	target = _target;
	val = _val;
	startTime = ofGetElapsedTimeMillis();
}

ofxTimer::ofxTimer(){
	start();
}

void ofxTimer::threadedFunction(){
	while(isThreadRunning()){
		if(lock()){
			int now = ofGetElapsedTimeMillis();
			for(vector<timerContainer>::iterator tIter = tContainers.begin(); tIter != tContainers.end();){
				if(now - tIter->startTime > tIter->duration){
					*tIter->target = tIter->val;
					tIter = tContainers.erase(tIter);
				} else {
					++tIter;
				}
			}
		}
	}
}

void ofxTimer::addTimer(int _duration, int* _target, int _val){
	tContainers.push_back(timerContainer(_duration, _target, _val));
}
