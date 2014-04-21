#ifndef GUINODE_H
#define GUINODE_H

#include "ofMain.h"
#include "SubObEvent.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "ofxTimer.h"
#include "Utils.h"

class SubObEvent;

enum{
    MOUSE_STATE_DOWN,
    MOUSE_STATE_UP,
    MOUSE_STATE_DRAG
};

class DragData{
public:
	DragData(){}
	DragData(int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){ID = _ID;n = _n; phase = _phase; absPos = _absPos; deltaPos = _deltaPos;}
	int getN(){return n;}
	void setN(int _n){n = _n;}
	int getID(){return ID;}
	void setID(int _ID){ID = _ID;}
	int getPhase(){return phase;}
	void setPhase(int _phase){phase = _phase;}
	ofVec2f getAPos(){return absPos;}
	void setAPos(ofVec2f _val){absPos = _val;}
	ofVec2f getDPos(){return absPos;}
	void setDPos(ofVec2f _val){deltaPos = _val;}

private:
	int n;
	int ID;
	int phase;
	ofVec2f absPos;
	ofVec2f deltaPos;
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
	void setPosition(float _x, float _y){pos.x = _x; pos.y = _y;}
	void setDrawPosition(ofVec2f _pos){drawPos = _pos; homePos = drawPos; bUseHomePos = true;}
	void draw();
	void draw(ofVec2f _pnt);
	void init();
	virtual void adjustPosition(ofVec2f _dPos, ofVec2f _aPos){}
    virtual void message(map<string,string> _msg){}
    virtual void sendMessage(){}
    virtual void reset(){}
    virtual void setAllAttr(string _attr, string _val){setAttr(_attr,_val);}
    virtual void unclick(){}
    virtual void click(){}
    virtual bool getClicked(){return 0;}
	virtual void activate();
	virtual void deactivate();
    virtual string getName(){return name;}
    void setName(string _name){name = _name;}
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

	virtual void hide();
	virtual void unhide();

	bool isHidden(){return bHidden;}
	virtual bool isActive(){
 		return bActive;
	}

	virtual void setupText();

	bool bHaveText;
	bool bHaveText2;
	ofTrueTypeFont font;
	string text;
	string text2;
	string textAlign;

	ofVec3f textColor;
	ofVec2f textPosition;
	ofVec2f textPosition2;

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
	
	float drawZ;

	float getZ(){return drawZ;}
	void setZ(float _z){drawZ = _z;}

	bool bReadyForInput;
	bool getReadyForInput(){return bReadyForInput;}

	void setMirrored(bool _m){bMirrored = _m;}
	void setFlipped(bool _f){bFlipped = _f;}

    //Virtual methods that each subclass is responsible for defining.
    virtual void execute();
    virtual void executeDrag(int _x, int _y){}
    virtual void drawLoosie(){}
    virtual void update(){}
	virtual void nodeInit(){}
	virtual void nodeDraw(){}
	virtual void nodeExecute(){}
	virtual void nodeActivate(){}
	virtual void nodeDeactivate(){}
	virtual void nodeSetPosition(){}
	virtual void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos){cout << "node input" << endl;}

	void setPrefix(string _p){prefix = _p;}
	string getPrefix(){return prefix;}

	bool isSelected(){return bSelected;}

	SubObEvent inter;
	void sendInteraction();

	void registerPages(string _pages);

	string pages;
	void setPages(string _pages){pages = _pages;}
	string getPages(){return pages;}

	bool bActive;

	bool bHaveCustomArea;
	ofVec2f customArea;
	ofVec2f customSize;
	void setCustomArea(ofVec2f _a, ofVec2f _s);

protected:

    map<string,string> attrs;
	map<string,string> params;
	vector<SubObEvent*> events;
    ofVec2f pos;
	ofVec2f drawPos;
	ofVec2f dragPos;
	ofVec2f homePos;
	bool bUseHomePos;
	ofVec2f drawSize;
    float scale;
	ofVec2f size;
    string name;
    string type;
    string parent;
    int dur;
	bool bDrawArea;
	ofVec3f drawColor;
	ofxTimer* timer;
	bool bHidden;

	bool bMirrored;
	bool bFlipped;

	string prefix;
	bool bSelected;

	string currentLanguage;

	ofVec2f lastMouse;
};


#endif // GUINODE_H
