#ifndef CUBIEMANAGER_H
#define CUBIEMANAGER_H

#include "ofMain.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"

class CubieManager: public Observer{
public:
	CubieManager();
	virtual ~CubieManager(){}
	void update(string _subName, Subject* _sub);
private:
};

#endif