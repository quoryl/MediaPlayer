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
    if(song != nullptr && song != playing ) {
        long elementNumber = 0;
        playList.remove(song);
        for(auto iter: playList) {
            iter->setID(elementNumber);
            elementNumber++;
        }
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

void Playlist::nowPlaying(Song* s){
    s->setSongState(wxMEDIASTATE_PLAYING);
    Song* previousSong = playing;
    playing = s;
    for(auto o: playListObservers)
        o->updateSongDetails(s, previousSong);
}

void Playlist::songChanged(std::vector<long>* indexList){
    long randomSongID = *indexList->begin();
    nowPlaying(this->getSong(randomSongID));
}


Song* Playlist::getSong(long ID){
    for(auto iter: playList)
        if(iter->getID() == ID)
            return iter;
}

const list<Song *> &Playlist::getPlayList() const {
    return playList;
}

void Playlist::setPlayList(const list<Song *> &playList) {
    Playlist::playList = playList;
}

Song *Playlist::getPlaying() const {
    return playing;
}

void Playlist::setPlaying(Song *playing) {
    Playlist::playing = playing;
}

Playlist::~Playlist() {
    for(auto i: playList){
        delete i;
    }
}








