//
// Created by azrael on 30/07/17.
//

#include "MainFrame.h"

void MainFrame::update() {

}

MainFrame::MainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size,
                     long style) : Observer(parent, id, title, pos, size, style )
{


    song=new Song;

    song->registerObserver(this);

    mediaController=new MediaController(song);

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* MainSizer;
    MainSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* cmdSubSizer;
    cmdSubSizer = new wxBoxSizer( wxHORIZONTAL );

    searchBar = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,30 ), 0 );
#ifndef __WXMAC__
    searchBar->ShowSearchButton( true );
#endif
    searchBar->ShowCancelButton( true );
    cmdSubSizer->Add( searchBar, 0, wxALL, 5 );

    addText = new wxStaticText( this, wxID_ANY, wxT("Add song from: "), wxDefaultPosition, wxDefaultSize, 0 );
    addText->Wrap( -1 );
    cmdSubSizer->Add( addText, 0, wxALL, 5 );

    loadFile = new wxFilePickerCtrl( this, wxID_ANY, wxT("/home/azrael/Pictures/Webp.net-resizeimage-_1_.bmp"), wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 200,-1 ), wxFLP_DEFAULT_STYLE );
    cmdSubSizer->Add( loadFile, 0, wxALL, 5 );

    addFile = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( addFile, 0, wxALL, 5 );

    deleteFromPlaylist = new wxButton( this, wxID_ANY, wxT("Delete Selected"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( deleteFromPlaylist, 0, wxALL, 5 );

    shuffle = new wxButton( this, wxID_ANY, wxT("Play random"), wxDefaultPosition, wxDefaultSize, 0 );
    shuffle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    cmdSubSizer->Add( shuffle, 0, wxALL, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    MainSizer->Add( cmdSubSizer, 0, wxALL, 5 );

    SongList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
    MainSizer->Add( SongList, 1, wxALL|wxEXPAND, 5 );

    mediaCtrl = new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    mediaCtrl->SetPlaybackRate(1);
    mediaCtrl->SetVolume(1);

    mediaCtrl->Stop();
    MainSizer->Add( mediaCtrl, 0, wxALL, 5 );

    progressBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,8 ), wxGA_HORIZONTAL|wxGA_SMOOTH );
    progressBar->SetValue( 0 );
    progressBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );

    MainSizer->Add( progressBar, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* controlSubSizer;
    controlSubSizer = new wxBoxSizer( wxHORIZONTAL );

    Previous = new wxButton( this, wxID_ANY, wxT("<<"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Previous->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Previous, 0, 0, 5 );

    Play = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Play->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Play, 0, 0, 5 );

    Next = new wxButton( this, wxID_ANY, wxT(">>"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Next->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Next, 0, 0, 5 );

    Repeat = new wxButton( this, wxID_ANY, wxT("O"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Repeat->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Repeat, 0, 0, 5 );


    controlSubSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    volumeLabel = new wxStaticText( this, wxID_ANY, wxT("Volume: "), wxDefaultPosition, wxDefaultSize, 0 );
    volumeLabel->Wrap( -1 );
    controlSubSizer->Add( volumeLabel, 0, wxALL, 5 );

    Volume = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 100,-1 ), wxSL_HORIZONTAL );
    Volume->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

    controlSubSizer->Add( Volume, 0, 0, 5 );


    MainSizer->Add( controlSubSizer, 0, wxALL, 5 );


    this->SetSizer( MainSizer );
    this->Layout();
    menuBar = new wxMenuBar( 0 );
    About = new wxMenu();
    wxMenuItem* aboutItem;
    aboutItem = new wxMenuItem( About, wxID_ANY, wxString( wxT("About Daeum ...") ) , wxEmptyString, wxITEM_NORMAL );
    About->Append( aboutItem );

    menuBar->Append( About, wxT("About") );

    Quit = new wxMenu();
    wxMenuItem* quitItem;
    quitItem = new wxMenuItem( Quit, wxID_ANY, wxString( wxT("Exit the app ...") ) , wxEmptyString, wxITEM_NORMAL );
    Quit->Append( quitItem );

    menuBar->Append( Quit, wxT("Quit") );

    this->SetMenuBar( menuBar );


    this->Centre( wxBOTH );

    // Connect Events
    searchBar->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    addFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ), nullptr, this );
    Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Repeat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRepeat ),nullptr, this );
    Volume->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    this->Connect( aboutItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::onMenuItem ) );
    this->Connect( quitItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::onMenuItem ) );
}

MainFrame::~MainFrame()
{
    song->removeObserver(this);
    delete song;
    delete mediaController;
    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    addFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ),nullptr, this );
    Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Repeat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onRepeat ), nullptr, this );
    Volume->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::onMenuItem ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::onMenuItem ) );

}

void MainFrame::onSearch(wxCommandEvent &event) {
    mediaController->Search();
}

void MainFrame::onAdd(wxCommandEvent &event) {
    mediaController->Add();
}

void MainFrame::onShuffle(wxCommandEvent &event) {
    mediaController->Shuffle();
}

void MainFrame::onDelete(wxCommandEvent &event) {
    mediaController->Delete();
}

void MainFrame::onPrevious(wxCommandEvent &event) {
    mediaController->Previous();
}

void MainFrame::onPlay(wxCommandEvent &event) {
    mediaController->Play();
}

void MainFrame::onNext(wxCommandEvent &event) {
    mediaController->Next();
}

void MainFrame::onRepeat(wxCommandEvent &event) {
    mediaController->Repeat();
}

void MainFrame::onScrollTrack(wxScrollEvent &event) {
    mediaController->showVolume();
}

void MainFrame::onScrollChanged(wxScrollEvent &event) {
    mediaController->changeVolume();
}

void MainFrame::onMenuItem(wxCommandEvent &event) {
    mediaController->executeMenuItem();
}





