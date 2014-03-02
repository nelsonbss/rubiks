#include "GuiNode.h"
#include "ofMain.h"

class GuiIBox: public GuiNode{

public:
	GuiIBox();
	virtual ~GuiIBox(){}
	void nodeInit();
	void nodeDraw(); 
	void dragInput(int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);
	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent* _event){}
private:
	vector<DragData*> drags;
	bool bDrawData;
	ofTrueTypeFont font;
};