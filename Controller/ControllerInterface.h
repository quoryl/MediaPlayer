//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLERINTERFACE_H
#define DAEUM_MEDIACONTROLLERINTERFACE_H


#include <wx/filepicker.h>
#include "wx/wx.h"
#include <wx/listctrl.h>
#include <wx/mediactrl.h>
#include <map>

class ControllerInterface {
public:
    virtual void searchItem(wxString text) = 0;
    virtual void addFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *m) = 0;
    virtual void deleteSong() = 0;
    virtual void shuffleList() = 0;
    virtual void prevSong() = 0;
    virtual void playSong() = 0;
    virtual void nextSong() = 0;
    virtual void loop(wxMediaCtrl* mediaControl) = 0;
    virtual void showVolume() = 0;
    virtual void changeVolume() = 0;
    virtual void showAbout() = 0;

    virtual std::map<wxString, wxString> getMetadata(wxFilePickerCtrl *filePicker) = 0;


};


#endif //DAEUM_MEDIACONTROLLERINTERFACE_H
