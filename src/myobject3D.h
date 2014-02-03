#ifndef __Tmyobject3D__myobject3D__
#define __Tmyobject3D__myobject3D__

#include "ofMain.h"
#include "sgCore.h"

class myobject3D {
	
public:
	myobject3D ();
    
	void setup();
	void update();
	void draw();

	void loadObjectFromFile(const char* pathTofile); //it will load a stl file into object
	void loadObject(sgCObject *obj); //it will load a sgCore lib object: torus, box...
	sgCObject* getObject();//it will return the object, i.e to be drawn
   
	sgCObject *object;
	string name; // may be usefull to call by first name
};

#endif /* defined(__Tmyobject3D__myobject3D__) */
