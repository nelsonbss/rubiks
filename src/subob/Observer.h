#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <iostream>
#include "Subject.h"

using namespace std;

class Observer
{
    public:
        Observer();
        virtual ~Observer();
        virtual void update(string _subName, Subject* _sub);
		virtual void update(string _eventName, SubObEvent _event);
    protected:
    private:
};

#endif // OBSERVER_H
