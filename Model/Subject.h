//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_ABSTRACTSONG_H
#define DAEUM_ABSTRACTSONG_H

#include "../GUI/Observer.h"

class Subject {
public:
    virtual void notifyObserver() const=0;
    virtual void registerObserver(Observer* o)=0;
    virtual void removeObserver(Observer* o)=0;

};


#endif //DAEUM_ABSTRACTSONG_H
