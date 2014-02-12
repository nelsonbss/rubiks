#ifndef __Tmyobject3D__myobject3D__
#define __Tmyobject3D__myobject3D__

#include "ofMain.h"
#include "sgCore.h"

class myobject3D {
	
public:
	myobject3D (float x,float y);
    
	void setup();
	void update();
	void draw();
	void exit();

	void loadObjectFromFile(const char* pathTofile); //it will load a stl file into object
	void loadObject(sgC3DObject *obj); //it will load a sgCore lib object: torus, box...
	sgCObject* getObject();//it will return the object, i.e to be used in boolean
	void unDraw();

	sgC3DObject *object;
	sgC3DObject *temp;//the original will be in 0,0,0 to be cut. the temp will be moved around and be drawn.
	ofMesh myMesh;// this will be used to draw the object, 
	void setNormals( ofMesh &mesh );
	float posX;
	float posY;
	float deg;

	string name; // may be usefull to call by first name
};

#endif /* defined(__Tmyobject3D__myobject3D__) */
