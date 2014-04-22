#include "GuiWindow.h"

void GuiWindow::nodeInit(){
	bHaveImage = false;
	if(params.count("image")){
		img.loadImage(params["image"]);
		bHaveImage = true;
	}
	bToggler = false;
	if(params.count("toggler")){
		if(params["toggler"] == "true"){
			bToggler = true;
			bToggled = false;
			SubObMediator::Instance()->addObserver(prefix + ":toggle", this);
			for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
				(*nIter)->setReadyForInput(bToggled);
			}
		}
	}
	for(int i = 0; i < nodes.size(); i++){
		nodePositions.push_back(ofVec2f(0.0,0.0));
	}
	bScrollable = false;
	currentTop = 0;
	if(bMirrored){
		drawPos.x = drawPos.x - drawSize.x;
	}
	if(bFlipped){
		drawPos.y = drawPos.y - drawSize.y;
	}
	calculateArea();
	positionNodes();
	if(bScrollable){
		scrollBar.setName(getName() + "-scroll");
		scrollBar.setPrefix(prefix);
		scrollBar.setPages(pages);
		scrollBar.init();
		if(!bMirrored){
			scrollBar.setDrawPosition(drawPos + ofVec2f(263, 30));
		} else {
			scrollBar.setDrawPosition(drawPos + ofVec2f(15, 30));
		}
		scrollBar.setBar();
		scrollBar.setTarget(getName());
		scrollBar.activate();
	}
	SubObMediator::Instance()->addObserver(getName() + ":scroll", this);
	SubObMediator::Instance()->addObserver(prefix + ":object-released()", this);
	//offset.set(0,0);
	topMax = offset.y;
	topMin = drawSize.y - windowHeight;
}

void GuiWindow::nodeActivate(){
	for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->activate();
	}
	currentTop = 0;
	positionNodes();
}

void GuiWindow::nodeDeactivate(){
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->deactivate();
	}
	currentTop = 0;
}

void GuiWindow::nodeDraw(){
	if(!bToggler){
		if(bScrollable){
			scrollBar.draw();
		}
		//glScissor(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
		//glEnable(GL_SCISSOR_TEST);
		/*ofSetColor(0,0,0);
		ofNoFill();
		ofRect(scissorRect);*/
		ofPushView();
		ofViewport(scissorRect);
		ofSetupScreen();
		ofDisableDepthTest();
		ofEnableAlphaBlending();
		if(bHaveImage){
			img.draw(0,0);
		}
		int counter = 0;
		for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
			if(!((*nIter)->isSelected())){
				(*nIter)->draw(nodePositions[counter]);
			}
			counter++;
		}
		/*ofFill();
		ofSetColor(0,255,0);
		ofRect(drawPos.x, drawPos.y, 200, 200);*/
		//glDisable(GL_SCISSOR_TEST);
		ofEnableDepthTest();
		ofPopView();
		for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
			if((*nIter)->isSelected()){
				(*nIter)->draw();
			}
		}
	} else {
		if(bToggled){
			ofPushView();
			ofViewport(scissorRect);
			ofSetupScreen();
			ofDisableDepthTest();
			ofEnableAlphaBlending();
			if(bHaveImage){
				img.draw(0,0);
			}
			int counter = 0;
			for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
				if(!((*nIter)->isSelected())){
					(*nIter)->draw(nodePositions[counter]);
				}
				counter++;
			}
			ofEnableDepthTest();
			ofPopView();
			/*if(bHaveImage){
			img.draw(drawPos.x, drawPos.y);
			}
			for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
			if((*nIter)->isSelected()){
			(*nIter)->draw();
			}
			}*/
		}
	}
	//ofRect(scissorRect);
}

void GuiWindow::hide(){
	bHidden = true;
	bActive = false;
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->hide();
		(*nIter)->deactivate();
	}
	if(bScrollable){
		scrollBar.hide();
		scrollBar.deactivate();
	}
	//deactivate();
}

void GuiWindow::unhide(){
	bHidden = false;
	bActive = true;
	for(vector<GuiNode*>::iterator nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->unhide();
		(*nIter)->activate();
	}
	if(bScrollable){
		scrollBar.unhide();
		scrollBar.activate();
	}
	//activate();
}

void GuiWindow::calculateArea(){
	scissorRect.x = drawPos.x;
	scissorRect.y = drawPos.y;
	scissorRect.width = drawSize.x;
	scissorRect.height = drawSize.y;
}

void GuiWindow::positionNodes(){
	int numNodes = nodes.size();
	int row = 0;
	int column = 0;
	/*ofVec2f startPos = drawPos;
	ofVec2f addPos(0,0);
	if(bFlipped){
	startPos.y = drawPos.y + drawSize.y;
	addPos.y = drawSize.y;
	}*/
	for(int i = 0;i < numNodes; i++){
		if(i > 0){
			column = i % numColumns;
			row = i / numColumns;
		}
		float nodeX = column * (columnWidth * ofGetWidth());
		float nodeY = row * (columnHeight * ofGetHeight());
		ofVec2f nodePos = ofVec2f(0, currentTop) + ofVec2f(nodeX, nodeY);
		nodePos += offset;
		if(bMirrored){
			nodePos += ofVec2f(40,0);
		}
		ofVec2f nodePosD = nodePos + drawPos;
		ofVec2f nodePosF(nodePos.x / ofGetWidth(), nodePos.y / ofGetHeight());
		//cout << "setting node position to - " << nodePos.x << ", " << nodePos.y << endl;
		//nodes[i]->setPosition(nodePosF);
		nodes[i]->setDrawPosition(nodePosD);
		/*
		if((nodePosD.y < drawPos.y) || (nodePosD.y > (drawPos.y + drawSize.y))){
		if(nodes[i]->isActive()){
		nodes[i]->deactivate();
		}
		} else {
		if(!nodes[i]->isActive()){
		nodes[i]->activate();
		}
		}
		*/
		nodes[i]->setCustomArea(drawPos, drawSize);
		nodePositions[i].set(nodePos);
		nodes[i]->setControlled();
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
	if(_eventName == prefix + ":toggle"){
		//cout << prefix << " toggling." << endl;
		if(bToggler){
			bToggled = !bToggled;
			for(auto nIter = nodes.begin(); nIter != nodes.end(); nIter++){
				(*nIter)->setReadyForInput(bToggled);
			}
			//cout << "bToggled = " << bToggled << endl;
		}
	}
	if(_eventName == getName() + ":object-released"){
		positionNodes();
	}
}