//
// Created by azrael on 30/07/17.
//

#include "Playlist.h"
bool Playlist::addToPlaylist(Song* song) {
    if(song != nullptr ) {
        playList.push_back(song);
        notifyObserver();
        return true;
    }
    else {
        return false;
    }
}

void Playlist::deleteFromPlaylist(Song* song){
    if(song != nullptr) {
        playList.remove(song);
        notifyObserver();
    }

}

void Playlist::searchPlaylist(wxString filterText) {
    searchTempList.clear(); // TODO does it take care of song pointers?
    for (auto i: playList) {
        if (!filterText.IsEmpty()) {
            if (i->getTitle().Contains(filterText)) {
                searchTempList.push_back(i);
            }
        } else {
        searchTempList.push_back(i);
        }

    }
    for(auto o: playListObservers)
        o->update(searchTempList);
}

void Playlist::notifyObserver() {
    for(auto o: playListObservers)
        o -> update(playList);

}

void Playlist::registerObserver(Observer *o) {
     playListObservers.push_back(o);
}

void Playlist::removeObserver(Observer *o) {
     playListObservers.remove(o);
}

const list<Song *> &Playlist::getPlayList() const {
    return playList;
}


Playlist::~Playlist() {
    for(auto i: playList){
        delete i;
    }
}




