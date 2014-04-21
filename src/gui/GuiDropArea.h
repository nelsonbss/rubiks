#include "ofMain.h"
#include "GuiNode.h"

class GuiDropArea: public GuiNode{

public:
	GuiDropArea();
	virtual ~GuiDropArea(){}
	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent* _event);

private:

};