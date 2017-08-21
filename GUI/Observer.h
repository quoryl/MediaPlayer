//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_FRAMEINTERFACE_H
#define DAEUM_FRAMEINTERFACE_H

#include <list>
#include "wx/wx.h"
#include "../Controller/ControllerInterface.h"
#include "../Model/Song.h"

class Observer{
public:
    virtual void update(std::__cxx11::list<Song *>& playList)=0;
protected:
    ControllerInterface* ctrl;
};


#endif //DAEUM_FRAMEINTERFACE_H
