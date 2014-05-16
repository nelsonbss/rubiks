#ifndef GUICONFIGURATOR_H_INCLUDED
#define GUICONFIGURATOR_H_INCLUDED

#include "ofMain.h"
#include "ofxXmlSettings.h"

//#include "Pager.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiIBox.h"
#include "GuiDropArea.h"
#include "GuiWindow.h"
//#include "GuiScrubber.h"
#include "GuiSheet.h"
//#include "GuiMediaPreview.h"
//#include "GuiViewport.h"
#include "..\SceneManager.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "SubObEvent.h"
#include "Utils.h"

class GuiCreator;

class GuiText{
	
public:
	GuiText(){}
	void setText(string _lang, string _text){texts[_lang] = _text;}
	string getText(string _lang){return texts[_lang];}

private:
	map<string, string> texts;
};

class GuiConfigurator : public Observer, public Subject{

public:

    static GuiConfigurator* Instance();
    void addFile(string _file);
    void getTags();
	void getSheet();
    //void getTimelines();
    void makeGUI();
    void makeNode(string _handle, map<string, string> &_attrs);
	void makeEvents();
    void print();
    void draw();
	void draw(string _position);
    void addSheet(string _handle, GuiSheet *_sheet);
    GuiSheet* openSheet(string _name);
    void closeSheet(string _name);
    void replaceSheet(string _name);
    void deliverMessage(map<string, string> _msg);
    string getGlobal(string _handle);
    void setGlobal(string _handle, string _val);
    GuiNode* getLoosie();
    void addLoosie(GuiNode* _loosie);
    void addActive(GuiNode* _node);
	void removeActive(GuiNode* _node);

	/*
	New
	*/
	void loadGui(bool _mirrored, bool _flipped);
	void extendGui(string _sheet, string _file, bool _mirrored, bool _flipped, string _prefix, map<string,string> _patterns); 
	void loadSheets();
	void loadNodes(string _sheetName, GuiWindow* _win);
	void loadNodesFromDirectory(string _path, GuiWindow* _win);
	void loadParams(GuiNode* _node);
	void loadEvents(GuiNode* _node);
	void loadArgs(SubObEvent* _event);
	void loadTexts(string _file);
	void specialTextLoadingCopOut();

	map<string, GuiText> texts;
	void addText(string _name, GuiText _t){texts[_name] = _t;}

	string currentLanguage;
	void setCurrentLanguage(string _l){currentLanguage = _l;}

	string getText(string _text, string _lang){return texts[_text].getText(_lang);}

	/*
    Subject/Observer interface
    */

    void update(string _subName, Subject* _sub);
	void update(string _eventName, SubObEvent _event);

    /*
    Do this next.
    */
    void testGUI();
    void click(int _x, int _y);
    void reset();

	map<string, string> patterns;

	void addNodeToPage(string _segment, string _page, GuiNode* _node);
	void registerGestures(GuiNode* _node);
	map<string, map<string, vector<GuiNode*> > > nodePages;

	void setCurrentPage(string _member, string _page);
	map<string, string> currentPages;
private:

    GuiConfigurator();
    static GuiConfigurator* mInstance;

    ofxXmlSettings mXML;
	ofxXmlSettings lXML;
    void addAttributeMap(string _sheet, vector<string> &_names, vector<string> &_values);

	map<string,GuiNode*> activeNodes;
	vector<string> availableGestures;

    map<string, vector<map<string,string> > > attrs;
    map<string, vector<GuiNode*> > guiNodes;
    map<string,GuiSheet*> sheets;
    map<string, string> globals;
    map<string,string>sheetTypes;
    map<string,vector<SubObEvent*>> nodeEvents;
	GuiNode* loosie;
    bool bFirstStart;
	SubObEvent dummy;

	bool bMirrored;
	bool bFlipped;

	string prefix;

	set<string> memberEvents;
};


#endif // GUICONFIGURATOR_H_INCLUDED
