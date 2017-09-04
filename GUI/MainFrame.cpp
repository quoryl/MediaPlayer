//
// Created by azrael on 30/07/17.
//

#include <chrono>
#include "MainFrame.h"


void MainFrame::update(list<Song*>& playList){
    songList->DeleteAllItems();
    for(auto iter: playList){

        wxString path = iter -> getSongPath();
        wxListItem listItem;
        listItem.SetAlign(wxLIST_FORMAT_LEFT);

        long nID = iter->getID();
        //cast an int/long to a wxString
        wxString s;
        s << nID;

        wxString length;
        length << iter->getLength();

        //for the next line: I should write only nID, not initialize it with getitemcount
        //for some reason that makes an assertion fail: invalid item index
        //index>=0 && index <itemcount FIXME

        listItem.SetId(nID = songList->GetItemCount());
        listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_STATE);
        listItem.SetData( iter );

        songList->InsertItem(listItem);
        songList->SetItem(nID, 0, s);
        songList->SetItem(nID, 1, iter->getTitle());
        songList->SetItem(nID, 2, length);
        songList->SetItem(nID, 3, path);
    }
}

void MainFrame::updateSongDetails(Song* s){

    wxLongLong llLength = mediaCtrl->Length();
    int nMinutes = (int) (llLength / 60000).GetValue();
    int nSeconds = (int) ((llLength % 60000) / 1000).GetValue();

    //convertion from int to string

    wxString minutes;
    wxString seconds;
    minutes << nMinutes;
    seconds << nSeconds;

    //TODO maybe I could save the length in an array or something similar and display from there

    if(seconds == "0")
        songList->SetItem(s->getID(), 2, "0"+minutes+":0"+seconds );
    else
        songList->SetItem(s->getID(), 2, "0"+minutes+":"+seconds );
    songList->SetItemTextColour(s->getID(), wxColour(97,119,136));
    statusBar->PopStatusText(0);
    statusBar->PushStatusText(s->getTitle(), 0);

    if(s->isLoop()) {
        statusBar->PopStatusText(2);
        statusBar->PushStatusText(wxT("Is being looped: True"), 2);
    }
    else{
        statusBar->PopStatusText(2);
        statusBar->PushStatusText(wxT("Is being looped: False"), 2);
    }

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

    searchBar = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,30 ), wxTE_PROCESS_ENTER );
#ifndef __WXMAC__
    searchBar->ShowSearchButton( true );
