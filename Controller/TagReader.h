//
// Created by azrael on 19/09/17.
//

#ifndef DAEUM_TAGREADER_H
#define DAEUM_TAGREADER_H

#include <wx/wx.h>
#include <vector>
#include "taglib/tag.h"
#include "taglib/fileref.h"
using namespace std;
class TagReader {
public:

    explicit TagReader(const char* songPath);

    virtual ~TagReader();

    wxString getTitle();
    wxString getArtist();
    wxString getAlbum();
    wxString getGenre();
private:
    TagLib::FileRef* tagFile = nullptr;
    TagLib::Tag* myTag = nullptr;
};


#endif //DAEUM_TAGREADER_H
