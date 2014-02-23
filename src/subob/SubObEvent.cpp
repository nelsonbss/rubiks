#include "SubObEvent.h"
#include "Utils.h"

SubObEventArg::SubObEventArg(){
	intArg = 0;
	floatArg = 0.0;
	stringArg = "none";
	vec2Arg.set(0,0);
	vec3Arg.set(0,0,0);
}

void SubObEvent::addArg(string _name, string _type, string _val){
	args[_name] = new SubObEventArg();
	args[_name]->setName(_name);
	if(_type == "int"){
		args[_name]->setInt(ofToInt(_val));
	}
	if(_type == "float"){
		args[_name]->setFloat(ofToFloat(_val));
	}
	if(_type == "string"){
		args[_name]->setString(_val);
	}
	if(_type == "ofVec2f"){
		args[_name]->setVec2(stringToVec2f(_val));
	}
	if(_type == "ofVec3f"){
		args[_name]->setVec3(stringToVec3f(_val));
	}
}

SubObEventArg* SubObEvent::getArg(string _name){
	if(args.count(_name)){
		return args[_name];
	} else {
		return new SubObEventArg();
	}
}

