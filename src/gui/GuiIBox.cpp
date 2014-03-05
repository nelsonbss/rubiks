#include "GuiIBox.h"

GuiIBox::GuiIBox() : GuiNode(){
	drawColor.set(0,0,0);
	bDrawArea = false;
}

void GuiIBox::nodeInit(){
	if(params.count("draw-data")){
		if(params["draw-data"] == "true"){
			bDrawData = true;
			font.loadFont("verdana.ttf", true, true);
		}
	}
}

void GuiIBox::dragInput(int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	cout << "ibox - " << _absPos.x << ", " << _absPos.y << endl;
	SubObEvent* ev = new SubObEvent();
	string evName = name + ":" + ofToString(_n);
	ev->setName(evName);
	ev->addArg("n",_n);
	ev->addArg("absPos",_absPos);
	ev->addArg("deltaPos",_deltaPos);
	SubObMediator::Instance()->sendEvent(evName, ev);
	/*
	for(vector<DragData*>:: iterator dIter = drags.begin(); dIter != drags.end();){
		if((*dIter)->getID() == _ID){
			if(_phase == 4){
				dIter = drags.erase(dIter);
				return;
			} else {
				(*dIter)->setAPos(_absPos);
				(*dIter)->setDPos(_deltaPos);
				return;
			}
		}
		++dIter;
	}
	drags.push_back(new DragData(_ID, _n, _phase, _absPos, _deltaPos));
	*/
}

void GuiIBox::nodeDraw(){
	if(bDrawData){
		ofVec2f fontPos = drawPos + 10;
		for(vector<DragData*>::iterator dIter = drags.begin(); dIter != drags.end();){
			stringstream ss;
			ss << "Drag - " << (*dIter)->getN() << " points. Abs = " << (*dIter)->getAPos().x << ", " << (*dIter)->getAPos().y << ". Delta = " << (*dIter)->getDPos().x << ", " << (*dIter)->getDPos().y << endl;
			font.drawString(ss.str(), fontPos.x, fontPos.y);
			fontPos.x += 20;
		}
	}
}
