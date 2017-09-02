//
// Created by azrael on 31/07/17.
//



#include "MediaController.h"


MediaController::MediaController(Playlist* pList) : playlist(pList){

}

void MediaController::searchItem(wxString text) {

    playlist -> searchPlaylist(text);

}

void MediaController::addFile(wxArrayString *paths, wxMediaCtrl *mediaControl) {

    for(auto i: *paths) {
        auto tempList = playlist->getPlayList();

        wxString name = wxFileName(i).GetName();
        bool found = false;
        if (tempList.empty()) {
            Song *song = new Song(name, wxT("Unknown"), wxT("Unknown"), 0, i); // I will use all the tags for creating the song (taglib)
            playlist->addToPlaylist(song); //to std::list
        } else {
            for (auto g : tempList) {
                //this is temporary. The title is not enough. I should check something like title+artist when taglib will work
                if ((g->getSongPath()).IsSameAs(i)) {
                    found = true;
                }
            }
            if (!found) {
                Song *song = new Song(name, wxT("Unknown"), wxT("Unknown"), 0, i);
                playlist->addToPlaylist(song); //to std::list
            }

        }
    }

}
void MediaController::deleteSong(wxString toDeletePath) {

    auto tempList = playlist -> getPlayList();
    for(auto g : tempList){
        if(g->getSongPath().IsSameAs(toDeletePath)) {
            playlist->deleteFromPlaylist(g);
        }
    }
    wxMessageBox(wxT("Song/s Deleted!"));

}

void MediaController::shuffleList() {

}

void MediaController::prevSong() {

}

void MediaController::playSong() {

}

void MediaController::nextSong() {

}

void MediaController::showVolume() {

}

void MediaController::changeVolume() {

}

void MediaController::showAbout() {
    wxString msg;
    msg.Printf( wxT("Daeum is a music player.\n\nYou can use the sliders to seek a position within the song or to\nchange the volume. You can add songs from your directories by selection Add button.\nYou can play a random song from your list and also\nyou can delete a song from your list if you don't like it."));
    wxMessageBox(msg, wxT("About Daem Music Player"));
}



map<wxString, wxString> MediaController::getMetadata(wxString *filePath) {
    //we need a const char* for FileName constructor so we need cast path(wxString)
   /*  using namespace TagLib;

     const char* charPath = (filePath->mbc_str());
     FileRef TagMain{FileName(charPath)};

    if(!TagMain.isNull() && TagMain.tag()) {

         TagLib::Tag *tag = TagMain.tag();

         wstring _tagTitle = (tag->title()).toWString();
         wstring _tagAlbum = (tag->album()).toWString();
         wstring _tagArtist = (tag->artist()).toWString();


         wxString tagTitle(_tagTitle);
         wxString tagAlbum(_tagAlbum);
         wxString tagArtist(_tagArtist);
         std::map<wxString,wxString> metadata = {{"title", tagTitle}, {"album", tagAlbum}, {"artist", tagArtist}};
         return metadata;

    } else
        wxMessageBox(wxT("Taglib error : FileRef is null or tag is false"));
*/
}

void MediaController::loop(wxMediaCtrl *mediaControl) {
/*
    if (song -> isLoop()){
        mediaControl -> Play();
    }
*/
}

void MediaController::setLoop() {
    //FIXME playlist is now the subject
    /*
    song -> setLoop( !song -> isLoop() );
    std:: cout << song -> isLoop() << std::endl;*/
}
void MediaController::save(){
    wxFileOutputStream fileOStream(wxT("../savedSession.txt"));
    if(fileOStream.IsOk()) {
        wxTextOutputStream textOStream(fileOStream);
        auto tmp = playlist->getPlayList();
        for(auto i:tmp) {
            textOStream << i->getSongPath()<<"\n";
        }
        wxMessageBox(wxT("The content was saved !"));
    }
    else
        wxMessageBox(wxT("Error : wxFileOutputStream object is not available"));
}

void MediaController::load() {

    wxTextFile text(wxT("../savedSession.txt"));
    if(text.Exists()) {
        text.Open();
        if (!text.GetFirstLine().IsSameAs(wxEmptyString)) {
            wxArrayString tmp;
            for (auto str = text.GetFirstLine(); !text.Eof(); str = text.GetNextLine()) {
                tmp.Add(str);
            }
            if (text.Eof()) {
                wxMessageBox(wxT("Loading completed! "));
            }
            text.Close();
            addFile(&tmp);
        } else
            wxMessageBox(wxT("There is no saved data"));

    }
    else
        wxMessageBox(wxT("File not found! "));
}
