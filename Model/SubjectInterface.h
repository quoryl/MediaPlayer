//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_ABSTRACTSONG_H
#define DAEUM_ABSTRACTSONG_H

#include "../GUI/MainFrame.h"

class SubjectInterface {
public:
    virtual void notifyObserver() const=0;
    virtual void registerObserver(MainFrame& mainFrame)=0;
    virtual void removeObserver(MainFrame& mainFrame)=0;

};


#endif //DAEUM_ABSTRACTSONG_H
