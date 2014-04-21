#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include "ofMain.h"
#include "GuiNode.h"
#include "GuiButton.h"

class GuiWindow: public GuiNode{

public:
	GuiWindow(){}
	virtual void nodeInit();
	virtual void nodeDraw();
	void addNode(GuiNode* _node){nodes.push_back(_node);}
	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent* _event){}
	void nodeActivate();
	void nodeDeactivate();
	void calculateArea();
	void hide();
	void unhide();

private:
	vector<GuiNode*> nodes;
	ofRectangle drawWindow;

	ofImage scrollBar;
	ofImage scrollArrowT;
	ofImage scrollArrowB;
	GuiButton scrollButton;

	ofVec2f scrollPos;
	float currentScroll;
	bool bScrollable;
};

#endif