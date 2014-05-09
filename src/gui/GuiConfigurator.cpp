#include "GuiConfigurator.h"

GuiConfigurator* GuiConfigurator::mInstance = NULL;

GuiConfigurator::GuiConfigurator(){
	bFirstStart = true;
	SubObMediator::Instance()->addObserver("replace-sheet", this);
	availableGestures.push_back("n-drag");
	availableGestures.push_back("n-rotate");
	availableGestures.push_back("n-tap");
	SubObMediator::Instance()->addObserver("touch-point",this);
	SubObMediator::Instance()->addObserver("gesture",this);
	SubObMediator::Instance()->addObserver("hide-node", this);
	SubObMediator::Instance()->addObserver("unhide-node", this);
	SubObMediator::Instance()->addObserver("toggle-node", this);
	//SubObMediator::Instance()->sendEvent("add-gesture-receiver", dummy);
	setCurrentLanguage("english");
	prefix = "";
	memberEvents.insert("toggle-node");
}

GuiConfigurator* GuiConfigurator::Instance(){
	if(!mInstance){
		mInstance = new GuiConfigurator();
	}
	return mInstance;
}

void GuiConfigurator::addFile(string _file){
	mXML.loadFile(_file);
}

void GuiConfigurator::update(string _subName, Subject* _sub){
	if(_subName == "button"){
		string target = _sub->getAttr("target");
		string action = _sub->getAttr("action");
		string name = _sub->getAttr("name");
		cout << "got a GUI update from " << name << " for target " << target << " with action " << action << endl;
		if(action == "open"){
			openSheet(target);
		}
		if(action == "close"){
			cout << "closing sheet" << endl;
			closeSheet(target);
		}
		if(action == "replace"){
			replaceSheet(target);
		}
		if(action == "add"){
			closeSheet(target);
		}
		if(action == "reset"){
			SubObMediator::Instance()->update("reset", this);
			//MediaCabinet::Instance()->reset();
			bFirstStart = true;
		}
		/*
		if(name == "render-button"){
		SceneManager::Instance()->render();
		}
		*/
		/*
		if(name == "language"){
		openSheet("start");
		}
		*/
	}
	/*
	if(_subName == "clip-selected"){
	closeSheet("none");
	closeSheet("none");
	}
	*/
}

