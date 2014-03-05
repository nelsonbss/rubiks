#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GuiNode.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "ofMain.h"

class GuiNode;

class GuiButton : public GuiNode
{
    public:
        GuiButton(map<string,string> &_attrs, vector<SubObEvent*> _events);
        GuiButton(string _img);
        GuiButton(){}
        virtual ~GuiButton(){}
		void nodeInit();
		void nodeExecute();
        bool processMouse(int _x, int _y, int _state);
		void input(string _type, int _ID, int _n, int _phase, ofVec2f _absPos, ofVec2f _deltaPos);
        virtual void update(string _subName, Subject* _sub);
        void update(string _eventName, SubObEvent* _event);
		void setMessage(map<string,string> _msg){}
        virtual void nodeDraw();
        string getAttr(const char* _key){return attrs[_key];}
		void setChannel(string _channel){channel = _channel;}
		string getChannel(){return channel;}
	protected:
    private:
        ofImage inactive;
        ofImage active;
        ofImage arabic;
		string channel;
		bool drawActive;
        bool haveActive;
        bool haveImage;
        bool haveArabic;

		//Draggable members
		bool bSendActions;
		bool bDraggable;
		bool bSelected;
		bool bTacky;
		ofVec2f selectionLocation;
		ofVec2f dragStartLocation;
};

#endif // GUIBUTTON_H
