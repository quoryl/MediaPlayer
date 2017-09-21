//
// Created by azrael on 19/09/17.
//

#ifndef DAEUM_TAGREADER_H
#define DAEUM_TAGREADER_H

#include <wx/wx.h>
#include <vector>
#include "taglib/tag.h"
#include "taglib/fileref.h"

#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/attachedpictureframe.h>
#include <iostream>
#include <fstream>
using namespace std;
class TagReader {
public:

    explicit TagReader(const char* songPath);

    virtual ~TagReader();

    wxString getTitle();
    wxString getArtist();
    wxString getAlbum();
    wxString getGenre();
    bool processAlbumArt();
    wxBitmap getAlbumArt();
private:
    wxString file = wxEmptyString;
    TagLib::FileRef* tagFile = nullptr;
    TagLib::Tag* myTag = nullptr;

};
/*
 get album art use:
    wxBoxSizer* bitmapSubSizer;
    bitmapSubSizer = new wxBoxSizer( wxHORIZONTAL );
    wxString albert = wxT("/home/azrael/Music/DNA.mp3");
    auto met = static_cast<const char*>(albert);
    TagReader a(met);
    wxStaticBitmap* art = new wxStaticBitmap(this, wxID_ANY, a.getAlbumArt(), wxDefaultPosition, wxSize(40,40));
    bitmapSubSizer->Add(art, 1,0,5);
    MainSizer->Add( bitmapSubSizer, 0, wxALL, 5 );
 */


#endif //DAEUM_TAGREADER_H
