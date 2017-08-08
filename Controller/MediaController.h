//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLER_H
#define DAEUM_MEDIACONTROLLER_H

#include <map>
#include <wx/filepicker.h>
#include <wx/listctrl.h>

#include <taglib/apefile.h>
#include <taglib/fileref.h>

#include "ControllerInterface.h"
#include "../Model/Song.h"
#include "../GUI/MainFrame.h"

class MediaController: public ControllerInterface{

public:
    explicit MediaController( Song* s);
    void searchItem() override ;
    void addFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *mediaControl) override ;
    void deleteSong() override ;
    void shuffleList() override ;
    void prevSong() override ;
    void playSong() override ;
    void nextSong() override ;
    void showVolume() override ;
    void changeVolume() override ;
    void showAbout() override ;
    void loop(wxMediaCtrl* mediaControl) override;
    map<wxString, wxString> getMetadata(wxFilePickerCtrl *filePicker) override;
private:
    Song* song;
};


#endif //DAEUM_MEDIACONTROLLER_H
