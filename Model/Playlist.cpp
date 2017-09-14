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

bool Playlist::deleteFromPlaylist(Song* song){
    if(song != nullptr && song != playing ) {
        long elementNumber = 0;
        playList.remove(song);//std::list::remove calls the destructor of song. There shouldn't be any memory leaks here
        delete song;
        for(auto iter: playList) {
            iter->setID(elementNumber);
            elementNumber++;
        }
        notifyObserver();
        return true;
    }
    else
        return false;
}

void Playlist::searchPlaylist(wxString filterText) {
    searchTempList.clear(); // TODO does it take care of song pointers?
    for (auto i: playList) {
        if (!filterText.IsEmpty()) {
            if (i->getTitle().Contains(filterText)) {
                searchTempList.push_back(i);
            }
        }
        else {
        searchTempList.push_back(i);
        }

    }
    //notify observer but with a different list
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
    // wxMediaState is an enumeration defined by wxWidgets.
    // You can't create a wxMediaState variable in the program (unless you change the sources ?)
    // This means you can't pass an invalid parameter to setSongState()
    if( s != nullptr) {

        s->setSongState(wxMEDIASTATE_PLAYING);
        Song *previousSong = playing;
        setPlaying(s);
        for (auto o: playListObservers)
            o->updateSongDetails(s, previousSong);
        if (previousSong != nullptr)
            previousSong->setSongState(wxMEDIASTATE_STOPPED);
    }
}

void Playlist::songChanged(std::vector<long>* indexList){
    if(!indexList->empty()) {
        long randomSongID = *indexList->begin();
        nowPlaying(this->getSong(randomSongID));
    }
}


Song* Playlist::getSong(long ID){
    if(ID >= 0 && ID < playList.size())
        for(auto iter: playList)
            if(iter->getID() == ID)
                return iter;
}

const list<Song *> &Playlist::getPlayList() const {
    return playList;
}


Song *Playlist::getPlaying() const {
    return playing;
}

void Playlist::setPlaying(Song *playing) {
    if(playing != nullptr)
        Playlist::playing = playing;
}

const list<Song *> &Playlist::getSearchTempList() const {
    return searchTempList;
}

Playlist::~Playlist() {
    if(!playList.empty())
        for(auto i: playList){
            delete i;
        }
}










