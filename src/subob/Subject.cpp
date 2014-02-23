#include "Subject.h"
#include "SubObMediator.h"
#include "Utils.h"

void Subject::update(){
    SubObMediator::Instance()->update(handle, this);
}
