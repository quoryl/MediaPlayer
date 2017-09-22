//
// Created by azrael on 30/07/17.
//

#include "MainFrame.h"


void MainFrame::update(list<Song *> &playList, Song *playing) {
    songList->DeleteAllItems();
    for (auto iter: playList) {
        wxString path = iter->getSongPath();
        wxListItem listItem;

        listItem.SetAlign(wxLIST_FORMAT_LEFT);
        long nID = songList->GetItemCount();
        iter -> setID(nID);
        //cast an int/long to a wxString
        wxString s;
        s << nID;

        listItem.SetId(nID);
        listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_STATE);
        listItem.SetData(iter);

        songList->InsertItem(listItem);
        songList->SetItem(nID, 0, s);
        songList->SetItem(nID, 1, iter->getTitle());
        songList->SetItem(nID, 2, iter->getArtist());
        songList->SetItem(nID, 3, iter->getAlbum());
        songList->SetItem(nID, 4, path);

        if(playing == iter && playing != nullptr)
            songList->SetItemBackgroundColour(playing->getID(), wxColour(110, 140, 130));
    }

}

void MainFrame::updateSongDetails(Song* s, Song* prevPlaying){
    if(s!=nullptr) {
        play(s->getSongPath());
        wxString ID;
        ID << s->getID();

        statusBar->PopStatusText(0);
        statusBar->PushStatusText(ID, 0);
        statusBar->PopStatusText(1);
        statusBar->PushStatusText(s->getTitle(), 1);

        if (prevPlaying != nullptr)
            songList->SetItemBackgroundColour(prevPlaying->getID(), GetBackgroundColour());
        if (s->getSongState() == wxMEDIASTATE_PLAYING || s->getSongState() == wxMEDIASTATE_PAUSED) {
            songList->SetItemBackgroundColour(s->getID(), wxColour(110, 140, 130));
        }
        if(s->isLoop()) {
            Loop->SetBackgroundColour(wxColour(110, 140, 130));
        }
        else
            Loop->SetBackgroundColour(GetBackgroundColour());

        if( art!=nullptr )
            cmdSubSizer->Detach(art);

        //The conversion was done because wxBitmap does not have a rescale method ( the difference with wxImage )
        auto imageAlbum = s->getAlbumArt().ConvertToImage();
        imageAlbum.Rescale(170,170);
        wxBitmap fromImage(imageAlbum);

        art = new wxStaticBitmap(this, wxID_ANY, fromImage, wxDefaultPosition);
        cmdSubSizer->Prepend(art, 0, wxALL);
        MainSizer->Layout();

        titleLabel->SetLabel(wxT("Title: ") + s->getTitle());
        artistLabel->SetLabel(wxT("Artist: ") + s->getArtist());
        albumLabel->SetLabel(wxT("Album: ") + s->getAlbum());
        genreLabel->SetLabel(wxT("Genre: ") + s->getGenre());


    }

}

