//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_MAINFRAME_H
#define DAEUM_MAINFRAME_H

#include <memory>
#include <chrono>
#include "wx/config.h"
#include <wx/wx.h>
#include <wx/windowid.h>
#include <wx/srchctrl.h>
#include <wx/gtk/stattext.h>
#include <wx/gtk/button.h>
#include <wx/gtk/slider.h>
#include <wx/statusbr.h>
#include <wx/gtk/menu.h>
#include <wx/mediactrl.h>
#include <wx/listbase.h>
#include <wx/filepicker.h>
#include <wx/statline.h>

#include "Observer.h"
#include "../Model/Song.h"
#include "../Model/Playlist.h"
#include "../Controller/MediaController.h"
#include "MediaTimer.h"
#include "DragAndDrop.h"

using namespace std;
class MediaController;
class MediaTimer;

class MainFrame : public wxFrame, public Observer {
public:
    void update(list<Song *> &playList, Song *playing) override;
    void updateSongDetails(Song* s, Song* prevPlaying) override;
    void play(wxString path) override;

    MainFrame(MediaController *mediaController, Playlist *pList, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("Title"),
              const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,373 ),
              long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);


    virtual ~MainFrame();


    wxSearchCtrl* searchBar;
    wxFileDialog* loadFile;
    wxButton* addFile;
    wxButton* deleteFromPlaylist;
    wxButton* shuffle;
    wxListView* songList;// the GUI part of the Playlist
    wxMediaCtrl* mediaCtrl;
    wxButton* Previous;
    wxButton* Play;
    wxButton* Next;
    wxButton* Stop;
    wxButton* Loop;
    wxButton* prevSession;
    wxButton* save;
    wxButton* volumeButton;
    wxSlider* Volume;
    wxSlider* mediaSlider;
    wxMenuBar* menuBar;
    wxMenu* File;//Menu option containing save and load methods
    wxMenu* About;
    wxMenu* Instructions;
    wxMenu* Quit;
    MediaTimer* mediaTimer; //updates the mediaSlider with the position of the playing song
    wxStatusBar* statusBar;
    wxTimer searchTimer;
    //these bitmaps are declared here because they are used in more than one method of the class
    wxBitmap pauseBitmap;
    wxBitmap playBitmap;
    wxBitmap mute;
    wxBitmap volume;
    wxBoxSizer* MainSizer;// the main sizer containing cmdSubSizer and optionsSongListSizer
    wxBoxSizer* optionsSongListSizer;//center horizontal containing optionsSizer and songList
    wxBoxSizer* controlSubSizer;//horizontal containing the control buttons and labelsSizer
    wxBoxSizer* cmdSubSizer;//horizontal containing the album art and sliderControls sizer
    wxBoxSizer* sliderControls;//vertical containing the slider and controlSubSizer
    wxBoxSizer* labelsSizer;//contains the details about the song (wxStaticText)
    wxStaticBitmap* art = nullptr;//the album art from TagReader class
    wxStaticText* titleLabel;
    wxStaticText* lengthLabel;
    wxStaticText* artistLabel;
    wxStaticText* albumLabel;
    wxStaticText* genreLabel;
    bool IsBeingDragged = false;


    void onSearch( wxCommandEvent& event );
    void onTextUpdated(wxCommandEvent& event);
    void onTextEnter(wxCommandEvent& event);
    void onTimer(wxTimerEvent& event);
    void onAdd( wxCommandEvent& event );
    void onDelete( wxCommandEvent& event);
    void onShuffle(wxCommandEvent &event);
    void onPrevious( wxCommandEvent& event );
    void onPlay( wxCommandEvent& event );
    void onNext( wxCommandEvent& event ) ;
    void onStop( wxCommandEvent& event );
    void setLoopFrame(wxCommandEvent& event);
    void onVolume(wxCommandEvent& event);
    void onPrevSession(wxCommandEvent& event);
    void onSave(wxCommandEvent &event);
    void onThumbRelease(wxScrollEvent &event) ;
    void onScrollChange(wxScrollEvent &event);
    void onAbout(wxCommandEvent &event);
    void onInstructions(wxCommandEvent& event);
    void onQuit(wxCommandEvent& event);
    void onBeginSeek(wxScrollEvent& event);
    void onEndSeek(wxScrollEvent& event);
    void onLoaded(wxMediaEvent& event);
    void onStopped(wxMediaEvent& event);
    void onListItemActivated(wxListEvent& event);//plays the selected song
    void onListItemSelected(wxListEvent& event);
    void onKillFocus(wxFocusEvent& event); // it stops searchTimer
    void onMediaSlider(wxScrollEvent& event);

    //controller
    MediaController* controller;
    Song* song;
    //model
    Playlist* playlist;




};
#endif //DAEUM_MAINFRAME_H
