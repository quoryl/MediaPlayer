//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLERINTERFACE_H
#define DAEUM_MEDIACONTROLLERINTERFACE_H


#include <wx/filepicker.h>
#include "wx/wx.h"
#include <wx/listctrl.h>
#include <wx/mediactrl.h>

class ControllerInterface {
public:
    virtual void Search() = 0;
    virtual void AddFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *m) = 0;
    virtual void Delete() = 0;
    virtual void Shuffle() = 0;
    virtual void Previous() = 0;
    virtual void Play() = 0;
    virtual void Next() = 0;
    virtual void Repeat() = 0;
    virtual void showVolume() = 0;
    virtual void changeVolume() = 0;
    virtual void showAbout() = 0;


};


#endif //DAEUM_MEDIACONTROLLERINTERFACE_H