MainFrame::MainFrame(MediaController *mediaController,
                     Playlist *pList, wxWindow *parent, wxWindowID id,const wxString &title, const wxPoint &pos,
                     const wxSize &size, long style): wxFrame( parent, id, title, pos, size, style )
{
    SetIcon(wxIcon(wxT("../ControlsPNG/musicPlayerIcon.xpm")));
    playlist = pList;
    MainFrame::controller = mediaController;
    pList->registerObserver(this);


    /*
        You should associate wxDropTarget and wxWindow calling SetDropTarget:
            wxWindow *pWindow = GetTopWindow();
            pWindow->SetDropTarget(new MyDropTarget);

        The object created passed to SetDropTarget becomes the propriety of wxWindow
        and will be deleted with the window (or when you call SetDropTarget next
        time). You can always break the association by calling SetDropTarget(NULL).

    */
    SetDropTarget(new DragAndDrop(this));

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    MainSizer = new wxBoxSizer( wxVERTICAL );

    cmdSubSizer = new wxBoxSizer( wxHORIZONTAL );

    sliderControls = new wxBoxSizer( wxVERTICAL );

    cmdSubSizer->Add(sliderControls, 1, wxEXPAND|wxALL, 5);

    ////////Media Slider//////////

    mediaSlider = new wxSlider(this, wxID_ANY, 0, 0, 10);

    sliderControls->Add(mediaSlider, 0, wxALL|wxEXPAND, 5 );

    MainSizer->Add( cmdSubSizer, 0, wxEXPAND|wxALL, 5);

    //////////SearchTimer////////////

    searchTimer.SetOwner(this, wxID_ANY);

    //////////ListCtrl//////////

    songList = new wxListView( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
    songList->InsertColumn(0,wxT("#"),wxLIST_FORMAT_LEFT, 25);
    songList->InsertColumn(1,wxT("Title"), wxLIST_FORMAT_LEFT, 250);
    songList->InsertColumn(2,wxT("Artist"), wxLIST_FORMAT_LEFT, 200);
    songList->InsertColumn(3,wxT("Album"), wxLIST_FORMAT_LEFT, 200);
    songList->InsertColumn(4,wxT("Location"), wxLIST_FORMAT_LEFT, 400);
    songList->SetBackgroundColour(GetBackgroundColour());

    ////////// SearchBar///////////

    searchBar = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,30 ), wxTE_PROCESS_ENTER );
    #ifndef __WXMAC__
        searchBar->ShowSearchButton( true );
    #endif
    searchBar->ShowCancelButton( true );


    //////////FileDialog//////////
    // file dialog searches only for mp3 files ( only one wildcard )
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

    auto centerLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    MainSizer->Add(centerLine, 0, wxEXPAND|wxALL, 5);
    //////// Options Buttons and Songlist sizers //////

    optionsSongListSizer = new wxBoxSizer( wxHORIZONTAL );

    auto optionsSizer = new wxBoxSizer(wxVERTICAL);
    auto firstSeparatorLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    auto secondSeparatorLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    auto thirdSeparatorLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    optionsSizer->Add( searchBar, 0, wxALL, 5 );
    optionsSizer->Add( firstSeparatorLine, 0, wxEXPAND | wxALL, 5 );
    optionsSizer->Add( addFile, 0, wxALIGN_LEFT|wxEXPAND, 5);
    optionsSizer->Add( deleteFromPlaylist, 0, wxALIGN_LEFT|wxEXPAND, 5 );
    optionsSizer->Add( secondSeparatorLine, 0, wxEXPAND | wxALL, 5 );
    optionsSizer->Add( save, 0, 0, 5);
    optionsSizer->Add( prevSession, 0, 0, 5);
    optionsSizer->Add( thirdSeparatorLine, 0, wxEXPAND | wxALL, 5 );

    optionsSongListSizer->Add(optionsSizer, 0);
    optionsSongListSizer->Add(songList, 1, wxEXPAND|wxRIGHT, 5);

    MainSizer->Add( optionsSongListSizer, 1, wxEXPAND|wxLEFT, 5 );

    /////////MediaCtrl/////////

    mediaCtrl = new wxMediaCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    mediaCtrl->Enable();

    /////////Controls////////

    controlSubSizer = new wxBoxSizer( wxHORIZONTAL );

    mediaTimer = new MediaTimer(this);
    mediaTimer->Start(500);

    wxBitmap prev;
    prev.LoadFile("../ControlsPNG/previous.png", wxBITMAP_TYPE_PNG);

    Previous = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, prev.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE);
    Previous->SetBitmap(prev);
    Previous->SetBackgroundColour(GetBackgroundColour());
    controlSubSizer->Add( Previous, 0, 0, 5 );

    playBitmap.LoadFile("../ControlsPNG/play-button.png", wxBITMAP_TYPE_PNG);

    Play = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, playBitmap.GetSize(),wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    Play->SetBitmap(playBitmap);
    Play->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( Play, 0, 0, 5 );

    pauseBitmap.LoadFile("../ControlsPNG/pause.png", wxBITMAP_TYPE_PNG);

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

    volume.LoadFile("../ControlsPNG/volume.png", wxBITMAP_TYPE_PNG);

    volumeButton = new wxButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, volume.GetSize(), wxTRANSPARENT_WINDOW|wxBORDER_NONE );
    volumeButton->SetBitmap(volume);
    volumeButton->SetBackgroundColour(GetBackgroundColour());

    controlSubSizer->Add( volumeButton, 0, 0, 5 );

    mute.LoadFile("../ControlsPNG/mute.png", wxBITMAP_TYPE_PNG);

    Volume = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 100,-1 ), wxSL_HORIZONTAL );

    controlSubSizer->Add( Volume, 0, 0, 5 );

    //////////////LabelsSizer/////////////


    labelsSizer = new wxBoxSizer( wxVERTICAL );

    titleLabel = new wxStaticText( this, wxID_ANY, wxT("Title : To be Defined"), wxDefaultPosition, wxDefaultSize, 0 );
    titleLabel->Wrap( -1 );
    labelsSizer->Add( titleLabel, 0, wxALL, 5 );

    lengthLabel = new wxStaticText( this, wxID_ANY, wxT("Length : To be Defined"), wxDefaultPosition, wxDefaultSize, 0 );
    lengthLabel->Wrap( -1 );
    labelsSizer->Add( lengthLabel, 0, wxALL, 5 );

    artistLabel = new wxStaticText( this, wxID_ANY, wxT("Artist : To be Defined"), wxDefaultPosition, wxDefaultSize, 0 );
    artistLabel->Wrap( -1 );
    labelsSizer->Add( artistLabel, 0, wxALL, 5 );

    albumLabel = new wxStaticText( this, wxID_ANY, wxT("Album : To be Defined"), wxDefaultPosition, wxDefaultSize, 0 );
    albumLabel->Wrap( -1 );
    labelsSizer->Add( albumLabel, 0, wxALL, 5 );

    genreLabel = new wxStaticText( this, wxID_ANY, wxT("Genre : To be Defined"), wxDefaultPosition, wxDefaultSize, 0 );
    genreLabel->Wrap( -1 );
    labelsSizer->Add( genreLabel, 0, wxALL, 5 );
    labelsSizer->SetMinSize(250,-1);

    auto separatorLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    controlSubSizer->Prepend( separatorLine, 0, wxEXPAND | wxALL, 5 );

    controlSubSizer->Prepend( labelsSizer, 1, wxALL, 5 );

    sliderControls->Add( controlSubSizer, 0, wxALL, 5 );
    this->SetSizer( MainSizer );
    this->Layout();

    statusBar = new wxStatusBar(this);
    int widths[3] = {30, 180, 120}; // width status bar fields
    statusBar -> SetFieldsCount(3, widths);
    statusBar -> PushStatusText(wxT("#"), 0);
    statusBar -> PushStatusText(wxT("Title here"), 1);
    this->SetStatusBar(statusBar);

    /////////Menu//////////

    menuBar = new wxMenuBar( 0 );

    File = new wxMenu();
    wxMenuItem* loadItem;
    loadItem = new wxMenuItem( Instructions, wxID_ANY, wxString( wxT("Load") ) , wxEmptyString, wxITEM_NORMAL );
    File->Append( loadItem );
    wxMenuItem* saveItem;
    saveItem = new wxMenuItem( Instructions, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
    File->Append( saveItem );
    menuBar->Append( File, wxT("File") );

    About = new wxMenu();
    wxMenuItem* aboutItem;
    aboutItem = new wxMenuItem( About, wxID_ANY, wxString( wxT("About Daeum ...") ) , wxEmptyString, wxITEM_NORMAL );
    About->Append( aboutItem );
    menuBar->Append( About, wxT("About") );

    Instructions = new wxMenu();
    wxMenuItem* instructionItem;
    instructionItem = new wxMenuItem( Instructions, wxID_ANY, wxString( wxT("Instructions...") ) , wxEmptyString, wxITEM_NORMAL );
    Instructions->Append( instructionItem );
    menuBar->Append( Instructions, wxT("Instructions") );

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
    this->Connect(instructionItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onInstructions));
    this->Connect(loadItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onPrevSession));
    this->Connect(saveItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onSave));
    this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Connect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
    mediaSlider->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(MainFrame::onMediaSlider), nullptr, this);
    mediaSlider->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onBeginSeek), nullptr, this);
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
    loadFile->Destroy(); //special case. wxWidgets doesn't take care of wxDialog

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
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onInstructions));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onPrevSession));
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::onSave));
    this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrame::onTimer ) );
    songList->Disconnect( wxEVT_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::onListItemSelected ), nullptr, this );
    mediaSlider->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(MainFrame::onMediaSlider), nullptr, this);
    mediaSlider->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(MainFrame::onBeginSeek), nullptr, this);

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
    wxArrayString toDelete;
    long selectedItem = songList -> GetFirstSelected();
    while(selectedItem != -1) {
        toDelete.push_back(songList -> GetItemText(selectedItem, 4));
        selectedItem = songList -> GetNextSelected(selectedItem);
    }
    controller -> deleteSong(toDelete);

}

