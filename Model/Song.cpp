//
// Created by azrael on 30/07/17.
//

#include "Song.h"

Song::Song(wxString title, wxString artist, wxString album, int duration):title(title),
         artist(artist), album(album), length(duration){}

int Song::getLength() const {
    return length;
}

void Song::setLength(int length) {
    Song::length = length;
}

const wxString &Song::getTitle() const {
    return title;
}

void Song::setTitle(const wxString &title) {
    Song::title = title;
}

const wxString &Song::getArtist() const {
    return artist;
}

void Song::setArtist(const wxString &artist) {
    Song::artist = artist;
}

const wxString &Song::getAlbum() const {
    return album;
}

void Song::setAlbum(const wxString &album) {
    Song::album = album;
}

void Song::notifyObserver() const {
    for(Observer* observer:observers){
        observer->update();
    }
}

void Song::registerObserver(Observer* o) {
    observers.push_back(o);
}

void Song::removeObserver(Observer* o) {
    observers.remove(o);

}
