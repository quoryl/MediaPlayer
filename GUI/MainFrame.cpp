//
// Created by azrael on 30/07/17.
//

#include "MainFrame.h"
#include "../Model/Playlist.h"

void MainFrame::update(list<Song *> &playList) {

}

MainFrame::MainFrame(MediaController *mediaController,
                     Playlist *pList, wxWindow *parent, wxWindowID id,const wxString &title, const wxPoint &pos,
                      const wxSize &size, long style): wxFrame( parent, id, title, pos, size, style )
{
    playlist = pList;
    MainFrame::controller = mediaController;
    pList->registerObserver(this);

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );


    wxBoxSizer* MainSizer;
    MainSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* cmdSubSizer;
    cmdSubSizer = new wxBoxSizer( wxHORIZONTAL );

    ////////// SearchBar///////////

    searchBar = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,30 ), 0 );
#ifndef __WXMAC__
    searchBar->ShowSearchButton( true );
#endif
    searchBar->ShowCancelButton( true );
    cmdSubSizer->Add( searchBar, 0, wxALL, 5 );

    //////////FilePicker//////////

    addText = new wxStaticText( this, wxID_ANY, wxT("Add song from: "), wxDefaultPosition, wxDefaultSize, 0 );
    addText->Wrap( -1 );
    cmdSubSizer->Add( addText, 0, wxALL, 5 );

    loadFile = new wxFilePickerCtrl( this, wxID_ANY, wxT("*.*"), wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxSize( 200,-1 ), wxFLP_DEFAULT_STYLE );

    cmdSubSizer->Add( loadFile, 0, wxALL, 5 );


    addFile = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( addFile, 0, wxALL, 5 );

    /////////DeleteItem/////////

    deleteFromPlaylist = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( deleteFromPlaylist, 0, wxALL, 5 );


    ////////Shuffle/////////

    shuffle = new wxButton( this, wxID_ANY, wxT("Random"), wxDefaultPosition, wxDefaultSize, 0 );
    shuffle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    cmdSubSizer->Add( shuffle, 0, wxALL, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    MainSizer->Add( cmdSubSizer, 0, wxALL, 5 );

    //////////ListCtrl//////////

    songList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
    songList->InsertColumn(0,wxT("#"),wxLIST_FORMAT_LEFT, 25);
    songList->InsertColumn(1,wxT("Title"), wxLIST_FORMAT_LEFT, 250);
    songList->InsertColumn(2,wxT("Artist"), wxLIST_FORMAT_LEFT, 150);
    songList->InsertColumn(3,wxT("Length"), wxLIST_FORMAT_LEFT, 150);

    MainSizer->Add( songList, 1, wxALL|wxEXPAND, 5 );

    /////////MediaCtrl/////////

    mediaCtrl = new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    //mediaCtrl->SetPlaybackRate(1);
    //mediaCtrl->SetVolume(1);
    //mediaCtrl->Stop();
    MainSizer->Add( mediaCtrl, 0, wxALL, 5 );

    //////////Slider//////////
    mediaSlider = new wxSlider(this, wxID_ANY, 0, 0, 10);


    MainSizer->Add(mediaSlider, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );



    wxBoxSizer* controlSubSizer;
    controlSubSizer = new wxBoxSizer( wxHORIZONTAL );

    /////////Controls////////
    mediaTimer = new MediaTimer(this);
    mediaTimer->Start(500);

    Previous = new wxButton( this, wxID_ANY, wxT("<<"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Previous->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Previous, 0, 0, 5 );

    Play = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Play->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Play, 0, 0, 5 );

    Next = new wxButton( this, wxID_ANY, wxT(">>"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Next->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    controlSubSizer->Add( Next, 0, 0, 5 );

    Loop = new wxButton( this, wxID_ANY, wxT("O"), wxDefaultPosition, wxSize( 30,30 ), 0 );
    Loop->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
    controlSubSizer->Add( Loop, 0, 0, 5 );

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

    statusBar = new wxStatusBar(this);
    int widths[4] = {200, 60, 60, 300};
    statusBar -> SetFieldsCount(4, widths);
    statusBar -> SetStatusText(wxT("Title here"), 0);
    statusBar -> PushStatusText(wxT("Is being looped: False"), 3);
    this->SetStatusBar(statusBar);
    /////////Menu//////////

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
    Loop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onLoop), nullptr, this);
    songList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Connect(aboutItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Connect(quitItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));


}



void MainFrame::onSearch(wxCommandEvent &event) {
    controller->searchItem(searchBar -> GetLineText(0));
}

void MainFrame::onAdd(wxCommandEvent &event) {
    controller->addFile(loadFile, songList, mediaCtrl);
}

void MainFrame::onShuffle(wxCommandEvent &event) {
    controller->shuffleList();
}

void MainFrame::onDelete(wxCommandEvent &event) {
    controller->deleteSong();
}

void MainFrame::onPrevious(wxCommandEvent &event) {
    controller->prevSong();
}

void MainFrame::onPlay(wxCommandEvent &event) {
    controller->playSong();
}

void MainFrame::setLoopFrame(wxCommandEvent& event){
    controller->setLoop();
}

void MainFrame::onLoop(wxMediaEvent& event){
    controller -> loop(mediaCtrl);
}

void MainFrame::onNext(wxCommandEvent &event) {
    controller->nextSong();
}

void MainFrame::onScrollTrack(wxScrollEvent &event) {
    controller->showVolume();
}

void MainFrame::onScrollChanged(wxScrollEvent &event) {
    controller->changeVolume();
}

void MainFrame::onAbout(wxCommandEvent &event) {
    controller->showAbout();
}
void MainFrame::onQuit(wxCommandEvent &event){
    Close();
}
void MainFrame::onBeginSeek(wxScrollEvent& event)
{
    IsBeingDragged = true;
}

void MainFrame::onEndSeek(wxScrollEvent &event) {
    IsBeingDragged = false;
}

void MainFrame::onLoaded(wxMediaEvent &event) {
    mediaCtrl->Play();
}

void MainFrame::onListItemActivated(wxListEvent &event) {
    mediaCtrl->Play();
}

MainFrame::~MainFrame()
{
    playlist->removeObserver(this);
    mediaTimer -> Stop();

    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    addFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ),nullptr, this );
    Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Loop->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onLoop), nullptr, this);
    songList->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));

}


