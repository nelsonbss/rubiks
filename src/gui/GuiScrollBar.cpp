#include "GuiScrollBar.h"

GuiScrollBar::GuiScrollBar(){
}

void GuiScrollBar::nodeInit(){
	addParam("drag","true");
	addParam("n","1");
	scrollBar.loadImage("Exports/Scrollbar_Slider_279x110.png");
	arrowUp.loadImage("Exports/Scrollbar_TopArrow_273x94.png");
	arrowDown.loadImage("Exports/Scrollbar_BottomArrow_273x310.png");
	arrowUpPos.set(0,0);
	arrowDownPos.set(ofVec2f(0, 165));
	scrollBarPos.set(ofVec2f(7,15));
}

void GuiScrollBar::setBar(){
	barRect.x = drawPos.x + scrollBarPos.x - 20;
	barRect.y = drawPos.y + scrollBarPos.y;
	barRect.width = scrollBar.getWidth() + 40;
	barRect.height = scrollBar.getHeight();
}

void GuiScrollBar::nodeDraw(){
	arrowUp.draw(drawPos + arrowUpPos);
	scrollBar.draw(drawPos + scrollBarPos);
	arrowDown.draw(drawPos + arrowDownPos);
}

bool GuiScrollBar::isInside(int _x, int _y){
    //cout << name << " checking insides " << drawPos.x << ", " << drawPos.x + (scale * drawSize.x) << " - " << drawPos.y << ", " << drawPos.y + (scale * drawSize.y);
	//cout << " against " << _x << ", " << _y << endl;
    //cout << "ScrollBar checking - " << barRect.x << ", " << barRect.y << " - " << barRect.x + barRect.width << ", " << barRect.y + barRect.height << endl;
	if((_x > barRect.x && _x < barRect.x + barRect.width &&
       (_y > barRect.y && _y < barRect.y + barRect.height))){
		   return true;
       }
    return false;
}

void GuiScrollBar::input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){
	//cout << "Type = " << _type << " dX, dY = " << _deltaPos.x << ", " << _deltaPos.y << endl;
	if(_type == "drag"){
		SubObEvent ev;
		ev.setName(target + ":scroll");
		ev.addArg("amount", _deltaPos.y);
		SubObMediator::Instance()->sendEvent(ev.getName(), ev);
	}
	sendInteraction();
}