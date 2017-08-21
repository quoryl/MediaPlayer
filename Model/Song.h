//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_SONG_H
#define DAEUM_SONG_H

#include "wx/wx.h"



class Song {

public:

    Song(wxString title, wxString artist = wxT("Unknown"), wxString album = wxT("Unknown"), int duration = 0, wxString sp = wxEmptyString);

    int getLength() const;

    void setLength(int length);

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


private:
    int length;
    wxString title;
    wxString artist;
    wxString album;
    wxString songPath;
    bool loop = false;

};


#endif //DAEUM_SONG_H
