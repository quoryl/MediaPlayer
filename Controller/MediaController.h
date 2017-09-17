//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLER_H
#define DAEUM_MEDIACONTROLLER_H


#include <chrono>
#include "wx/listctrl.h"
#include "wx/wx.h"

#include "taglib/tag.h"
#include "taglib/fileref.h"
#include <wx/generic/listctrl.h>
#include <wx/mediactrl.h>
#include <wx/filepicker.h>
#include <wx/aboutdlg.h>

#include <wx/txtstrm.h> //output and input stream. For saving the previous session
#include <wx/wfstream.h>

#include "ControllerInterface.h"
#include "../Model/Song.h"
#include "../GUI/MainFrame.h"

class MediaController: public ControllerInterface{

public:
    explicit MediaController(Playlist* pList);

    void searchItem(wxString text) override ;
    void addFile(wxArrayString *paths) override ;
    void deleteSong(wxArrayString toDeletePath) override ;
    void shuffleList() override ;
    void prevSong() override ;
    void nextSong() override ;
    void setLoop();
    bool save() override;
    bool load() override;
    Song* getSongFromPlaylist(wxString path);
    void tellPlaylist(wxString songPath) override;
    vector<wxString> myReader(const char* path);
private:
    Playlist* playlist;
};


#endif //DAEUM_MEDIACONTROLLER_H
