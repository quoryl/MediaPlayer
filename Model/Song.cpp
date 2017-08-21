//
// Created by azrael on 30/07/17.
//

#include "Song.h"

Song::Song( wxString title, wxString artist, wxString album, int duration, wxString sp):title(title),
         artist(artist), album(album), length(duration), songPath(sp){}

int Song::getLength() const {
    return length;
}

void Song::setLength(int length) {
    Song::length = length;
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
    songPath = sp;
}

