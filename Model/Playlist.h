//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_PLAYLIST_H
#define DAEUM_PLAYLIST_H

#include "wx/wx.h"
#include "Subject.h"
#include "Song.h"
#include <list>
#include <vector>
using namespace std;

class Playlist: public Subject {
public:
    bool addToPlaylist( Song* song);
    void searchPlaylist(wxString filterText);
    void deleteFromPlaylist(Song* song);
    void notifyObserver() override;
    void registerObserver(Observer* o) override;
    void removeObserver(Observer* o) override;
    void nowPlaying(Song* s);
    const list<Song *> &getPlayList() const;
    void setPlayList(const list<Song *> &playList);

    virtual ~Playlist();

private:
    list<Song*> playList;

    list<Song*> searchTempList; //used for searching

    list<Observer*> playListObservers;
};


#endif //DAEUM_PLAYLIST_H
