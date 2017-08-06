//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLER_H
#define DAEUM_MEDIACONTROLLER_H


#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include "ControllerInterface.h"
#include "../Model/Song.h"
#include "../GUI/MainFrame.h"

class MediaController: public ControllerInterface{

public:
    explicit MediaController( Song* s);
    void Search() override ;
    void AddFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *mediaController) override ;
    void Delete() override ;
    void Shuffle() override ;
    void Previous() override ;
    void Play() override ;
    void Next() override ;
    void Repeat() override ;
    void showVolume() override ;
    void changeVolume() override ;
    void showAbout() override ;
private:
    Song* song;
};


#endif //DAEUM_MEDIACONTROLLER_H
