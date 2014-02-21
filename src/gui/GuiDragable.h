#ifndef GUIDRAGABLE_H
#define GUIBUTTON_H

#include "GuiNode.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "ofMain.h"

class GuiNode;

class GuiDragable : public GuiNode, public Subject, public Observer
{
public:
	GuiDragable(map<string,string> &_attrs);
	GuiDragable(string _img);
	virtual ~GuiDragable(){}
