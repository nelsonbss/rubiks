#include "CubieManager.h"

CubieManager::CubieManager(){
	SubObMediator::Instance()->addObserver("button", this);
}

void CubieManager::update(string _subName, Subject *_sub){
	if(_subName == "button"){
		string msg = _sub->getAttr("message");
		cout << "CM got message - " << msg << endl;
	}
};