#endif
    searchBar->ShowCancelButton( true );
    cmdSubSizer->Add( searchBar, 0, wxALL, 5 );

    //////////SearchTimer////////////

    searchTimer.SetOwner(this, wxID_ANY);

    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    MainSizer->Add( cmdSubSizer, 0, wxALL, 5 );

    //////////ListCtrl//////////

    songList = new wxListView( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
    songList->InsertColumn(0,wxT("#"),wxLIST_FORMAT_LEFT, 25);
    songList->InsertColumn(1,wxT("Title"), wxLIST_FORMAT_LEFT, 250);
    songList->InsertColumn(2,wxT("Length"), wxLIST_FORMAT_LEFT, 150);
    songList->InsertColumn(3,wxT("Location"), wxLIST_FORMAT_LEFT, 400);
    songList->SetBackgroundColour(GetBackgroundColour());

    //////////FileDialog//////////

    loadFile = new wxFileDialog( this,wxT("Select a file"),wxT("/home/azrael/Music"),wxEmptyString, wxT("*.mp3"),wxFD_MULTIPLE|wxFD_OPEN);

    //addFile = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxSize(205,-1), 0 );
    wxBitmap addPNG;
    addPNG.LoadFile("../ControlsPNG/add_text.png", wxBITMAP_TYPE_PNG);

    addFile = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(205,35), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    addFile->SetBitmap(addPNG);
    addFile->SetBackgroundColour(GetBackgroundColour());
    /////////DeleteItem/////////
    wxBitmap deletePNG;
    deletePNG.LoadFile("../ControlsPNG/delete_text.png", wxBITMAP_TYPE_PNG);

    deleteFromPlaylist = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(205,35), wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    deleteFromPlaylist->SetBitmap(deletePNG);
    deleteFromPlaylist->SetBackgroundColour(GetBackgroundColour());
    ////////Loads previous session(saved by the user)/////////

    wxBitmap loadPNG;
    loadPNG.LoadFile("../ControlsPNG/load_text.png", wxBITMAP_TYPE_PNG);

    prevSession = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(205,35), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    prevSession->SetBitmap(loadPNG);
    prevSession->SetBackgroundColour(GetBackgroundColour());

    ////////Saves session/////////

    wxBitmap savePNG;
    savePNG.LoadFile("../ControlsPNG/save_text.png", wxBITMAP_TYPE_PNG);

    save = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(205,35), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    save->SetBitmap(savePNG);
    save->SetBackgroundColour(GetBackgroundColour());

    //////////CollapsiblePane//////

    auto paneSizer = new wxBoxSizer( wxHORIZONTAL );
    pane = new wxCollapsiblePane(this,wxID_ANY, wxEmptyString ,wxDefaultPosition, wxSize(-1,-1));

    auto paneButtonSz = new wxBoxSizer(wxVERTICAL);
    paneButtonSz->Add(pane,0);

    paneButtonSz->Add( addFile, 0, wxALIGN_LEFT|wxEXPAND, 5);
    paneButtonSz->Add( deleteFromPlaylist, 0, wxALIGN_LEFT|wxEXPAND, 5 );
    paneButtonSz->Add( save, 0, 0, 5);
    paneButtonSz->Add( prevSession, 0, 0, 5);


    paneSizer->Add(paneButtonSz, 0);
    paneSizer->Add(songList, 1, wxEXPAND|wxRIGHT, 5);

    wxWindow *win = pane->GetPane();
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);

    win->SetSizer(paneSz);
    paneSz->SetSizeHints(win);

    MainSizer->Add( paneSizer, 1, wxEXPAND|wxLEFT, 5 );

    /////////MediaCtrl/////////

    mediaCtrl = new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    mediaCtrl->Enable();

    //////////Slider//////////
    mediaSlider = new wxSlider(this, wxID_ANY, 0, 0, 10);

    MainSizer->Add(mediaSlider, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

    wxBoxSizer* controlSubSizer;
    controlSubSizer = new wxBoxSizer( wxHORIZONTAL );

    /////////Controls////////
    mediaTimer = new MediaTimer(this);
    mediaTimer->Start(500);

    wxBitmap prev;
    prev.LoadFile("../ControlsPNG/previous.png", wxBITMAP_TYPE_PNG);

    Previous = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, prev.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    Previous->SetBitmap(prev);
    Previous->SetBackgroundColour(GetBackgroundColour());
    controlSubSizer->Add( Previous, 0, 0, 5 );

    wxBitmap play;
    play.LoadFile("../ControlsPNG/play-button.png", wxBITMAP_TYPE_PNG);

    Play = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, play.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Play->SetBitmap(play);
    Play->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Play, 0, 0, 5 );

    wxBitmap pause;
    pause.LoadFile("../ControlsPNG/pause.png", wxBITMAP_TYPE_PNG);

    Pause = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, pause.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Pause->SetBitmap(pause);
    Pause->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Pause, 0, 0, 5 );

    wxBitmap next;
    next.LoadFile("../ControlsPNG/next.png", wxBITMAP_TYPE_PNG);

    Next = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, next.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Next->SetBitmap(next);
    Next->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Next, 0, 0, 5 );

    wxBitmap stop;
    stop.LoadFile("../ControlsPNG/stop.png", wxBITMAP_TYPE_PNG);

    Stop = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, stop.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Stop->SetBitmap(stop);
    Stop->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Stop, 0, 0, 5 );

    wxBitmap loop;
    loop.LoadFile("../ControlsPNG/repeat.png", wxBITMAP_TYPE_PNG);

    Loop = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, loop.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Loop->SetBitmap(loop);
    Loop->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Loop, 0, 0, 5 );

    wxBitmap shufflePNG;
    shufflePNG.LoadFile("../ControlsPNG/shuffle.png", wxBITMAP_TYPE_PNG);

    shuffle = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, shufflePNG.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    shuffle->SetBitmap(shufflePNG);
    shuffle->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( shuffle, 0, 0, 5 );


    controlSubSizer->Add( 10, 0, 1, wxEXPAND, 5 );


    controlSubSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    wxBitmap volume;
    volume.LoadFile("../ControlsPNG/volume.png", wxBITMAP_TYPE_PNG);

    volumeButton = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, volume.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    volumeButton->SetBitmap(volume);
    volumeButton->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( volumeButton, 0, 0, 5 );
    //TODO connect volume button to an event handler

    Volume = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 100,-1 ), wxSL_HORIZONTAL );
    Volume->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

    controlSubSizer->Add( Volume, 0, 0, 5 );


    MainSizer->Add( controlSubSizer, 0, wxALL, 5 );


    this->SetSizer( MainSizer );
    this->Layout();

    statusBar = new wxStatusBar(this);
    int widths[3] = {200, 120, 300}; // width status bar fiels
    statusBar -> SetFieldsCount(3, widths);
    statusBar -> PushStatusText(wxT("You will see the title here"), 0);
    statusBar -> PushStatusText(wxT("Is being looped: False"), 2);
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
    searchBar->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::onTextUpdated ), nullptr, this );
    searchBar->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::onTextEnter ), nullptr, this );
    addFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ), nullptr, this );
    Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onStop ), nullptr, this );
    Pause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPause ), nullptr, this );
    Loop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    prevSession->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPrevSession), nullptr, this);
    save->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onSave), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onStopped), nullptr, this);
    songList->Connect( wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Connect(aboutItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Connect(quitItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));
    this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Connect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
    searchBar->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainFrame::onKillFocus ), nullptr, this);

}



