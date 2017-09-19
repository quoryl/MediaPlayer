//
// Created by azrael on 19/09/17.
//

#include "TagReader.h"

TagReader::TagReader(const char* songPath) {
    tagFile = new TagLib::FileRef(songPath);
    if(!tagFile->isNull() && tagFile->tag() != nullptr){
        myTag = tagFile->tag();
    }
    else
        cout << "The file is not valid or it doesn't have tags" << endl;

}

wxString TagReader::getTitle() {
    if(myTag != nullptr) {
        wxString title = myTag->title().to8Bit(true);
        return title;
    }
    else
        return wxEmptyString;
}

wxString TagReader::getArtist() {
    if(myTag != nullptr) {
        wxString artist = myTag->artist().to8Bit(true);
        return artist;
    }
    else
        return wxEmptyString;
}

wxString TagReader::getAlbum() {
    if(myTag != nullptr) {
        wxString album = myTag->album().to8Bit(true);
        return album;
    }
    else
        return wxEmptyString;
}

wxString TagReader::getGenre() {
    if(myTag != nullptr) {
        wxString genre = myTag->genre().to8Bit(true);
        return genre;
    }
    else
        return wxEmptyString;
}

TagReader::~TagReader() {
    delete tagFile;
}
