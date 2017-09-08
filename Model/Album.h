//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_ALBUM_H
#define DAEUM_ALBUM_H
#include "wx/wx.h"
#include "Song.h"

using namespace std;
class Album {

public:

    int nSongs;
    int releaseYear;
    wxString artist;
    wxString title;
    wxString genre;

};


#endif //DAEUM_ALBUM_H
