#ifndef GUINODE_H
#define GUINODE_H

#include "ofMain.h"
#include "SubObEvent.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"

class SubObEvent;

enum{
    MOUSE_STATE_DOWN,
    MOUSE_STATE_UP,
    MOUSE_STATE_DRAG
};

class GuiNode: public Subject, public Observer{

public:

    GuiNode();
    virtual ~GuiNode(){}
    //Test if a touchscreen selection is within the node.
    virtual bool isInside(int _x, int _y);
    virtual bool isDragging(int _x, int _y){return false;}
    virtual bool processMouse(int _x, int _y, int _state) = 0;
	virtual void setPosition(ofVec2f _pos){pos = _pos;}
	virtual void adjustPosition(ofVec2f _dPos){drawPos += _dPos;}
    virtual void message(map<string,string> _msg){}
    virtual void sendMessage(){}
    virtual void reset(){}
    virtual void setAllAttr(string _attr, string _val){setAttr(_attr,_val);}
    virtual void unclick(){}
    virtual void click(){}
    virtual bool getClicked(){return 0;}
	virtual void activate();
	virtual void deactivate();
    void initialize();
    string getName(){return name;}
    void setName(string &_name){name = _name;}
    void setName(const char * _name){name = _name;}
    string getType(){return type;}
    ofVec2f getPos(){return drawPos;}
	ofVec2f getCenter(){ofVec2f(drawPos.x + (size.x / 2), drawPos.y + (size.y / 2));}
    ofVec2f getSize(){return size;}
    void setSize(float x, float y){size.set(x,y);}
	void setSize(ofVec2f _size){size = _size;}
	void setScale(float _scale){scale = _scale;}
    void setAttr(string _attr, string _val){attrs.insert(pair<string,string>(_attr, _val));}
    string getAttr(string _attr){return attrs[_attr];}
    void setDur(int _dur){dur = _dur;}
    int getDur(){return dur;}
    void setParent(string _parent){parent = _parent;}
	void setPosition();
	void _windowResized();
	virtual void windowResized(){}

	//new event and param
	void addEvent(SubObEvent* _event){events.push_back(_event);}
	void addParam(string _name, string _val){params[_name] = _val;}
	string getParam(string _name){
		if(params.count(_name)){
			return params[_name];
		} else {
			return "__NONE__";
		}
	}

    //Virtual methods that each subclass is responsible for defining.
    virtual void execute();
    virtual void executeDrag(int _x, int _y){}
    virtual void draw() = 0;
    virtual void drawLoosie(){}
    virtual void update(){}
	virtual void init(){}

protected:

    map<string,string> attrs;
	map<string,string> params;
	vector<SubObEvent*> events;
    ofVec2f pos;
	ofVec2f drawPos;
    float scale;
	ofVec2f size;
    string name;
    string type;
    string parent;
    int dur;
};


#endif // GUINODE_H
