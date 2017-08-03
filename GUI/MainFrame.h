//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_MAINFRAME_H
#define DAEUM_MAINFRAME_H

#include <memory>
#include <wx/filepicker.h>
#include <wx/srchctrl.h>
#include <wx/listctrl.h>
#include <wx/mediactrl.h>

#include "wx/wx.h"
#include "Observer.h"
#include "../Model/Song.h"
#include "../Controller/MediaController.h"


using namespace std;

class MainFrame : public Observer {
public:
    void update();

    explicit MainFrame(wxWindow *parent,  wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,373 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    virtual ~MainFrame();
protected:
    wxSearchCtrl* searchBar;
    wxStaticText* addText;
    wxFilePickerCtrl* loadFile;
    wxButton* addFile;
    wxButton* deleteFromPlaylist;
    wxButton* shuffle;
    wxListCtrl* SongList;
    wxMediaCtrl* mediaCtrl;
    wxGauge* progressBar;
    wxButton* Previous;
    wxButton* Play;
    wxButton* Next;
    wxButton* Repeat;
    wxStaticText* volumeLabel;
    wxSlider* Volume;
    wxMenuBar* menuBar;
    wxMenu* About;
    wxMenu* Quit;


    void onSearch( wxCommandEvent& event );
    void onAdd( wxCommandEvent& event );
    void onDelete( wxCommandEvent& event );
    void onShuffle(wxCommandEvent &event);
    void onPrevious( wxCommandEvent& event );
    void onPlay( wxCommandEvent& event );
    void onNext( wxCommandEvent& event ) ;
    void onRepeat( wxCommandEvent& event ) ;
    void onScrollTrack(wxScrollEvent &event) ;
    void onScrollChanged(wxScrollEvent &event) ;
    void onMenuItem( wxCommandEvent& event );

    MediaController* mediaController;
    Song* song;




};
#endif //DAEUM_MAINFRAME_H
