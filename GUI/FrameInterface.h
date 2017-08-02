//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_FRAMEINTERFACE_H
#define DAEUM_FRAMEINTERFACE_H

#include "wx/wx.h"

class FrameInterface: public wxFrame {
public:
    virtual void update()=0; //TODO
    FrameInterface( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) ;

};


#endif //DAEUM_FRAMEINTERFACE_H
