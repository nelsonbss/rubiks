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

void GuiIBox::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	if(_type == "select"){
		cout << "ibox selected" << endl;
		SubObEvent ev;
		string evName = name + "-tap";
		ev.setName(evName);
		ev.addArg("n",_n);
		ev.addArg("absPos",_absPos);
		ev.addArg("deltaPos",_deltaPos);
		SubObMediator::Instance()->sendEvent(evName, &ev);
	}
	if(_type == "drag"){
		cout << "ibox sending - " << _deltaPos.x << ", " << _deltaPos.y << endl;
		SubObEvent ev;
		string evName = name + ":" + ofToString(_n);
		ev.setName(evName);
		ev.addArg("n",_n);
		ev.addArg("absPos",_absPos);
		ev.addArg("deltaPos",_deltaPos);
		SubObMediator::Instance()->sendEvent(evName, &ev);
	}
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