void GuiConfigurator::update(string _eventName, SubObEvent _event){
	//cout << "received event named - " << _eventName << endl;
	if(_eventName == "replace-sheet"){
		string target = _event.getArg("target")->getString();
		//cout << "replacing with " << target << endl;
		replaceSheet(target);
	}
	if(_eventName == "touch-point"){
		vector<GuiNode*> nodesHit;
		/*
		for(map<string, GuiNode*>::iterator nIter =  activeNodes.begin(); nIter != activeNodes.end(); ++nIter){
		//cout << "Testing - " << nIter->second->getName() << endl;
		if(nIter->second->isActive()){
		if(nIter->second->isInside(_event.getArg("x")->getFloat(), _event.getArg("y")->getFloat())){
		//_event->getArg("hit")->setInt(1);
		//_event->addArg("target",nIter->first);
		//break;
		nodesHit.push_back(nIter->second);
		}
		} else {
		//cout << "...Inactive" << endl;
		}
		}
		*/
		for(auto mIter = nodePages.begin(); mIter != nodePages.end(); mIter++){
			//cout << "Drawing nodes for " << mIter->first << endl;
			string page = currentPages[mIter->first];
			for(auto nIter = nodePages[mIter->first][page].begin(); nIter != nodePages[mIter->first][page].end(); nIter++){	
				//cout << "\tDrawing " << (*nIter)->getName() << endl;
				//cout << "Drawing " << (*nIter)->getName() << endl;
				if((*nIter)->isInside(_event.getArg("x")->getFloat(), _event.getArg("y")->getFloat())){
					nodesHit.push_back((*nIter));
				}	
			}
		}
		if(nodesHit.size()){
			//cout << "hit " << nodesHit.size() << " nodes." << endl;
			float highestZ = -100000.0;
			GuiNode* highestNode = NULL;
			for(vector<GuiNode*>::iterator nIter = nodesHit.begin(); nIter != nodesHit.end(); ++nIter){
				float nodeZ = (*nIter)->getZ();
				//cout << "checking node with z = " << nodeZ << endl;
				if(nodeZ > highestZ){
					//cout << "setting " << (*nIter)->getName() << " to highest z. Z = " << nodeZ << endl;
					highestNode = *nIter;
					highestZ = nodeZ;
				}
			}
			if(highestNode != NULL){
				//_event.getArg("hit")->setInt(1);
				//_event.addArg("target", highestNode->getName());
				SubObEvent ev;
				ev.addArg("target", highestNode->getName());
				ev.addArg("id", _event.getArg("id")->getInt());
				SubObMediator::Instance()->sendEvent("assign-touch-point", ev);
			}
		}
		_event.getArg("receivers")->setInt(_event.getArg("receivers")->getInt() + 1);
	}
	if(_eventName == "gesture"){
		string target = _event.getArg("target")->getString();
		string type = _event.getArg("type")->getString();
		//cout << "have " << type << " going to " << target << endl;
		if(activeNodes.count(target)){
			//cout << draggable << endl;
			int n = 1;
			string targetN = activeNodes[target]->getParam("n");
			int eventN = _event.getArg("n")->getInt();
			if(targetN != "__NONE__"){
				n = ofToInt(targetN);
			}
			//cout << "event n = " << eventN << " and target n = " << targetN << endl;
			if(activeNodes[target]->getParam(type) == "true"){
				if(eventN == n || n == -1){
					//cout << activeNodes[target]->getName() << " is getting " << type << endl;
					//activeNodes[target]->adjustPosition(_event->getArg("drag_d")->getVec2(), _event->getArg("position")->getVec2());
					//cout << "sending drag." << endl;
					int cID = _event.getArg("ID")->getInt();
					int cN = _event.getArg("n")->getInt();
					int cPhase = _event.getArg("phase")->getInt();
					ofVec2f cAPos = _event.getArg("absPos")->getVec2();
					ofVec2f cDPos = _event.getArg("deltaPos")->getVec2();
					if(activeNodes[target]->getReadyForInput()){
						activeNodes[target]->input(type, cID, cN, cPhase, cAPos, cDPos);
					} 
					//cout << "sent " << cAPos.x << ", " << cAPos.y << ". P = " << cPhase << endl;
					//cout << "sent " << cAPos.x << ", " << cAPos.y << ". P = " << cPhase << endl;
				} else {
					//cout << "n mismatch" << endl;
				}
			} else {
				//cout << "...but we're not draggable." << endl;
			}
		}
	}
	//if(_eventName == "hide-node"){
	//	string target = _event.getArg("target")->getString();
	//	if(activeNodes.count(target)){
	//		if(!activeNodes[target]->isHidden()){
	//			activeNodes[target]->hide();
	//		}
	//	} else {
	//		cout << "NODE_HIDE - don't have node - " << target << endl;
	//	}
	//}
	//if(_eventName == "unhide-node"){
	//	string target = _event.getArg("target")->getString();
	//	if(activeNodes.count(target)){
	//		if(activeNodes[target]->isHidden()){
	//			activeNodes[target]->unhide();
	//		}
	//	} else {
	//		cout << "NODE_UNHIDE - don't have node - " << target << endl;
	//	}
	//}
	if(_eventName == "toggle-node"){
		string target = _event.getArg("target")->getString();
		if(activeNodes.count(target)){
			if(activeNodes[target]->isHidden()){
				activeNodes[target]->unhide();
			} else {
				activeNodes[target]->hide();
			}
		}
	}
}

void GuiConfigurator::getTags(){
	mXML.pushTag("gui");
	int numSheets = mXML.getNumTags("sheet");
	cout << "have " << numSheets << " sheets." << endl;
	for(int i = 0; i < numSheets; i++){
		string sheetName = mXML.getAttribute("sheet","name","none",i);
		string sheetType = mXML.getAttribute("sheet","type","none",i);
		sheetTypes[sheetName] = sheetType;
		cout << "starting sheet - " << sheetName << endl;
		mXML.pushTag("sheet", i);
		int numNodes = mXML.getNumTags("node");
		cout << "have " << numNodes << " nodes." << endl;
		for(int j = 0; j < numNodes; j++){
			vector<string> attrs, vals;
			vector<SubObEvent*> events;
			mXML.getAttributeNames("node",attrs, j);
			string nodeName = mXML.getAttribute("node", "name", "none", j); 
			for(int k = 0; k < attrs.size(); k++){
				vals.push_back(mXML.getAttribute("node",attrs[k], "none", j));
			}
			mXML.pushTag("node",j);
			string pos = mXML.getValue("pos", "none");
			string size = mXML.getValue("size", "none");
			string scale = mXML.getValue("scale","1.0");
			int numEvents = mXML.getNumTags("event");
			vector<SubObEvent*> tmpEvents;
			for(int k = 0; k < numEvents; k++){
				SubObEvent *tmpEvent = new SubObEvent();
				string eventName = mXML.getAttribute("event", "name", "none", k);
				//cout << "adding event - " << eventName << endl;
				tmpEvent->setName(eventName);
				mXML.pushTag("event", k);
				int numArgs = mXML.getNumTags("arg");
				for(int args = 0; args < numArgs; args++){
					string argName = mXML.getAttribute("arg", "name", "none", args);
					mXML.pushTag("arg", args);
					string type = mXML.getValue("type", "string");
					string val = mXML.getValue("val","none");
					tmpEvent->addArg(argName, type, val);
					mXML.popTag();
				}
				mXML.popTag();
				tmpEvents.push_back(tmpEvent);
			}
			nodeEvents[nodeName] = tmpEvents;
			attrs.push_back("pos");
			vals.push_back(pos);
			attrs.push_back("size");
			vals.push_back(size);
			attrs.push_back("scale");
			vals.push_back(scale);
			mXML.popTag();
			addAttributeMap(sheetName, attrs, vals);
		}
		mXML.popTag();
	}
	mXML.popTag();
}

