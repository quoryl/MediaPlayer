//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_DAEUMAPP_H
#define DAEUM_DAEUMAPP_H

#include "wx/wx.h"
#include "MainFrame.h"

class DaeumApp : public wxApp {
public:

    bool OnInit() override;
    int OnExit() override;

    Playlist* playlist;
    MediaController* controller;
    MainFrame* mainFrame;

};


#endif //DAEUM_DAEUMAPP_H
