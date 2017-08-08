//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_SONG_H
#define DAEUM_SONG_H

#include "wx/wx.h"
#include "Subject.h"
#include <list>
using namespace std;

class Song : public Subject{

public:

    Song(wxString title, wxString artist = wxT("Unknown"), wxString album = wxT("Unknown"), int duration = 0);

    int getLength() const;

    void setLength(int length);

    const wxString &getTitle() const;

    void setTitle(const wxString &title);

    const wxString &getArtist() const;

    void setArtist(const wxString &artist);

    const wxString &getAlbum() const;

    void setAlbum(const wxString &album);

    void setLoop(bool loop);

    bool isLoop() const;

    void notifyObserver() const override;
    void registerObserver(Observer* o) override;
    void removeObserver(Observer* o) override;

private:
    int length;
    wxString title;
    wxString artist;
    wxString album;
    bool loop;
    list<Observer*> observers;
};


#endif //DAEUM_SONG_H
