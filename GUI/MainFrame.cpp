//
// Created by azrael on 30/07/17.
//

#include "MainFrame.h"

void MainFrame::update() {

}


MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : FrameInterface( parent, id, title, pos, size, style )
{


    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    unique_ptr<wxBoxSizer> mainSizer (new wxBoxSizer(wxVERTICAL));

    unique_ptr<wxBoxSizer> cmdSubSizer (new wxBoxSizer(wxHORIZONTAL));

    unique_ptr<wxSearchCtrl> searchBar(new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,30 ), 0 ));

#ifndef __WXMAC__

    searchBar->ShowSearchButton( true );

#endif


    searchBar->ShowCancelButton( true );
    cmdSubSizer->Add(searchBar.get(), 0, wxALL, 5 );

    unique_ptr<wxStaticText> addText(new wxStaticText( this, wxID_ANY, wxT("Add song from: "), wxDefaultPosition, wxDefaultSize, 0 ));
    addText->Wrap( -1 );
    cmdSubSizer->Add( addText.get(), 0, wxALL, 5 );

    unique_ptr<wxFilePickerCtrl> loadFile(new wxFilePickerCtrl( this, wxID_ANY, wxT("/home/azrael/Pictures/Webp.net-resizeimage-_1_.bmp"), wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 200,-1 ), wxFLP_DEFAULT_STYLE ));
    cmdSubSizer->Add( loadFile.get(), 0, wxALL, 5 );

    unique_ptr<wxButton> addFile(new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 ));
    cmdSubSizer->Add( addFile.get(), 0, wxALL, 5 );

    unique_ptr<wxButton> deleteFromPlaylist(new wxButton( this, wxID_ANY, wxT("Delete Selected"), wxDefaultPosition, wxDefaultSize, 0 ));
    cmdSubSizer->Add( deleteFromPlaylist.get(), 0, wxALL, 5 );

    unique_ptr<wxButton> shuffle(new wxButton( this, wxID_ANY, wxT("Play random"), wxDefaultPosition, wxDefaultSize, 0 ));
    shuffle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    cmdSubSizer->Add( shuffle.get(), 0, wxALL, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    mainSizer->Add( cmdSubSizer.get(), 0, wxALL, 5 );

    unique_ptr<wxListCtrl> songList(new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT ));
    mainSizer->Add( songList.get(), 1, wxALL|wxEXPAND, 5 );

    unique_ptr<wxMediaCtrl> mediaCtrl(new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize));
    mediaCtrl->SetPlaybackRate(1);
    mediaCtrl->SetVolume(1);

    mediaCtrl->Stop();
    mainSizer->Add( mediaCtrl.get(), 0, wxALL, 5 );

    unique_ptr<wxGauge> progressBar (new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,8 ), wxGA_HORIZONTAL|wxGA_SMOOTH ));
    progressBar->SetValue( 0 );
    progressBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );

    mainSizer->Add( progressBar.get(), 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    unique_ptr<wxBoxSizer> controlSubSizer(new wxBoxSizer(wxHORIZONTAL));

    unique_ptr<wxButton> previous(new wxButton( this, wxID_ANY, wxT("<<"), wxDefaultPosition, wxSize( 30,30 ), 0 ));
    previous->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( previous.get(), 0, 0, 5 );

    unique_ptr<wxButton> play(new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,30 ), 0 ));
    play->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( play.get(), 0, 0, 5 );

    unique_ptr<wxButton> next(new wxButton( this, wxID_ANY, wxT(">>"), wxDefaultPosition, wxSize( 30,30 ), 0 ));
    next->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( next.get(), 0, 0, 5 );

    unique_ptr<wxButton> repeat(new wxButton( this, wxID_ANY, wxT("O"), wxDefaultPosition, wxSize( 30,30 ), 0 ));
    repeat->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( repeat.get(), 0, 0, 5 );


    controlSubSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    unique_ptr<wxStaticText> volumeLabel(new wxStaticText( this, wxID_ANY, wxT("Volume: "), wxDefaultPosition, wxDefaultSize, 0 ));
    volumeLabel->Wrap( -1 );
    controlSubSizer->Add( volumeLabel.get(), 0, wxALL, 5 );

    unique_ptr<wxSlider> volume(new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 100,-1 ), wxSL_HORIZONTAL ));
    volume->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

    controlSubSizer->Add( volume.get(), 0, 0, 5 );


    mainSizer->Add( controlSubSizer.get(), 0, wxALL, 5 );


    this->SetSizer( mainSizer.get() );
    this->Layout();
    unique_ptr<wxMenuBar> menuBar(new wxMenuBar( 0 ));

    unique_ptr<wxMenu> About(new wxMenu);
    About->Append( wxID_ANY, wxString( wxT("About Daeum ...") ));

    unique_ptr<wxMenu> Quit(new wxMenu);
    Quit->Append(wxID_ANY, wxString( wxT("Exit the app ...") ));

    menuBar->Append( About.get(), wxT("About") );
    menuBar->Append( Quit.get(), wxT("Quit") );

    this->SetMenuBar( menuBar.get() );


    this->Centre( wxBOTH );

    // Connect Events
    searchBar->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    addFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRandom ), nullptr, this );
    previous->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ), nullptr, this );
    play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    repeat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRepeat ), nullptr, this );
    volume->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::volumeChanged ), nullptr, this );
    volume->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::onVolumeTrack ), nullptr, this );

}

MainFrame::~MainFrame()
{
    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    addFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRandom ), nullptr, this );
    previous->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ),nullptr, this );
    play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    repeat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRepeat ), nullptr, this );
    volume->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrame::volumeChanged ), nullptr, this );
    volume->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MainFrame::onVolumeTrack ), nullptr, this );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::wxEVT_Menu ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::wxEVT_Menu ) );

}

void MainFrame::onSearch(wxCommandEvent &event) {

}

void MainFrame::onAdd(wxCommandEvent &event) {

}

void MainFrame::onDelete(wxCommandEvent &event) {

}

void MainFrame::onRandom(wxCommandEvent &event) {

}

void MainFrame::onPrevious(wxCommandEvent &event) {

}

void MainFrame::onPlay(wxCommandEvent &event) {

}

void MainFrame::onNext(wxCommandEvent &event) {

}

void MainFrame::onRepeat(wxCommandEvent &event) {

}

void MainFrame::volumeChanged(wxScrollEvent &event) {

}

void MainFrame::onVolumeTrack(wxScrollEvent &event) {

}

void MainFrame::wxEVT_Menu(wxCommandEvent &event) {

}
