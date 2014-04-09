#ifndef SUBOBEVENT_H
#define SUBOBEVENT_H

#include "ofMain.h"

class SubObEventArg{

public:
	SubObEventArg();
	void setName(string _name){name = _name;}
	string getName(){return name;}
	void setType(string _type){type = _type;}
	string getType(){return type;}
	void setString(string _str){stringArg = _str;}
	string getString(){return stringArg;}
	void setInt(int _i){intArg = _i;}
	int getInt(){return intArg;}
	void setFloat(float _f){floatArg = _f;}
	float getFloat(){return floatArg;}
	void setVec2(ofVec2f _v){vec2Arg = _v;}
	ofVec2f getVec2(){return vec2Arg;}
	void setVec3(ofVec3f _v){vec3Arg = _v;}
	ofVec3f getVec3(){return vec3Arg;}

private:
	string name;
	string type;
	int intArg;
	float floatArg;
	string stringArg;
	ofVec2f vec2Arg;
	ofVec3f vec3Arg;

};

class SubObEvent{

public:
	SubObEvent(){bActive = false;}
	SubObEvent(string _name){bActive = false; name = _name;}
	~SubObEvent();
	SubObEvent(const SubObEvent& e);
	SubObEvent & SubObEvent::operator=(const SubObEvent& e);
	void setName(string _name){name = _name;}
	string getName(){return name;}
	SubObEventArg* getArg(string _name);
	void addArg(string _name, string _type, string _arg);
	void addArg(string _name, string _arg);
	void addArg(string _name, int _arg);
	void addArg(string _name, float _arg);
	void addArg(string _name, ofVec2f _arg);
	void addArg(string _name, ofVec3f _arg);
	bool hasArg(string _arg){return args.count(_arg);}
	void setActive(bool _active){bActive = _active;}
	bool isActive(){return bActive;}

	map<string,SubObEventArg*> args;

private:
	string name;
	bool bActive;
};

#endif