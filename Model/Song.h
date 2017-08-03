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
private:
    int length;
public:
    int getLength() const;

    void setLength(int length);

    const wxString &getTitle() const;

    void setTitle(const wxString &title);

    const wxString &getArtist() const;

    void setArtist(const wxString &artist);

    const wxString &getAlbum() const;

    void setAlbum(const wxString &album);

    void notifyObserver() const override;
    void registerObserver(Observer* o) override;
    void removeObserver(Observer* o) override;

private:
    wxString title;
    wxString artist;
    wxString album;
    list<Observer*> observers;

};


#endif //DAEUM_SONG_H