void MainFrame::onPrevious(wxCommandEvent &event) {
    controller->prevSong();
}

void MainFrame::onPlay(wxCommandEvent &event) {
    if(mediaCtrl->GetState() == wxMEDIASTATE_PLAYING){
        mediaCtrl->Pause();
        Play->SetBitmap(playBitmap);
    }
    else //otherwise it will change the bitmap even if there aren't any playing songs
        {
            mediaCtrl->Play();
            Play->SetBitmap(pauseBitmap);
        }
}

void MainFrame::onNext(wxCommandEvent &event) {
    controller->nextSong();
}

void MainFrame::onStop(wxCommandEvent &event) {
    mediaCtrl -> Stop();
    Play->SetBitmap(playBitmap);
}

void MainFrame::setLoopFrame(wxCommandEvent& event){
    controller->setLoop();
    if(Loop->GetBackgroundColour() == wxColour(110, 140, 130))
        Loop->SetBackgroundColour(GetBackgroundColour());
    else
        Loop->SetBackgroundColour(wxColour(110, 140, 130));
}

void MainFrame::onVolume(wxCommandEvent& event){
    // it goes from any volume to mute and from mute to 50 %
    auto value = Volume->GetValue();
    if(value != 0) {
        mediaCtrl->SetVolume(0);
        Volume->SetValue(0);
        volumeButton->SetBitmap(mute);
    }
    else{
        mediaCtrl->SetVolume(0.5);
        Volume->SetValue(50);
        volumeButton->SetBitmap(volume);
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
    wxString msg;
    msg.Printf( wxT("Daeum is a music player.\n\n It plays the songs you  add with the help of the add button or by drag & drop.\n It supports mp3 (wav also work)"));
    wxMessageBox(msg, wxT("About Daem Music Player"));
}
void MainFrame::onInstructions(wxCommandEvent &event){
    wxString msg;
    msg.Printf( wxT("Add button: Adds files from a given directory. You can add one or more files at the same time. You will find only mp3 files in your folder.\n If you want wav files just drag and drop them in the playlist.\n\n"
                    "Delete button: Deletes selected songs (one or more) from the playlist.\n\n"
                    "Save button: Saves the session. If you want to find the same playlist the next time please save it first.\n\n"
                    "Load button: Loads the previous session \n\n"
                    "The previous, play, pause, next buttons work as you expect it to work.\n\n"
                    "Loop button: If you click it once it will loop the song . To stop the looping process press the button again. \n\n"
                    "Shuffle: it selects a random song from the playlist and plays it. \n\n"
                    "Search field: searches for a song. You can search for the title or part of it. It is case sensitive"
                ));
    wxMessageBox(msg, wxT("How to use the player"));
}
void MainFrame::onQuit(wxCommandEvent &event){
    Close();
}
void MainFrame::onBeginSeek(wxScrollEvent& event)
{
    IsBeingDragged = true;
}

void MainFrame::onLoaded(wxMediaEvent &event) {
    mediaCtrl->Play();
}

void MainFrame::onStopped(wxMediaEvent& event){
    controller->nextSong();
}
void MainFrame::onListItemActivated(wxListEvent &event) {
    controller->tellPlaylist(songList->GetItemText(event.GetIndex(), 4));
}

void MainFrame::onListItemSelected(wxListEvent &event){
}

void MainFrame::play(wxString path){
    if(path != wxEmptyString){
        mediaCtrl->Load(path);
        Play->SetBitmap(pauseBitmap);
    }

}

void MainFrame::onMediaSlider(wxScrollEvent &event){
    if( mediaCtrl->Seek(mediaSlider->GetValue() * 1000) == wxInvalidOffset )
        wxMessageBox(wxT("Couldn't seek!"));
    IsBeingDragged = false;
}

