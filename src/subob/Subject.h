#ifndef SUBJECT_H
#define SUBJECT_H

#include "ofMain.h"
#include "SubObEvent.h"

class Subject
{
    public:
        Subject() {}
        Subject(string _handle) {handle = _handle;}
        virtual ~Subject(){}
        void update();
        virtual string getAttr(const char* _key){string ret = _key; return ret;}
		void addEvent(string _name, SubObEvent* _event){events[_name] = _event;}
		string getEventArg(string _event, string _arg);
    protected:
    private:
        string handle;
		map<string, SubObEvent*> events;
};

#endif // SUBJECT_H
