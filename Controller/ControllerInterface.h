//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLERINTERFACE_H
#define DAEUM_MEDIACONTROLLERINTERFACE_H


#include <wx/filepicker.h>
#include "wx/wx.h"
#include "../Model/Song.h"
#include <wx/listctrl.h>
#include <wx/mediactrl.h>
#include <map>

class ControllerInterface {
public:
    virtual void searchItem(wxString text) = 0;
    virtual void addFile(wxArrayString *paths) = 0;
    virtual void deleteSong(wxString toDeletePath) = 0;
    virtual void shuffleList() = 0;
    virtual void prevSong() = 0;
    virtual void nextSong() = 0;
    virtual void showAbout() = 0;
    virtual void save() = 0;
    virtual void load() = 0;

    virtual std::map<wxString, wxString> getMetadata(wxString *filePicker) = 0;


};


#endif //DAEUM_MEDIACONTROLLERINTERFACE_H