void GuiConfigurator::loadGui(bool _mirrored, bool _flipped){
	bMirrored = _mirrored;
	bFlipped = _flipped;
	mXML.pushTag("gui");
	string main = mXML.getValue("main-sheet","attract");
	loadSheets();
	SceneManager::Instance()->pushSheet(sheets[main]);
	//loadText("assets.xml");
}

void GuiConfigurator::extendGui(string _sheet, string _file, bool _mirrored, bool _flipped, string _prefix, map<string, string> _patterns){
	bMirrored = _mirrored;
	bFlipped = _flipped;
	prefix = _prefix;
	patterns = _patterns;
	mXML.loadFile(_file);
	mXML.pushTag("gui");
	loadNodes(_sheet, NULL);
	mXML.popTag();
	if(SceneManager::Instance()->getTopSheet()->getName() == _sheet){
		SceneManager::Instance()->popSheet();
		SceneManager::Instance()->pushSheet(sheets[_sheet]);
	}
}

void GuiConfigurator::loadText(string _file){
	/*
	mXML.loadFile(_file);
	mXML.pushTag("texts");
	int numTexts = mXML.getNumTags("text");
	cout << "have " << numTexts << " texts" << endl;
	for(int i = 0; i < numTexts; i++){
	mXML.pushTag("text", i);
	string name = mXML.getValue("name", "none");
	cout << "loading string - " << name << endl;
	if(name != "none"){
	int numVals = mXML.getNumTags("value");
	if(!texts.count(name)){
	texts[name] = GuiText();
	}
	for(int j = 0; j < numVals; j++){
	string lang = mXML.getAttribute("value", "language", "none", j);
	string text = mXML.getValue("value", "none", j);
	ofStringReplace(text, "\\n", "\n");
	//ofStringReplace(text, "\\u", "\u");
	//string text = mXML.getAttribute("value", "val", "none", j);
	texts[name].setText(lang, text);
	cout << "loading text - " << text << endl;
	}
	}
	mXML.popTag();
	}
	*/
	/*
	lXML.load(_file);
	lXML.setTo("texts");
	int numTexts = lXML.getNumChildren("text");
	cout << "have " << numTexts << " texts" << endl;
	for(int i = 0; i < numTexts; i++){
	lXML.setToChild(i);
	string name = lXML.getValue<string>("name", "none");
	cout << "loading text - " << name << endl;
	if(name != "none"){
	int numVals = lXML.getNumChildren("value");
	if(!texts.count(name)){
	texts[name] = GuiText();
	}
	for(int j = 0; j < numVals; j++){
	//lXML.setTo("text[" + ofToString(i) + "]");
	string lang = lXML.getAttribute("value[" + ofToString(j) + "][@language]");
	string text = lXML.getValue<string>("value[" + ofToString(j) + "]", "none");
	texts[name].setText(lang, text);
	cout << "loading text - " << text << " for lang - " << lang << endl;
	}
	}
	lXML.setToParent();
	}
	*/
	specialTextLoadingCopOut();
}

