#ifndef __Tarmature__armature__
#define __Tarmature__armature__

#include "ofMain.h"

class armature {
	
public:
    armature (ofVec3f cntr, float w, float h, float d, float bs);
    
	void setup();
	void update();
	void draw();
    
    void togglePlanes();
    bool btogglePlanes;
    
    ofVec3f center;
    float width;
    float height;
    float depth;
    float blockSide;
    float side;
    float degX;
    float degY;
    float degZ;
    
    int mode;
    
    
    ofPlanePrimitive plane1;
    ofPlanePrimitive plane2;
    ofPlanePrimitive plane3;
    ofPlanePrimitive plane4;
    ofPlanePrimitive plane5;
    ofPlanePrimitive plane6;
    
    ofBoxPrimitive box1;
    ofBoxPrimitive box2;
    ofBoxPrimitive box3;
    ofBoxPrimitive box4;
    ofBoxPrimitive box5;
    ofBoxPrimitive box6;
    
    ofCylinderPrimitive cylinder1;
    ofCylinderPrimitive cylinder2;
    ofCylinderPrimitive cylinder3;
    
    // place to store the sides of the box //
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofVboMesh deformPlane;
    ofVboMesh topCap, bottomCap, body;
    vector<ofMeshFace> triangles;
    
    
};

#endif /* defined(__Tarmature__armature__) */
