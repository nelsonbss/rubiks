#include "GuiWindow.h"

void GuiWindow::nodeInit(){
	if(getParam("scrollable") == "true"){
		bScrollable = true;
		scrollBar.loadImage("Exports/Scrollbar_Slider_279x110.png");
		scrollArrowT.loadImage("Exports/Scrollbar_TopArrow_273x94.png");
		scrollArrowB.loadImage("Exports/Scrollbar_BottomArrow_273x310.png");
	} else {
		bScrollable = false;
	}
}

void GuiWindow::nodeActivate(){
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->activate();
	}
}

void GuiWindow::nodeDeactivate(){
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->deactivate();
	}
}

void GuiWindow::nodeDraw(){
	if(bScrollable){
		scrollArrowT.draw(drawPos.x + 263, drawPos.y + 30);
		scrollBar.draw(drawPos.x + 270, drawPos.y + 45);
		scrollArrowB.draw(drawPos.x + 263, drawPos.y + 195);
	}
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->draw();
	}
}

void GuiWindow::hide(){
	bHidden = true;
	bActive = false;
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->hide();
	}
}

void GuiWindow::unhide(){
	bHidden = false;
	bActive = true;
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->unhide();
	}
}

void GuiWindow::calculateArea(){
	
}