void GuiConfigurator::specialTextLoadingCopOut(){

	texts["english"] = GuiText();
	texts["english"].setText("english", "English");
	texts["english"].setText("french", "English");
	texts["english"].setText("spanish", "English");

	texts["french"] = GuiText();
	texts["french"].setText("english", "Français");
	texts["french"].setText("french", "Français");
	texts["french"].setText("spanish", "Français");

	texts["spanish"] = GuiText();
	texts["spanish"].setText("english", "Español");
	texts["spanish"].setText("french", "Español");
	texts["spanish"].setText("spanish", "Español");

	texts["save"] = GuiText();
	texts["save"].setText("english","SAVE");
	texts["save"].setText("french","ENREGISTER");
	texts["save"].setText("spanish","GUARDAR");

	texts["solve"] = GuiText();
	texts["solve"].setText("english","SOLVE");
	texts["solve"].setText("french","RÉSOUDRE");
	texts["solve"].setText("spanish","RESOLVER");

	texts["next"] = GuiText();
	texts["next"].setText("english", "NEXT");
	texts["next"].setText("french", "SUIVANT");
	texts["next"].setText("spanish", "SIGUIENTE");

	texts["reset"] = GuiText();
	texts["reset"].setText("english", "START OVER");
	texts["reset"].setText("french", "RECOMMENCER");
	texts["reset"].setText("spanish", "REINICIAR");

	texts["make"] = GuiText();
	texts["make"].setText("english", "MAKE ONE");
	texts["make"].setText("french", "CRÉER");
	texts["make"].setText("spanish", "HAZ UNO");

	texts["color"] = GuiText();
	texts["color"].setText("english", "Drag colors to\ndecorate your puzzle.");
	texts["color"].setText("french", "Faites glisser les\ncouleurs pour décorer\nvotre puzzle.");
	texts["color"].setText("spanish", "Para decorar tu puzle,\narrastra los colores.");

	texts["arm-start"] = GuiText();
	texts["arm-start"].setText("english", "Drag a structure\nto slice your shape.");
	texts["arm-start"].setText("french", "Faites glisser une\nstructure pour découper\nvotre forme.");
	texts["arm-start"].setText("spanish", "Para cortar tu forma,\narrastra una estructura.");

	texts["arm-rotate-1"] = GuiText();
	texts["arm-rotate-1"].setText("english", "Single finger swipe\nmoves structure.");
	texts["arm-rotate-1"].setText("french", "Glissez avec un doigt\npour déplacer la\nstructure.");
	texts["arm-rotate-1"].setText("spanish", "Para girar la estructura,\ndeslízala con un\nsolo dedo.");

	texts["arm-rotate-2"] = GuiText();
	texts["arm-rotate-2"].setText("english", "Two-finger swipe\nrotates structure.");
	texts["arm-rotate-2"].setText("french", "Glissez avec deux doigts\npour faire tourner\nla structure.");
	texts["arm-rotate-2"].setText("spanish", "Para rotar la estructura,\ndeslízala con dos\ndedos.");

	texts["draw-help"] = GuiText();
	texts["draw-help"].setText("english", "Draw an outline with \nyour finger to make\nyour own shape.");
	texts["draw-help"].setText("french", "Dessinez un contour\ndu doigt pour créer\nvotre propre forme.");
	texts["draw-help"].setText("spanish", "Para crear tu propia figura,\ndibuja un contorno\ncon el dedo.");

	texts["start"] = GuiText();
	texts["start"].setText("english", "Drag a shape to begin\nto make a puzzle like\nthe one you selected.");

	texts["play-help-1"] = GuiText();
	texts["play-help-1"].setText("english", "Single finger swipe\ntwists puzzle.");
	texts["play-help-1"].setText("french", "Un doigt = glisse/\ndéplace les pièces");
	texts["play-help-1"].setText("spanish", "Un solo dedo mueve o\ndesliza las piezas");

	texts["play-help-2"] = GuiText();
	texts["play-help-2"].setText("english", "Two finger swipe\nrotates puzzle.");
	texts["play-help-2"].setText("french", "Deux doigts = glisse/\ntourne le cube");
	texts["play-help-2"].setText("spanish", "Dos dedos deslizan,\narrastran o hacen\nrotar al cubo");

	texts["start-help"] = GuiText();
	texts["start-help"].setText("english", "Drag A Puzzle From\nAbove Or A Shape Here\nto Play");
	texts["start-help"].setText("french", "Faites glisser un puzzle\nci-dessus ou une forme ici\npour jouer");
	texts["start-help"].setText("spanish", "Para jugar, arrastra aquí una\nforma o un rompecabezas\ndesde arriba");
}

