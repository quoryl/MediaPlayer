//
// Created by azrael on 31/07/17.
//
#include "MediaController.h"


MediaController::MediaController(Playlist* pList) : playlist(pList){

}

void MediaController::searchItem(wxString text) {

    playlist -> searchPlaylist(text);

}

void MediaController::addFile(wxArrayString *paths) {

    for(auto i: *paths) {
        auto tempList = playlist->getPlayList();

        wxString name = wxFileName(i).GetName();
        bool found = false;
        if (tempList.empty()) {

            Song *song = new Song(name, wxT("Unknown"), wxT("Unknown"), 0, i); // I will use all the tags for creating the song (taglib)
            song->setID(playlist->getPlayList().size());
            playlist->addToPlaylist(song); //to std::list

        }
        else {

            for (auto g : tempList) {
                if ((g->getSongPath()).IsSameAs(i)) {
                    found = true;
                }
            }
            if (!found) {
                Song *song = new Song(name, wxT("Unknown"), wxT("Unknown"), 0, i);
                song->setID(playlist->getPlayList().size());
                playlist->addToPlaylist(song); //to std::list

            }

        }
    }

}
void MediaController::deleteSong(wxString toDeletePath) {

    playlist->deleteFromPlaylist(getSongFromPlaylist(toDeletePath));
    wxMessageBox(wxT("Song/s Deleted!"));

}

void MediaController::shuffleList() {
    if(playlist->getPlayList().empty()) {
        wxMessageBox("You can't shuffle an empty playlist!");
        return;
    }
    std::vector<long>indexes;
    long seed = std::chrono::system_clock::now().time_since_epoch().count();
    for (long i = 0; i < playlist->getPlayList().size(); i++) {
        indexes.push_back(i);
    }

    std::shuffle(indexes.begin(), indexes.end(), std::default_random_engine(seed));
    playlist->songChanged(&indexes);
    for(auto i : indexes) {
        cout<< " " << i;
    }
    cout<<"\n"<<endl;

    indexes.clear();
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

}

void MediaController::setLoop() {
    auto tempList = playlist->getPlayList();
    for(auto iter : tempList){
        if(iter->getSongState() == wxMEDIASTATE_PLAYING || iter ->getSongState() == wxMEDIASTATE_PAUSED)
            iter->setLoop(!iter->isLoop());
    }

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

void MediaController::tellPlaylist(wxString songPath){

    playlist->nowPlaying(getSongFromPlaylist(songPath));
}

Song* MediaController::getSongFromPlaylist(wxString path){

    for(auto iter : playlist->getPlayList()){
        if(iter->getSongPath().IsSameAs(path))
            return iter;
    }
}