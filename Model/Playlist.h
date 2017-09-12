//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_PLAYLIST_H
#define DAEUM_PLAYLIST_H

#include <list>
#include <vector>
#include "wx/wx.h"
#include "Subject.h"
#include "Song.h"

using namespace std;

class Playlist: public Subject {
public:
    bool addToPlaylist( Song* song);
    bool deleteFromPlaylist(Song* song);
    void searchPlaylist(wxString filterText);
    void notifyObserver() override;
    void registerObserver(Observer* o) override;
    void removeObserver(Observer* o) override;
    void nowPlaying(Song* s);
    void songChanged(std::vector<long>* indexList );//follows the indexes created by shuffle to play songs in that order)
    Song* getSong(long ID);//returns song with the given id from the playlist
    const list<Song *> &getPlayList() const;
    void setPlayList(const list<Song *> &playList);
    Song *getPlaying() const;
    void setPlaying(Song *playing);
    const list<Song *> &getSearchTempList() const;
    const list<Observer *> &getPlayListObservers() const;

    virtual ~Playlist();
private:
    Song* playing = nullptr;//currently playing song

    list<Song*> playList;//main playlist

    list<Song*> searchTempList;//used for searching

    list<Observer*> playListObservers;
};


#endif //DAEUM_PLAYLIST_H