void GuiConfigurator::loadSheets(){
	prefix = "all";
	int numSheets = mXML.getNumTags("sheet");
	for(int i = 0; i < numSheets; i++){
		string sheetName = mXML.getAttribute("sheet", "name", "none", i);
		if(sheetName == "none"){
			continue;
		}
		sheets[sheetName] = new GuiSheet();
		sheets[sheetName]->setName(sheetName);
		mXML.pushTag("sheet", i);
		loadNodes(sheetName, NULL);
		mXML.popTag();
	}
}

void GuiConfigurator::loadNodes(string _sheetName, GuiWindow* _win){
	bool bSheet = true;
	if(_win != NULL){
		bSheet = false;
	}
	cout << "bSheet = " << bSheet << endl; 
	int numNodes = mXML.getNumTags("node");
	cout << "have " << numNodes << " nodes." << endl;
	for(int i = 0; i < numNodes; i++){
		string nodeName = mXML.getAttribute("node","name","none",i);
		if(nodeName == "none"){
			continue;
		}
		nodeName = prefix + ":" + nodeName;
		string nodeType = mXML.getAttribute("node","type","none",i);
		//cout << "have node of type - " << nodeType << endl;
		if(nodeType == "none"){
			continue;
		}
		GuiNode* nodePtr = NULL;
		if(nodeType == "button"){
			nodePtr = new GuiButton();
		} else if (nodeType == "ibox") {
			nodePtr = new GuiIBox();
		} else if (nodeType == "drop") {
			nodePtr = new GuiDropArea();
		} else if (nodeType == "window"){	
			nodePtr = new GuiWindow();
			mXML.pushTag("node",i);
			string path = mXML.getValue("directory", "none");
			if(path != "none"){
				loadNodesFromDirectory(path, (GuiWindow*)nodePtr);
			} else {
				loadNodes("", (GuiWindow*)nodePtr);
			}
			mXML.popTag();
		} else {
			continue;
		}
		if(nodePtr == NULL){
			continue;
		}
		nodePtr->setName(nodeName);
		mXML.pushTag("node", i);
		string pos = "0.0,0.0";
		if(!bMirrored){
			pos = mXML.getValue("pos", "0.0,0.0");
		} else {
			pos = mXML.getValue("pos-mirrored", "none");
			if(pos == "none"){
				pos = mXML.getValue("pos", "0.0,0.0");
			}
		}
		string size = mXML.getValue("size", "150.0,150.0");
		string scale = mXML.getValue("scale","1.0");
		string pages = mXML.getValue("pages","none");
		if(nodeType == "window"){
			int columns = mXML.getValue("columns",0);
			float columnWidth = mXML.getValue("column-width",0.0);
			float columnHeight = mXML.getValue("column-height", 0.0);
			string offset = mXML.getValue("offset", "0.0,0.0");
			GuiWindow* tWinPtr = (GuiWindow*)nodePtr;
			tWinPtr->setNumColumns(columns);
			tWinPtr->setColumnWidth(columnWidth);
			tWinPtr->setColumnHeight(columnHeight);
			tWinPtr->setOffset(stringToVec2f(offset));
		}
		ofVec2f posVec = stringToVec2f(pos);
		if(bMirrored){
			posVec.x = 1.0 - posVec.x;
		}
		if(bFlipped){
			posVec.y = 1.0 - posVec.y;
		}
		nodePtr->setPosition(posVec);
		nodePtr->setSize(stringToVec2f(size)); 
		nodePtr->setScale(ofToFloat(scale));
		nodePtr->setMirrored(bMirrored);
		nodePtr->setFlipped(bFlipped);
		nodePtr->setPrefix(prefix);
		nodePtr->setPages(pages);
		loadParams(nodePtr);
		loadEvents(nodePtr);
		nodePtr->init();
		if(bSheet){
			sheets[_sheetName]->addNode(nodePtr);
		} else {
			_win->addNode(nodePtr);
		}
		mXML.popTag();
	}
}

