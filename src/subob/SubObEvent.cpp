#include "SubObEvent.h"
#include "Utils.h"

SubObEventArg::SubObEventArg(){
	intArg = 0;
	floatArg = 0.0;
	stringArg = "none";
	vec2Arg.set(0,0);
	vec3Arg.set(0,0,0);
}

SubObEvent::~SubObEvent(){
	/*
	for(auto mIter = args.begin(); mIter != args.end(); mIter++){
		if(mIter->second){
			delete [] mIter->second;
		}
	}
	*/
	if(!args.empty()){
		args.clear();
	}
}

SubObEvent::SubObEvent(const SubObEvent& e){
	//this->args = e.args;
	for(auto mIter = e.args.begin(); mIter != e.args.end(); mIter++){
		this->args[mIter->first] = mIter->second;
	}
	this->name = e.name;
	this->bActive = e.bActive;
}

SubObEvent & SubObEvent::operator=(const SubObEvent& e){
	if(this == &e){
		return *this;
	}

	name = e.name;
	bActive = e.bActive;

	for(auto mIter = e.args.begin(); mIter != e.args.end(); mIter++){
		args[mIter->first] = mIter->second;
	}
}

void SubObEvent::addArg(string _name, string _type, string _val){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	if(_type == "int"){
		args[_name]->setInt(ofToInt(_val));
		args[_name]->setType("int");
	}
	if(_type == "float"){
		args[_name]->setFloat(ofToFloat(_val));
		args[_name]->setType("float");
	}
	if(_type == "string"){
		args[_name]->setString(_val);
		args[_name]->setType("string");
	}
	if(_type == "ofVec2f"){
		args[_name]->setVec2(stringToVec2f(_val));
		args[_name]->setType("vec2");
	}
	if(_type == "ofVec3f"){
		args[_name]->setVec3(stringToVec3f(_val));
		args[_name]->setType("vec3");
	}
}

void SubObEvent::addArg(string _name, string _arg){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	args[_name]->setString(_arg);
	args[_name]->setType("string");
}

void SubObEvent::addArg(string _name, int _arg){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	args[_name]->setInt(_arg);
	args[_name]->setType("int");
}

void SubObEvent::addArg(string _name, float _arg){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	args[_name]->setFloat(_arg);
	args[_name]->setType("float");
}

void SubObEvent::addArg(string _name, ofVec2f _arg){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	args[_name]->setVec2(_arg);
	args[_name]->setType("vec2");
}

void SubObEvent::addArg(string _name, ofVec3f _arg){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	args[_name]->setVec3(_arg);
	args[_name]->setType("vec3");
}

SubObEventArg* SubObEvent::getArg(string _name){
	if(args.count(_name)){
		return args[_name];
	} else {
		return new SubObEventArg();
	}
}

