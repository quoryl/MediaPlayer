//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_PLAYLIST_H
#define DAEUM_PLAYLIST_H

#include "wx/wx.h"
#include "Subject.h"
#include "Song.h"
#include <list>

using namespace std;

class Playlist: public Subject {
public:
    bool addToPlaylist( Song* song);
    void searchPlaylist(wxString filterText);
    bool deleteFromPlaylist(Song* song);

    void notifyObserver() const override;
    void registerObserver(Observer* o) override;
    void removeObserver(Observer* o) override;

    virtual ~Playlist();

    list<Song*> playList;
    list<Observer*> playListObservers;
};


#endif //DAEUM_PLAYLIST_H
