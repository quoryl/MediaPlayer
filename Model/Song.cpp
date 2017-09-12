//
// Created by azrael on 30/07/17.
//

#include "Song.h"

Song::Song( wxString title, wxString artist, wxString album, int duration, wxString path):title(title),
         artist(artist), album(album){
    // This two members are initialized here (not above) because they have to respect some conditions
    // length must be not negative
    // path must not be wxEmptyString
    setLength(duration);
    setSongPath(path);
    
    //these ones with get changed in the future
    setLoop(false);
    setSongState(wxMEDIASTATE_STOPPED);
    ID = -1;
}

long Song::getLength() const {
    return length;
}

void Song::setLength(long length) {
    if(length>=0)
        Song::length = length;
    else
        Song::length = 0;
}

const wxString Song::getTitle() const {
    return title;
}

void Song::setTitle(const wxString &title) {
    Song::title = title;
}

const wxString Song::getArtist() const {
    return artist;
}

void Song::setArtist(const wxString &artist) {
    Song::artist = artist;
}

const wxString Song::getAlbum() const {
    return album;
}

void Song::setAlbum(const wxString &album) {
    Song::album = album;
}

bool Song::isLoop() const {
    return loop;
}

void Song::setLoop(bool loop) {
    Song::loop = loop;
}

const wxString Song::getSongPath(){
    return songPath;
}

void Song::setSongPath(const wxString& sp){
    if(sp != wxEmptyString)
        songPath = sp;
}

long Song::getID() const {
    return ID;
}

void Song::setID(long ID) {
    if(ID >=0)
        Song::ID = ID;
    else
        Song::ID = -1; // not 0 because it is a valid id; at least it will be known that the invalid id is -1(the initial one)
}

wxMediaState Song::getSongState() const {
    return songState;
}

void Song::setSongState(wxMediaState songState) {
    Song::songState = songState;
}

Song::~Song() {
    std::cout << "Deleted " << this->getTitle() << " at address: " << this << std::endl;
}
