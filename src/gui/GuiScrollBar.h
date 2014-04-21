#ifndef GUISCROLLBAR_H
#define GUISCROLLBAR_H

#include "ofMain.h"
#include "GuiNode.h"

enum{ELEMENT_ARROW_UP, ELEMENT_ARROW_DOWN, ELEMENT_BAR};

class GuiScrollBar : public GuiNode{
public:
	GuiScrollBar();
	virtual ~GuiScrollBar(){}
	bool processMouse(int _x, int _y, int _state){return false;}
	virtual bool isInside(int _x, int _y);
	virtual void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent _event){}
	virtual void nodeDraw();
	virtual void nodeInit();
	void setBar();
	void setTarget(string _t){target = _t;}

private:
	ofImage scrollBar;
	ofImage arrowUp;
	ofImage arrowDown;

	int currentElement;

	string target;

	ofVec2f arrowUpPos;
	ofVec2f arrowDownPos;
	ofVec2f scrollBarPos;

	ofRectangle barRect;
};

#endif