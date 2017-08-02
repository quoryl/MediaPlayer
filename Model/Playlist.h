//
// Created by azrael on 30/07/17.
//

#ifndef DAEUM_PLAYLIST_H
#define DAEUM_PLAYLIST_H

#include "wx/wx.h"
#include "SubjectInterface.h"
#include "Song.h"
#include <map>

using namespace std;

class Playlist: public SubjectInterface {
public:

    wxString title;
    int nSongs;
    int length;

    //Must be an aggregation  with song TODO
    map<int, Song> mapPlaylist;
};


#endif //DAEUM_PLAYLIST_H