void MainFrame::onSearch(wxCommandEvent &event) {
    controller->searchItem(searchBar -> GetLineText(0));
}

void MainFrame::onTimer(wxTimerEvent &event){ //I could have used onSearch as the function called periodically but the event type is different
    controller->searchItem(searchBar -> GetLineText(0));
}

void MainFrame::onTextUpdated(wxCommandEvent &event) {
    if(!searchTimer.IsRunning()){
        searchTimer.Start(500);
    }
}
void MainFrame::onTextEnter(wxCommandEvent &event){
    controller->searchItem(searchBar->GetLineText(0));
}
void MainFrame::onKillFocus(wxFocusEvent &event){
    if(searchTimer.IsRunning())
        searchTimer.Stop();
}

void MainFrame::onAdd(wxCommandEvent &event) {
    loadFile->ShowModal();
    wxArrayString savePaths;
    loadFile -> GetPaths(savePaths);
    controller->addFile(&savePaths);
}

void MainFrame::onShuffle(wxCommandEvent &event) {
    controller->shuffleList();
}



void MainFrame::onDelete(wxCommandEvent &event) {

    long selectedItem = songList -> GetFirstSelected();
    cout<<selectedItem<<endl;
    while(selectedItem != -1) {
        controller -> deleteSong(songList -> GetItemText(selectedItem,3));
        if(selectedItem != songList->GetItemCount()) {
            selectedItem = songList -> GetNextSelected(selectedItem);
            cout<<selectedItem<<"from next"<<endl;
        }
        else{
            selectedItem = -1;//breaks the cycle and exits
            cout<<"from break"<<endl;
        }
    }

}

void MainFrame::onPrevious(wxCommandEvent &event) {
    controller->prevSong();
}

void MainFrame::onPlay(wxCommandEvent &event) {
    if(mediaCtrl->GetState() == wxMEDIASTATE_PAUSED)
        mediaCtrl->Play();
}

void MainFrame::onPause(wxCommandEvent& event){

    if(mediaCtrl->GetState() == wxMEDIASTATE_PLAYING)
        mediaCtrl->Pause();
}

void MainFrame::onNext(wxCommandEvent &event) {

}

void MainFrame::onStop(wxCommandEvent &event) {
    mediaCtrl -> Stop();
}

void MainFrame::setLoopFrame(wxCommandEvent& event){
    controller->setLoop();
}

void MainFrame::onPrevSession(wxCommandEvent &event) {
    controller->load();
}

void MainFrame::onSave(wxCommandEvent &event) {
    controller->save();
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

void MainFrame::onStopped(wxMediaEvent& event){

}
void MainFrame::onListItemActivated(wxListEvent &event) {

    mediaCtrl->Load(songList->GetItemText(event.GetIndex(), 3));
    controller->tellPlaylist(songList->GetItemText(event.GetIndex(), 3));

}

void MainFrame::onListItemSelected(wxListEvent &event){

}
MainFrame::~MainFrame()
{
    playlist->removeObserver(this);

    if(mediaTimer->IsRunning())
        mediaTimer -> Stop();
    if(searchTimer.IsRunning())
        searchTimer.Stop();
    delete mediaTimer;

    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::onTextUpdated ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::onTextEnter ), nullptr, this );
    addFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ),nullptr, this );
    Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Pause->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPause ), nullptr, this );
    Next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Stop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onStop ), nullptr, this );
    Loop->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    prevSession->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPrevSession), nullptr, this);
    save->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onSave), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onStopped), nullptr, this);
    songList->Disconnect( wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));
    this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Disconnect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
}