void GuiConfigurator::loadNodesFromDirectory(string _path, GuiWindow* _win){
	//cout << "Loading directory - " << _path << endl;
	string path = _path;
	for(auto pIter = patterns.begin(); pIter != patterns.end(); pIter++){
		ofStringReplace(path, pIter->first, pIter->second);
	}
	string type = ofSplitString(path, "/")[1];
	ofDirectory dir(path);
	dir.allowExt("png");
	dir.listDir();
	vector<ofFile> files = dir.getFiles();
	string eventName = "";
	string offset = mXML.getValue("offset", "0.0,0.0");
	int columns = mXML.getValue("columns",0);
	float columnWidth = mXML.getValue("column-width",0.0);
	float columnHeight = mXML.getValue("column-height", 0.0);
	_win->setNumColumns(columns);
	_win->setColumnWidth(columnWidth);
	_win->setColumnHeight(columnHeight);
	_win->setOffset(stringToVec2f(offset));
	if(type == "objects"){
		eventName = prefix + ":object-selected";
	}
	if(type == "armatures"){
		eventName = prefix + ":armature-selected";
	}
	if(type == "colors"){
		eventName = prefix + ":color-selected";
	}
	for(auto fIter = files.begin(); fIter != files.end(); fIter++){
		string name = fIter->getFileName();
		int id = 0;
		if(type != "colors"){
			id = ofToInt(ofSplitString(name, "_")[0]);
		}
		GuiButton* buttonPtr = new GuiButton();
		SubObEvent* eventPtr = new SubObEvent();
		eventPtr->setName(eventName);
		if(type != "colors"){
			eventPtr->addArg("object", id);
		}
		string pages = mXML.getValue("pages","none");
		buttonPtr->addEvent(eventPtr);
		buttonPtr->setPosition(ofVec2f(0.0,0.0));
		buttonPtr->setSize(ofVec2f(1.0,1.0));
		buttonPtr->setScale(1.0);
		buttonPtr->setMirrored(bMirrored);
		buttonPtr->setFlipped(bFlipped);
		buttonPtr->setPrefix(prefix);
		buttonPtr->setPages(pages);
		buttonPtr->setName(prefix + "-" + name);
		buttonPtr->addParam("image", path + name);
		buttonPtr->addParam("drag", "true");
		buttonPtr->addParam("n","1");
		buttonPtr->addParam("droppable", "true");
		buttonPtr->addParam("hidden", "true");
		buttonPtr->addParam("active", "false");
		if(type == "colors"){
			buttonPtr->addParam("attach-sample", "true");
		}
		buttonPtr->init();
		_win->addNode(buttonPtr);
	}
}

void GuiConfigurator::loadParams(GuiNode* _node){
	int numParams = mXML.getNumTags("param");
	for(int i = 0; i < numParams; i++){
		string name = mXML.getValue("param:name", "none", i);
		string val = mXML.getValue("param:val", "none", i);
		if(name == "image"){
			for(auto pIter = patterns.begin(); pIter != patterns.end(); pIter++){
				ofStringReplace(val, pIter->first, pIter->second);
			}
		}
		if((name == "none") || (val == "none")){
			continue;
		}
		_node->addParam(name, val);
	}
}

void GuiConfigurator::loadEvents(GuiNode* _node){
	int numEvents = mXML.getNumTags("event");
	for(int i = 0; i < numEvents; i++){
		string eventName = mXML.getAttribute("event","name","none",i);
		if(eventName == "none"){
			continue;
		}
		SubObEvent *event = new SubObEvent();
		if(memberEvents.find(eventName) != memberEvents.end()){
			event->setName(eventName);
		} else {
			event->setName(prefix + ":" + eventName);
		}
		mXML.pushTag("event", i);
		loadArgs(event);
		_node->addEvent(event);
		mXML.popTag();
	}
}

void GuiConfigurator::loadArgs(SubObEvent* _event){
	int numArgs = mXML.getNumTags("arg");
	for(int i = 0; i < numArgs; i++){
		string argName = mXML.getAttribute("arg","name","none",i);
		if(argName == "none"){
			continue;
		}
		string type = mXML.getValue("arg:type","none",i);
		string val = mXML.getValue("arg:val","none",i);
		if((type == "none") || (val == "none")){
			continue;
		}
		ofStringReplace(val,"%PREFIX%", prefix + ":");
		_event->addArg(argName,type,val);
	}
}

/*
void GuiConfigurator::getTimelines(){
mXML.pushTag("timelines");
int numTags = mXML.getNumTags("timeline");
for(int i = 0; i < numTags; i++){
mXML.pushTag("timeline",i);
string timelineName = mXML.getValue("name","none");
string timelineMode = mXML.getValue("mode","none");
string timelinePos = mXML.getValue("pos","none");
//SceneManager::Instance()->addTimeline(timelineName, timelineMode, timelinePos);
cout << "adding timeline " << timelineName << ", of type " << timelineMode << ", at position " << timelinePos << endl;
mXML.popTag();
}
mXML.popTag();
}
*/

