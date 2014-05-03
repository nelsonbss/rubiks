//#ifndef __Tarmature__armature__
//#define __Tarmature__armature__
//
//#include "ofMain.h"
//#include "SubObMediator.h"
//#include "SubObEvent.h"
//#include "Observer.h"
//
//class armature : public Observer{
//	
//public:
//    armature (ofVec3f cntr, float w, float h, float d, float bs);
//    
//	void setup();
//	void update();
//	void draw();
//    
//    void togglePlanes();
//    bool btogglePlanes;
//    
//    ofVec3f center;
//	void moveA(ofVec3f input);
//	void rotateA(ofVec3f input);
//
//	void update(string _eventName, SubObEvent* _event);
//	ofVec2f lastInput;
//
//    float width;
//    float height;
//    float depth;
//    float blockSide;
//    float side;
//    ofVec3f rot;
//    
////    int mode;
//
//    ofPlanePrimitive plane1;
//    ofPlanePrimitive plane2;
//    ofPlanePrimitive plane3;
//    ofPlanePrimitive plane4;
//    ofPlanePrimitive plane5;
//    ofPlanePrimitive plane6;
//    
//    ofBoxPrimitive box1;
//    ofBoxPrimitive box2;
//    ofBoxPrimitive box3;
//    ofBoxPrimitive box4;
//    ofBoxPrimitive box5;
//    ofBoxPrimitive box6;
//    
//    ofCylinderPrimitive cylinder1;
//    ofCylinderPrimitive cylinder2;
//    ofCylinderPrimitive cylinder3;
//};
//
//#endif /* defined(__Tarmature__armature__) */
#ifndef __Tarmature__armature__
#define __Tarmature__armature__

#include "ofMain.h"
#include "SubObMediator.h"
#include "SubObEvent.h"
#include "Observer.h"

class armature : public Observer{
	
public:
    armature (ofVec3f cntr, float w, float h, float d, float bs, int gSize);
    
	void setup();
	void update();
	void draw();
    
    //void togglePlanes();
    //bool btogglePlanes;
    
    ofVec3f center;
	void moveA(ofVec3f input);
	void rotateA(ofVec3f input);

	void update(string _eventName, SubObEvent* _event);
	ofVec2f lastInput;

    float width;
    float height;
    float depth;
    float blockSide;
    float side;
    ofVec3f rot;
    

	ofCylinderPrimitive cylinderA;
    ofCylinderPrimitive cylinderB;
    ofCylinderPrimitive cylinderC;

	vector<ofCylinderPrimitive> cylinders;

	int numCylinders;
	int gridSize;
};

#endif /* defined(__Tarmature__armature__) */