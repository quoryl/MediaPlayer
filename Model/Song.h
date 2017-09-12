//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_SONG_H
#define DAEUM_SONG_H

#include <wx/mediactrl.h>
#include "wx/wx.h"

class Song {

public:

    explicit Song(wxString title, wxString artist = wxT("Unknown"), wxString album = wxT("Unknown"), int duration = 0, wxString path = wxEmptyString);

    long getLength() const;

    void setLength(long length);

    const wxString getTitle() const;

    void setTitle(const wxString &title);

    const wxString getArtist() const;

    void setArtist(const wxString &artist);

    const wxString getAlbum() const;

    void setAlbum(const wxString &album);

    void setLoop(bool loop);

    bool isLoop() const;

    const wxString getSongPath();

    void setSongPath(const wxString& sp);

    long getID() const;

    void setID(long ID);

    wxMediaState getSongState() const;

    void setSongState(wxMediaState songState);

    virtual ~Song();

private:
    long length;
    wxString title;
    wxString artist;
    wxString album;
    wxString songPath;
    long ID; 
    bool loop;
    wxMediaState songState;



};


#endif //DAEUM_SONG_H
