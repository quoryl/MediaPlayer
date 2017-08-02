//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_ALBUM_H
#define DAEUM_ALBUM_H
#include "wx/wx.h"
#include "Song.h"
#include <map>

using namespace std;
class Album {

public:

    int nSongs;
    int releaseYear;
    wxString artist;
    wxString title;
    wxString genre;


    //Must be an aggregation  with song TODO
    map<int, Song> mapAlbum;
};


#endif //DAEUM_ALBUM_H
