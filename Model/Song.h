//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_SONG_H
#define DAEUM_SONG_H

#include <wx/mediactrl.h>
#include "wx/wx.h"

class Song {

public:

    explicit Song(wxString title,  wxBitmap albumArt, wxString artist = wxT("Unknown"), wxString album = wxT("Unknown"), int duration = 0, wxString path = wxEmptyString,  wxString genre = wxT("Unknown"));

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

    const wxString &getGenre() const;

    const wxBitmap &getAlbumArt() const;

    virtual ~Song();

private:
    long length = 0;
    wxString title;
    wxString artist;
    wxString album;
    wxString songPath;
    long ID; 
    bool loop;
    wxMediaState songState;
    wxString genre;
    wxBitmap albumArt ;

};


#endif //DAEUM_SONG_H
