#ifndef GUISHEET_H
#define GUISHEET_H

#include "GuiNode.h"
#include "Subject.h"
//#include "Compositor.h"

class GuiSheet : public Subject{

public:
    GuiSheet();
    void addNode(GuiNode* _node);
	void addMenu(string _set, GuiSheet* _sheet); //Expects a completed gui sheet.  
    //void pushToStack();
    //void popFromStack();
    void update();
    void activate();
	void deactivate();
    void draw();
    void touch();
    void setName(string _name){name = _name;}
    void setType(string _type){attrs["type"] = _type;}
    void setAllAttr(string _attr, string _val);
    void reset();
    string getName(){return name;}
    bool checkNodes(int _x, int _y, string _state);
    bool checkNodesForDrag(int _x, int _y);
    GuiNode* getActivatedNode(){return activatedNode;}
    bool deliverMessage(map<string, string> _msg);
    void setLoosie(){haveLoosie = true;}
    string getAttr(const char * _key){return attrs[_key];}

private:
    vector<GuiNode*> nodes;
    string name;
    string type;
    string effectTarget;
	ofVec2f pos;
	map<string, GuiSheet*> menus;
    GuiNode* activatedNode;
    bool haveLoosie;
    map<string,string> attrs;

};

#endif // GUISHEET_H
