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
        //index>=0 && index < itemcount FIXME

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

void MainFrame::updateSongDetails(Song* s, Song* prevPlaying){
    play(s->getSongPath());
    wxLongLong llLength = mediaCtrl->Length();
    int nMinutes = (int) (llLength / 60000).GetValue();
    int nSeconds = (int) ((llLength % 60000) / 1000).GetValue();

    //convertion from int to string

    wxString minutes;
    wxString seconds;
    minutes << nMinutes;
    seconds << nSeconds;

    seconds=="0" ? songList->SetItem(s->getID(), 2, "0"+minutes+":0"+seconds ) : songList->SetItem(s->getID(), 2, "0"+minutes+":"+seconds );

    wxString ID;
    ID << s->getID();

    statusBar->PopStatusText(0);
    statusBar->PushStatusText(ID, 0);
    statusBar->PopStatusText(1);
    statusBar->PushStatusText(s->getTitle(), 1);
    if(prevPlaying != nullptr)
        songList->SetItemBackgroundColour(prevPlaying->getID(), GetBackgroundColour());
    if(s->getSongState() == wxMEDIASTATE_PLAYING || s->getSongState() == wxMEDIASTATE_PAUSED) {
        songList->SetItemBackgroundColour(s->getID(), wxColour(110, 140, 130));
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

    //////// Options Buttons and Songlist sizers //////

    auto optionsSongListSizer = new wxBoxSizer( wxHORIZONTAL );

    auto optionsSizer = new wxBoxSizer(wxVERTICAL);

    optionsSizer->Add( addFile, 0, wxALIGN_LEFT|wxEXPAND, 5);
    optionsSizer->Add( deleteFromPlaylist, 0, wxALIGN_LEFT|wxEXPAND, 5 );
    optionsSizer->Add( save, 0, 0, 5);
    optionsSizer->Add( prevSession, 0, 0, 5);


    optionsSongListSizer->Add(optionsSizer, 0);
    optionsSongListSizer->Add(songList, 1, wxEXPAND|wxRIGHT, 5);

    MainSizer->Add( optionsSongListSizer, 1, wxEXPAND|wxLEFT, 5 );

    /////////MediaCtrl/////////

    mediaCtrl = new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    mediaCtrl->Enable();

    //////////Media Slider//////////
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

    controlSubSizer->Add( Volume, 0, 0, 5 );

    MainSizer->Add( controlSubSizer, 0, wxALL, 5 );

    this->SetSizer( MainSizer );
    this->Layout();

    statusBar = new wxStatusBar(this);
    int widths[3] = {20, 250, 120}; // width status bar fields
    statusBar -> SetFieldsCount(3, widths);
    statusBar -> PushStatusText(wxT("#"), 0);
    statusBar -> PushStatusText(wxT("Title here"), 1);
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
    searchBar->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainFrame::onKillFocus ), nullptr, this);
    addFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ), nullptr, this );
    Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Pause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPause ), nullptr, this );
    Stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onStop ), nullptr, this );
    Loop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    prevSession->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPrevSession), nullptr, this);
    save->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onSave), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onThumbRelease), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChange), nullptr, this);
    volumeButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onVolume), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_FINISHED, wxMediaEventHandler(MainFrame::onStopped), nullptr, this);
    songList->Connect( wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Connect(aboutItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Connect(quitItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));
    this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Connect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
    mediaSlider->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onMediaSlider), nullptr, this);
}

MainFrame::~MainFrame()
{
    playlist->removeObserver(this);

    if(mediaTimer->IsRunning())
        mediaTimer -> Stop();
    if(searchTimer.IsRunning())
        searchTimer.Stop();
    delete mediaTimer;
    mediaCtrl->Stop();

    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::onTextUpdated ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::onTextEnter ), nullptr, this );
    searchBar->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainFrame::onKillFocus ), nullptr, this);
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
    Volume->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(MainFrame::onThumbRelease), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChange), nullptr, this);
    volumeButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onVolume), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onStopped), nullptr, this);
    songList->Disconnect( wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));
    this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Disconnect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
    mediaSlider->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onMediaSlider), nullptr, this);
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
    if(selectedItem != -1) {
        controller -> deleteSong(songList -> GetItemText(selectedItem, 3));
    }

}

void MainFrame::onPrevious(wxCommandEvent &event) {
    controller->prevSong();
}

void MainFrame::onPlay(wxCommandEvent &event) {
    if(mediaCtrl->GetState() == wxMEDIASTATE_PAUSED || mediaCtrl->GetState() == wxMEDIASTATE_STOPPED)
        mediaCtrl->Play();
}

void MainFrame::onPause(wxCommandEvent& event){

    if(mediaCtrl->GetState() == wxMEDIASTATE_PLAYING)
        mediaCtrl->Pause();
}

void MainFrame::onNext(wxCommandEvent &event) {
    controller->nextSong();
}

void MainFrame::onStop(wxCommandEvent &event) {
    mediaCtrl -> Stop();
}

void MainFrame::setLoopFrame(wxCommandEvent& event){
    controller->setLoop();
}

void MainFrame::onVolume(wxCommandEvent& event){
    // it goes from any volume to mute and from mute to 50 %
    auto value = Volume->GetValue();
    if(value != 0) {
        mediaCtrl->SetVolume(0);
        Volume->SetValue(0);

        wxBitmap mute;
        mute.LoadFile("../ControlsPNG/mute.png", wxBITMAP_TYPE_PNG);
        volumeButton->SetBitmap(mute);
        volumeButton->SetBackgroundColour(GetBackgroundColour());
    }
    else{
        mediaCtrl->SetVolume(0.5);
        Volume->SetValue(50);

        wxBitmap volume;
        volume.LoadFile("../ControlsPNG/volume.png", wxBITMAP_TYPE_PNG);
        volumeButton->SetBitmap(volume);
        volumeButton->SetBackgroundColour(GetBackgroundColour());
    }
}

void MainFrame::onPrevSession(wxCommandEvent &event) {
    controller->load();
}

void MainFrame::onSave(wxCommandEvent &event) {
    controller->save();
}

void MainFrame::onThumbRelease(wxScrollEvent &event) {
    //mediaCtrl->SetVolume(Volume->GetValue() / 100.0 );
}

void MainFrame::onScrollChange(wxScrollEvent &event) {
    mediaCtrl->SetVolume(Volume->GetValue() / 100.0 );
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
    controller->nextSong();
}
void MainFrame::onListItemActivated(wxListEvent &event) {
    controller->tellPlaylist(songList->GetItemText(event.GetIndex(), 3));
}

void MainFrame::onListItemSelected(wxListEvent &event){

}

void MainFrame::play(wxString path){
    if(path != wxEmptyString)
        mediaCtrl->Load(path);
}

void MainFrame::onMediaSlider(wxScrollEvent &event){
    if( mediaCtrl->Seek(mediaSlider->GetValue() * 1000) == wxInvalidOffset )
        wxMessageBox(wxT("Couldn't seek!"));
    IsBeingDragged = false;
}