void GuiConfigurator::addAttributeMap(string _sheet, vector<string> &_names, vector<string> &_values){
	//cout << "adding map to - " << _sheet << endl;
	map<string,string> tmpAttr;
	vector<string>::iterator namesIter, valsIter;
	for(namesIter = _names.begin(), valsIter = _values.begin(); namesIter < _names.end() && valsIter < _values.end(); namesIter++, valsIter++){
		//cout << "--->adding " << (*namesIter) << ", " << (*valsIter) << " to - " << _sheet << endl;
		tmpAttr[(*namesIter)] = (*valsIter);
	}
	attrs[_sheet].push_back(tmpAttr);
}

void GuiConfigurator::makeGUI(){
	//cout << "making GUI" << endl;
	map<string, vector<map<string,string> > >::iterator sIter;
	vector<map<string, string> >::iterator gIter;
	for(sIter = attrs.begin(); sIter != attrs.end(); sIter++){
		string tmpName = (*sIter).first;
		//GuiSheet* tmpSheetPtr = new GuiSheet()
		//tmpGuiSheet->setName(tmpName);
		sheets[tmpName] = new GuiSheet();
		sheets[tmpName]->setName(tmpName);
		sheets[tmpName]->setType(sheetTypes[tmpName]);
		//vector<map<string,string> > tmpAttrs = (*sIter).second;
		for(gIter = (*sIter).second.begin(); gIter != (*sIter).second.end(); gIter++){
			makeNode(tmpName,(*gIter));
		}
	}
	//SceneManager::Instance()->addMask(sheets["screen"]);
	//SceneManager::Instance()->addInfoMask(sheets["info-screen"]);
	SceneManager::Instance()->pushSheet(sheets["attract"]);
}

void GuiConfigurator::makeNode(string _handle, map<string,string> &_attrs){
	string type = _attrs["type"];
	string name = _attrs["name"];
	/*
	if(type == "scrubber"){
	sheets[_handle]->addNode(new GuiScrubber(_attrs));
	//cout << "CREATING SCRUBBER" << endl;
	*/
	if(type == "image"){
		sheets[_handle]->addNode(new GuiImage(_attrs));
	} else if(type == "button"){
		//cout << "making a button with " << nodeEvents[name].size() << " events" << endl;
		sheets[_handle]->addNode(new GuiButton(_attrs, nodeEvents[name]));
		/*
		} else if(type == "media-preview"){
		sheets[_handle]->addNode(new GuiMediaPreview(_attrs));
		} else if(type == "timeline"){
		sheets[_handle]->addNode(new GuiTimeline(_attrs));
		} else if(type == "viewport"){
		sheets[_handle]->addNode(new GuiViewport(_attrs));
		} else if(type == "video-pager"){
		sheets[_handle]->addNode(new Pager(_attrs));
		*/
	} else if(type == "loosie") {
		sheets[_handle]->setLoosie();
	}
}

void GuiConfigurator::addSheet(string _handle, GuiSheet* _sheet){
	sheets[_handle] = _sheet;
}

void GuiConfigurator::addLoosie(GuiNode* _loosie){
	loosie = _loosie;
}

GuiNode* GuiConfigurator::getLoosie(){
	return loosie;
}

void GuiConfigurator::print(){
	map<string, vector<map<string,string> > >::iterator mIter;
	vector<map<string, string> >::iterator vIter;
	//cout << "printing map of size - " << attrs.size() << endl;
	for(mIter = attrs.begin(); mIter != attrs.end(); mIter++){
		//cout << "sheet - " << (*mIter).first << endl;
		for(vIter = (*mIter).second.begin(); vIter != (*mIter).second.end(); vIter++){
			map<string,string>::iterator mpIter;
			//cout << "NEW NODE" << endl;
			for(mpIter = (*vIter).begin(); mpIter != (*vIter).end(); mpIter++){
				string first = (*mpIter).first;
				string second = (*mpIter).second;
				//cout << "--->" << first << "==>" << second << endl;
			}
		}
	}
}

void GuiConfigurator::deliverMessage(map<string,string> _msg){
	map<string,GuiSheet*>::iterator sIter;
	for(sIter = sheets.begin(); sIter != sheets.end(); ++sIter){
		if((*sIter).second->deliverMessage(_msg)){
			return;
		}
	}
}

GuiSheet* GuiConfigurator::openSheet(string _name){
	SceneManager::Instance()->pushSheet(sheets[_name]);
	if(bFirstStart && _name == "main"){
		SceneManager::Instance()->pushSheet(sheets["start-overlay"]);
		bFirstStart = false;
	}
	return sheets[_name];
}

void GuiConfigurator::setGlobal(string _handle, string _val){
	globals[_handle] = _val;
}

