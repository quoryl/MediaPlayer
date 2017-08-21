//
// Created by azrael on 30/07/17.
//

#include <chrono>
#include "MainFrame.h"
#include "../Model/Playlist.h"

void MainFrame::update(list<Song*>& playList){
    songList->DeleteAllItems();
    for(auto iter: playList){
        wxString path = iter -> getSongPath();

        wxListItem listItem;
        listItem.SetAlign(wxLIST_FORMAT_LEFT);

        int nID ;

        listItem.SetId(nID = songList->GetItemCount());
        listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_STATE);
        //listItem.SetData(new wxString(path));
        listItem.SetData( iter );

        //cast an int to a wxString

        wxString s;
        s << nID;


        songList->InsertItem(listItem);
        songList->SetItem(nID, 0, s);
        songList->SetItem(nID, 1, iter->getTitle());
        songList->SetItem(nID, 2, iter->getArtist());
        songList->SetItem(nID, 3, wxT("Unknown"));
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

    //////////FilePicker//////////

    addText = new wxStaticText( this, wxID_ANY, wxT("Add song : "), wxDefaultPosition, wxDefaultSize, 0 );
    addText->Wrap( -1 );
    cmdSubSizer->Add( addText, 0, wxALL, 5 );

    loadFile = new wxFileDialog( this,wxT("Select a file"),wxT("/home/azrael/Music"),wxEmptyString, wxT("*.mp3"),wxFD_MULTIPLE|wxFD_OPEN);

    cmdSubSizer->Add( loadFile, 0, wxALL, 5 );


    addFile = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( addFile, 0, wxALL, 5 );

    /////////DeleteItem/////////

    deleteFromPlaylist = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    cmdSubSizer->Add( deleteFromPlaylist, 0, wxALL, 5 );



    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    cmdSubSizer->Add( 0, 0, 1, wxEXPAND, 5 );


    MainSizer->Add( cmdSubSizer, 0, wxALL, 5 );

    //////////ListCtrl//////////

    songList = new wxListView( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
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

    wxBitmap next;
    next.LoadFile("../ControlsPNG/next.png", wxBITMAP_TYPE_PNG);

    Next = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, next.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Next->SetBitmap(next);
    Next->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Next, 0, 0, 5 );

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

    prevSession = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxSize( 60,30 ), 0 );
    prevSession->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
    controlSubSizer->Add( prevSession, 0, 0, 5 );

    controlSubSizer->Add( 10, 0, 1, wxEXPAND, 5 );

    save = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize( 60,30 ), 0 );
    save->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
    controlSubSizer->Add( save, 0, 0, 5 );


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
    int widths[4] = {200, 60, 60, 300};
    statusBar -> SetFieldsCount(4, widths);
    statusBar -> PushStatusText(wxT("You will see the title here"), 0);
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
    searchBar->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::onTextUpdated ), nullptr, this );
    searchBar->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::onTextEnter ), nullptr, this );
    addFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ), nullptr, this );
    Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Loop->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    prevSession->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPrevSession), nullptr, this);
    save->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onSave), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Connect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onLoop), nullptr, this);
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
    controller->addFile(&savePaths, mediaCtrl);
}

void MainFrame::onShuffle(wxCommandEvent &event) {
    if(songList->GetItemCount() == 0) {
        wxMessageBox("You can't shuffle an empty playlist!");
        return;
    }
    long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::vector<long>indexes;
    for (long i = 0; i < songList->GetItemCount(); i++) {
        indexes.push_back(i);

    }

    std::shuffle(indexes.begin(), indexes.end(), std::default_random_engine(seed));

    for(auto i : indexes) {
        cout<< " " <<i;
    }
    cout<<"\n"<<endl;
    controller->shuffleList();
    indexes.clear();
}



void MainFrame::onDelete(wxCommandEvent &event) {
    long selectedItem = songList -> GetFirstSelected();
    cout<<selectedItem<<endl;
    while(selectedItem != -1) {
        controller -> deleteSong(songList -> GetItemText(selectedItem,1));
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
    controller->playSong();
}

void MainFrame::onNext(wxCommandEvent &event) {
    controller->nextSong();
}

void MainFrame::setLoopFrame(wxCommandEvent& event){
    controller->setLoop();
}

void MainFrame::onLoop(wxMediaEvent& event){
    controller -> loop(mediaCtrl);
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

}

void MainFrame::onListItemActivated(wxListEvent &event) {
    mediaCtrl->Play();
}

void MainFrame::onListItemSelected(wxListEvent &event){
    {
        statusBar->PopStatusText(0);
        statusBar->PushStatusText(songList->GetItemText(event.GetIndex(), 1));
    }


}
MainFrame::~MainFrame()
{
    playlist->removeObserver(this);

    if(mediaTimer->IsRunning())
        mediaTimer -> Stop();
    if(searchTimer.IsRunning())
        searchTimer.Stop();

    // Disconnect Events
    searchBar->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( MainFrame::onSearch ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainFrame::onTextUpdated ), nullptr, this );
    searchBar->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::onTextEnter ), nullptr, this );
    addFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onAdd ), nullptr, this );
    deleteFromPlaylist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onDelete ), nullptr, this );
    shuffle->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onShuffle), nullptr, this);
    Previous->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPrevious ),nullptr, this );
    Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onPlay ), nullptr, this );
    Next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::onNext ), nullptr, this );
    Loop->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::setLoopFrame), nullptr, this);
    prevSession->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPrevSession), nullptr, this);
    save->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onSave), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(MainFrame::onScrollTrack), nullptr, this);
    Volume->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onScrollChanged), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_LOADED, wxMediaEventHandler(MainFrame::onLoaded), nullptr, this);
    mediaCtrl->Disconnect(wxEVT_MEDIA_STOP, wxMediaEventHandler(MainFrame::onLoop), nullptr, this);
    songList->Disconnect( wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::onListItemActivated), nullptr, this );
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onAbout));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onQuit));
    this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Disconnect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
}


