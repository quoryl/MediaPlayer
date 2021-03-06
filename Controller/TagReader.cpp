//
// Created by azrael on 19/09/17.
//

#include "TagReader.h"

TagReader::TagReader(const char* songPath) {
    if(songPath != nullptr) {
        tagFile = new TagLib::FileRef(songPath);
        if (!tagFile->isNull() && tagFile->tag() != nullptr) {
            myTag = tagFile->tag();
            file = static_cast<wxString>(songPath);
        } else
            cout << "The file is not valid or it doesn't have tags" << endl;
    }
    else
        cout << "The path is null" << endl;
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

//obtained from:
//http://rajeevandlinux.wordpress.com/2012/04/24/extract-album-art-from-mp3-files-using-taglib-in-c/
bool TagReader::processAlbumArt(){
    if(file != wxEmptyString) {

        const char *IdPicture = "APIC";

        TagLib::MPEG::File mpegFile(file.fn_str());
        TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
        TagLib::ID3v2::FrameList Frame;
        TagLib::ID3v2::AttachedPictureFrame *PicFrame;
        char *SrcImage;
        unsigned long Size;

        if (id3v2tag) {
            std::fstream outFile("AlbumArt.png", ios::out | ios::binary);
            Frame = id3v2tag->frameListMap()[IdPicture];
            if (!Frame.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it) {
                    PicFrame = (TagLib::ID3v2::AttachedPictureFrame *) (*it);
                    {
                        Size = PicFrame->picture().size();

                        SrcImage = new char[Size];

                        if (SrcImage) {
                            memcpy(SrcImage, PicFrame->picture().data(), Size);

                            outFile.write(SrcImage, Size);

                            outFile.close();
                            delete SrcImage;
                            return true;
                        }

                    }
                }
            }
        }
    }
    return false;
}
wxBitmap TagReader::getAlbumArt()
{
    wxBitmap bmp;

    if(processAlbumArt()){
        bmp = wxBitmap( wxT("AlbumArt.png"), wxBITMAP_TYPE_ANY);
    }
    else
        bmp = wxBitmap( wxT("../ControlsPNG/NoAlbumArt.png"), wxBITMAP_TYPE_ANY);

    return bmp;
}

TagReader::~TagReader() {
    if(tagFile!=nullptr)
        delete tagFile;
}