string GuiConfigurator::getGlobal(string _handle){
	return globals[_handle];
}

void GuiConfigurator::closeSheet(string _name){
	SceneManager::Instance()->popSheet();
}

void GuiConfigurator::replaceSheet(string _name){
	SceneManager::Instance()->popSheet();
	SceneManager::Instance()->pushSheet(sheets[_name]);
}

void GuiConfigurator::draw(){
}

void GuiConfigurator::draw(string _position){
	for(auto mIter = nodePages["all"]["all"].begin() ; mIter != nodePages["all"]["all"].end(); mIter++){
		if((*mIter)->getParam("draw-position") == _position){
			(*mIter)->draw();
		}
	}
	for(auto mIter = nodePages.begin(); mIter != nodePages.end(); mIter++){
		//cout << "Drawing nodes for " << mIter->first << endl;
		string page = currentPages[mIter->first];
		/*
		for(auto nIter = nodePages["all"][page].begin(); nIter != nodePages["all"][page].end(); nIter++){	
			//cout << "\tDrawing " << (*nIter)->getName() << endl;
			if(!(*nIter)->getIsControlled()){
				if((*nIter)->getParam("draw-position") == _position){
					//cout << "Drawing " << (*nIter)->getName() << endl;
					(*nIter)->draw();
				}
			} else {
				//cout << "\t" << (*nIter)->getName() << " not in " << _position << endl; 
			}
		}*/
		for(auto nIter = nodePages[mIter->first][page].begin(); nIter != nodePages[mIter->first][page].end(); nIter++){	
			//cout << "\tDrawing " << (*nIter)->getName() << endl;
			if(!(*nIter)->getIsControlled()){
				if((*nIter)->getParam("draw-position") == _position){
					//cout << "Drawing " << (*nIter)->getName() << endl;
					(*nIter)->draw();
				}
			} else {
				//cout << "\t" << (*nIter)->getName() << " not in " << _position << endl; 
			}
		}
	}
}

void GuiConfigurator::click(int _x, int _y){
}

void GuiConfigurator::reset(){
	bFirstStart = true;
	SceneManager::Instance()->pushSheet(sheets["attract"]);
}

void GuiConfigurator::addActive(GuiNode* _node){
	string nodeName = _node->getName();
	activeNodes[nodeName] = _node;
	//cout << "adding node - " << nodeName << ". Have " << activeNodes.size() << " node." << endl;
	SubObEvent nEvent; 
	nEvent.setName("add-object");
	nEvent.addArg("objName", nodeName);
	SubObMediator::Instance()->sendEvent(nEvent.getName(), nEvent);
	/*
	for(vector<string>::iterator aIter = availableGestures.begin(); aIter != availableGestures.end(); ++aIter){
	SubObEvent* gEvent = new SubObEvent();
	gEvent->setName("add-gesture");
	gEvent->addArg("objName",nodeName);
	gEvent->addArg("gesture",*aIter);
	SubObMediator::Instance()->sendEvent(gEvent->getName(), gEvent);
	}
	*/
	registerGestures(_node);
}

void GuiConfigurator::addNodeToPage(string _segment, string _page, GuiNode* _node){
	nodePages[_segment][_page].push_back(_node);
	registerGestures(_node);
	addActive(_node);
	//cout << "adding " << _node->getName() << " to " << _page << " for " << _segment << endl; 
}

void GuiConfigurator::setCurrentPage(string _member, string _page){
	currentPages[_member] = _page;
}

void GuiConfigurator::registerGestures(GuiNode* _node){
	if(_node->getParam("drag") == "true"){
		SubObEvent gEvent;
		gEvent.setName("add-gesture");
		gEvent.addArg("objName",_node->getName());
		gEvent.addArg("gesture","n-drag");
		SubObMediator::Instance()->sendEvent(gEvent.getName(), gEvent);
	}
	if(_node->getParam("tap") == "true"){
		SubObEvent gEvent;
		gEvent.setName("add-gesture");
		gEvent.addArg("objName",_node->getName());
		gEvent.addArg("gesture","n-tap");
		SubObMediator::Instance()->sendEvent(gEvent.getName(), gEvent);
	}
}

void GuiConfigurator::removeActive(GuiNode* _node){
	SubObEvent gEvent; 
	gEvent.setName("remove-object");
	gEvent.addArg("objName",_node->getName());
	SubObMediator::Instance()->sendEvent(gEvent.getName(), gEvent);
	activeNodes.erase(_node->getName());
}