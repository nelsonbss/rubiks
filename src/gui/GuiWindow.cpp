#include "GuiWindow.h"

void GuiWindow::nodeInit(){
	bScrollable = false;
	currentTop = 0;
	calculateArea();
	positionNodes();
	if(bScrollable){
		scrollBar.setName(getName() + "-scroll");
		scrollBar.init();
		scrollBar.setDrawPosition(drawPos + ofVec2f(263, 30));
		scrollBar.setBar();
		scrollBar.setTarget(getName());
		scrollBar.activate();
	}
	SubObMediator::Instance()->addObserver(getName() + ":scroll", this);
	topMax = 0;
	topMin = drawSize.y - windowHeight;
}

void GuiWindow::nodeActivate(){
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->activate();
	}
	currentTop = 0;
}

void GuiWindow::nodeDeactivate(){
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->deactivate();
	}
	currentTop = 0;
}

void GuiWindow::nodeDraw(){
	ofDisableDepthTest();
	if(bScrollable){
		scrollBar.draw();
	}
	//glScissor(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
	//glEnable(GL_SCISSOR_TEST);
	//ofRect(scissorRect);
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->draw();
	}
	ofFill();
	ofSetColor(0,255,0);
	//ofRect(drawPos.x, drawPos.y, 200, 200);
	//glDisable(GL_SCISSOR_TEST);
	ofEnableDepthTest();
	//ofRect(scissorRect);
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
	scissorRect.x = drawPos.x;
	scissorRect.y = ofGetHeight() - drawPos.y;
	scissorRect.width = drawSize.x;
	scissorRect.height = drawSize.y;
}

void GuiWindow::positionNodes(){
	int numNodes = nodes.size();
	int row = 0;
	int column = 0;
	for(int i = 0;i < numNodes; i++){
		if(i > 0){
			column = i % numColumns;
			row = i / numColumns;
		}
		float nodeX = column * (columnWidth * ofGetWidth());
		float nodeY = row * (columnHeight * ofGetHeight());
		ofVec2f nodePos = (drawPos + ofVec2f(0, currentTop)) + ofVec2f(nodeX, nodeY);
		ofVec2f nodePosF(drawPos.x / ofGetWidth(), drawPos.y / ofGetHeight());
		//cout << "setting node position to - " << nodePos.x << ", " << nodePos.y << endl;
		nodes[i]->setPosition(nodePosF);
		nodes[i]->setDrawPosition(nodePos);
	}
	windowHeight = (row + 1) * (columnHeight * ofGetHeight());
	//cout << "Window height = " << windowHeight << endl;
	if(windowHeight > drawSize.y){
		bScrollable = true;
	}
}

void GuiWindow::update(string _eventName, SubObEvent _event){
	if(_eventName == getName() + ":scroll"){
		float amount = _event.getArg("amount")->getFloat();
		currentTop -= amount;
		if(currentTop > topMax){
			currentTop = topMax;
		} else if(currentTop < topMin){
			currentTop = topMin;
		}
		positionNodes();
	}
}