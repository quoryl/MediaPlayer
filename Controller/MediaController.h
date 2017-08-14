//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLER_H
#define DAEUM_MEDIACONTROLLER_H

#include <map>


#include "wx/listctrl.h"
#include "wx/wx.h"

#include <taglib/apefile.h>
#include <taglib/fileref.h>
#include <wx/generic/listctrl.h>
#include <wx/mediactrl.h>
#include <wx/filepicker.h>

#include "ControllerInterface.h"
#include "../Model/Song.h"
#include "../GUI/MainFrame.h"
class MainFrame; // FIXME it's linked to the fixme below
class MediaController: public ControllerInterface{

public:
    MediaController(Playlist* pList);

    void searchItem(wxString text) override ;
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
    void setLoop();
    map<wxString, wxString> getMetadata(wxFilePickerCtrl *picker) override;
private:
    Playlist* playlist;

};


#endif //DAEUM_MEDIACONTROLLER_H
