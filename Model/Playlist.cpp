//
// Created by azrael on 30/07/17.
//

#include "Playlist.h"
bool Playlist::addToPlaylist(Song* song) {
    if(song != nullptr) {
        playList.push_back(song);
        return true;
    }
    else
        return false;
}

bool Playlist::deleteFromPlaylist(Song* song){
    if(song != nullptr){
        playList.remove(song);
        return true;
    }
    else
        return false;
}

void Playlist::searchPlaylist(wxString filterText) {

    for (auto i: playList) {
        if (!filterText.IsEmpty()) {
            if (i->getTitle().IsSameAs(filterText, false) || i->getTitle().StartsWith(filterText)) {
                //addSongToList(i);
            }
        } else {
            //addSongToList(i);
        }

    }
}

void Playlist::notifyObserver() const {

}

void Playlist::registerObserver(Observer *o) {
     playListObservers.push_back(o);
}

void Playlist::removeObserver(Observer *o) {
     playListObservers.remove(o);
}

Playlist::~Playlist() {

}
