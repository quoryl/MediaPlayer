//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIATIMER_H
#define DAEUM_MEDIATIMER_H

#include "wx/wx.h"
#include "MainFrame.h"
class MainFrame;
class MediaTimer : public wxTimer {
public:

    MediaTimer(MainFrame* frame) {m_frame = frame;}

    // Called each time the timer's timeout expires
    void Notify() override;

    MainFrame* m_frame;
};


#endif //DAEUM_MEDIATIMER_H
