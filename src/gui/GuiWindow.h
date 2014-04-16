#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include "ofMain.h"
#include "GuiNode.h"
#include "GuiButton.h"
#include "GuiScrollBar.h"

class GuiWindow: public GuiNode{

public:
	GuiWindow(){}
	virtual void nodeInit();
	virtual void nodeDraw();
	void addNode(GuiNode* _node){nodes.push_back(_node);}
	bool processMouse(int _x, int _y, int _state){return false;}
	virtual void update(string _subName, Subject* _sub){}
	virtual void update(string _eventName, SubObEvent _event);
	void setNumColumns(int _n){numColumns = _n;}
	void setColumnWidth(float _n){columnWidth = _n;}
	void setColumnHeight(float _n){columnHeight = _n;}
	void nodeActivate();
	void nodeDeactivate();
	void calculateArea();
	void positionNodes();
	void hide();
	void unhide();

private:
	vector<GuiNode*> nodes;
	vector<ofVec2f> nodePositions;
	ofRectangle drawWindow;

	GuiScrollBar scrollBar;
	GuiButton scrollButton;

	ofVec2f scrollPos;
	float currentScroll;
	bool bScrollable;

	int numColumns;
	float columnWidth;
	float columnHeight;

	ofRectangle scissorRect;

	int currentTop;
	int windowHeight;
	int movementDiff;

	int topMin;
	int topMax;
};

#endif