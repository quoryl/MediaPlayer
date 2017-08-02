//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_SONG_H
#define DAEUM_SONG_H

#include "wx/wx.h"
#include "SubjectInterface.h"


class Song : public SubjectInterface{
public:
    int length;
    wxString title;
    wxString artist;
    wxString album;


    virtual void notifyObserver() const override;
    virtual void registerObserver(MainFrame& mainFrame) override;
    virtual void removeObserver(MainFrame& mainFrame) override;

};


#endif //DAEUM_SONG_H
