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
#include "FrameInterface.h"

using namespace std;

class MainFrame : public FrameInterface{
public:
    void update() override;


    MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,373 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    ~MainFrame();


protected:
        unique_ptr<wxSearchCtrl> searchBar;
        unique_ptr<wxStaticText> addText;
        unique_ptr<wxFilePickerCtrl> loadFile;
        unique_ptr<wxButton> addFile;
        unique_ptr<wxButton> deleteFromPlaylist;
        unique_ptr<wxButton> shuffle;
        unique_ptr<wxListCtrl> songList;
        unique_ptr<wxMediaCtrl> mediaCtrl;
        unique_ptr<wxGauge> progressBar;
        unique_ptr<wxButton> previous;
        unique_ptr<wxButton> play;
        unique_ptr<wxButton> next;
        unique_ptr<wxButton> repeat;
        unique_ptr<wxStaticText> volumeLabel;
        unique_ptr<wxSlider> volume;
        unique_ptr<wxMenuBar> menuBar;
        unique_ptr<wxMenu> About;
        unique_ptr<wxMenu> Quit;


        void onSearch( wxCommandEvent& event ) ;
        void onAdd( wxCommandEvent& event ) ;
        void onDelete( wxCommandEvent& event ) ;
        void onRandom( wxCommandEvent& event ) ;
        void onPrevious( wxCommandEvent& event ) ;
        void onPlay( wxCommandEvent& event ) ;
        void onNext( wxCommandEvent& event ) ;
        void onRepeat( wxCommandEvent& event ) ;
        void volumeChanged( wxScrollEvent& event ) ;
        void onVolumeTrack( wxScrollEvent& event ) ;
        void wxEVT_Menu( wxCommandEvent& event ) ;

};
#endif //DAEUM_MAINFRAME_H
