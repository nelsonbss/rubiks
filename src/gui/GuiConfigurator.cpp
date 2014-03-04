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
	SubObMediator::Instance()->sendEvent("add-gesture-receiver",new SubObEvent());
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

void GuiConfigurator::update(string _eventName, SubObEvent* _event){
	//cout << "received event named - " << _eventName << endl;
	if(_eventName == "replace-sheet"){
   		string target = _event->getArg("target")->getString();
		cout << "replacing with " << target << endl;
		replaceSheet(target);
	}
	if(_eventName == "touch-point"){
		vector<GuiNode*> nodesHit;
		for(map<string, GuiNode*>::iterator nIter =  activeNodes.begin(); nIter != activeNodes.end(); ++nIter){
			if(nIter->second->isInside(_event->getArg("x")->getFloat(), _event->getArg("y")->getFloat())){
				//_event->getArg("hit")->setInt(1);
				//_event->addArg("target",nIter->first);
				//break;
				nodesHit.push_back(nIter->second);
			}
		}
		if(nodesHit.size()){
			cout << "hit " << nodesHit.size() << " nodes." << endl;
			float highestZ = -10000.0;
			GuiNode* highestNode;
			for(vector<GuiNode*>::iterator nIter = nodesHit.begin(); nIter != nodesHit.end(); ++nIter){
				float nodeZ = (*nIter)->getZ();
				cout << "checking node with z = " << nodeZ << endl;
				if(nodeZ > highestZ){
					cout << "setting " << (*nIter)->getName() << " to highest z. Z = " << nodeZ << endl;
					highestNode = *nIter;
					highestZ = nodeZ;
				}
			}
			_event->getArg("hit")->setInt(1);
			_event->addArg("target", highestNode->getName());
		}
		_event->getArg("receivers")->setInt(_event->getArg("receivers")->getInt() + 1);
	}
	if(_eventName == "gesture"){
		string target = _event->getArg("target")->getString();
		string type = _event->getArg("type")->getString();
		cout << "have " << type << " going to " << target << endl;
		if(type == "drag" && (activeNodes.count(target))){
			string draggable = activeNodes[target]->getParam("draggable");
			cout << draggable << endl;
			if(draggable == "true"){
				cout << "...and we're draggable." << endl;
				int n = 1;
				string targetN = activeNodes[target]->getParam("n");
				int eventN = _event->getArg("n")->getInt();
				cout << "event n = " << eventN << endl;
				if(targetN != "__NONE__"){
					n = ofToInt(targetN);
				}
				if(eventN == n){
					//activeNodes[target]->adjustPosition(_event->getArg("drag_d")->getVec2(), _event->getArg("position")->getVec2());
					cout << "sending drag." << endl;
					int cID = _event->getArg("ID")->getInt();
					int cN = _event->getArg("n")->getInt();
					int cPhase = _event->getArg("phase")->getInt();
					ofVec2f cAPos = _event->getArg("absPos")->getVec2();
					ofVec2f cDPos = _event->getArg("deltaPos")->getVec2();
					activeNodes[target]->dragInput(cID, cN, cPhase, cAPos, cDPos);
				}
			} else {
				cout << "...but we're not draggable." << endl;
			}
		} else {
			cout << "gesture fail" << endl;
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
				SubObEvent* tmpEventPtr = new SubObEvent();
				string eventName = mXML.getAttribute("event", "name", "none", k);
				//cout << "adding event - " << eventName << endl;
				tmpEventPtr->setName(eventName);
				mXML.pushTag("event", k);
				int numArgs = mXML.getNumTags("arg");
				for(int args = 0; args < numArgs; args++){
					string argName = mXML.getAttribute("arg", "name", "none", args);
					mXML.pushTag("arg", args);
					string type = mXML.getValue("type", "string");
					string val = mXML.getValue("val","none");
					tmpEventPtr->addArg(argName, type, val);
					mXML.popTag();
				}
				mXML.popTag();
				tmpEvents.push_back(tmpEventPtr);
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

void GuiConfigurator::loadGui(){
	mXML.pushTag("gui");
	string main = mXML.getValue("main-sheet","attract");
	loadSheets();
	SceneManager::Instance()->pushSheet(sheets[main]);
}

void GuiConfigurator::loadSheets(){
	int numSheets = mXML.getNumTags("sheet");
	for(int i = 0; i < numSheets; i++){
		string sheetName = mXML.getAttribute("sheet", "name", "none", i);
		if(sheetName == "none"){
			continue;
		}
		sheets[sheetName] = new GuiSheet();
		sheets[sheetName]->setName(sheetName);
		mXML.pushTag("sheet", i);
		loadNodes(sheetName);
		mXML.popTag();
	}
}

void GuiConfigurator::loadNodes(string _sheetName){
	int numNodes = mXML.getNumTags("node");
	for(int i = 0; i < numNodes; i++){
		string nodeName = mXML.getAttribute("node","name","none",i);
		if(nodeName == "none"){
			continue;
		}
		string nodeType = mXML.getAttribute("node","type","none",i);
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
		} else {
			continue;
		}
		if(nodePtr == NULL){
			continue;
		}
		nodePtr->setName(nodeName);
		mXML.pushTag("node", i);
		string pos = mXML.getValue("pos", "0.0,0.0");
		string size = mXML.getValue("size", "150.0,150.0");
		string scale = mXML.getValue("scale","1.0");
		nodePtr->setPosition(stringToVec2f(pos));
		nodePtr->setSize(stringToVec2f(size)); 
		nodePtr->setScale(ofToFloat(scale));
		loadParams(nodePtr);
		loadEvents(nodePtr);
		nodePtr->init();
		sheets[_sheetName]->addNode(nodePtr);
		mXML.popTag();
	}
}

void GuiConfigurator::loadParams(GuiNode* _node){
	int numParams = mXML.getNumTags("param");
	for(int i = 0; i < numParams; i++){
		string name = mXML.getValue("param:name", "none", i);
		string val = mXML.getValue("param:val", "none", i);
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
		SubObEvent* eventPtr = new SubObEvent();
		eventPtr->setName(eventName);
		mXML.pushTag("event", i);
		loadArgs(eventPtr);
		_node->addEvent(eventPtr);
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
	cout << "making GUI" << endl;
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

void GuiConfigurator::click(int _x, int _y){
}

void GuiConfigurator::reset(){
    bFirstStart = true;
    SceneManager::Instance()->pushSheet(sheets["attract"]);
}

void GuiConfigurator::addActive(GuiNode* _node){
	string nodeName = _node->getName();
	activeNodes[nodeName] = _node;
	SubObEvent* nEvent = new SubObEvent();
	nEvent->setName("add-object");
	nEvent->addArg("objName", nodeName);
	SubObMediator::Instance()->sendEvent(nEvent->getName(), nEvent);
	for(vector<string>::iterator aIter = availableGestures.begin(); aIter != availableGestures.end(); ++aIter){
		SubObEvent* gEvent = new SubObEvent();
		gEvent->setName("add-gesture");
		gEvent->addArg("objName",nodeName);
		gEvent->addArg("gesture",*aIter);
		SubObMediator::Instance()->sendEvent(gEvent->getName(), gEvent);
	}
}

void GuiConfigurator::removeActive(GuiNode* _node){
	activeNodes.erase(_node->getName());
}