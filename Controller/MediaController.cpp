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
    if(paths != nullptr) {
        auto tempList = playlist->getPlayList();
        for (auto i: *paths) {
            if (wxFileName(i).FileExists()) {
                auto metapath = static_cast<const char*>(i);
                TagReader tags(metapath);
                bool found = false;
                if (tempList.empty()) {
                    Song *song = new Song(tags.getTitle(), tags.getArtist(), tags.getAlbum(), 0, i);
                    song->setID(playlist->getPlayList().size());
                    playlist->addToPlaylist(song); //to std::list

                } else {

                    for (auto g : tempList) {
                        if ((g->getSongPath()).IsSameAs(i)) {
                            found = true;
                        }
                    }
                    if (!found) {
                        Song *song = new Song(tags.getTitle(), tags.getArtist(), tags.getAlbum(), 0, i);
                        playlist->addToPlaylist(song); //to std::list

                    }

                }
            } else {
                std::cerr << "\nThe given file path: " << i << " is not valid" << std::endl;
            }
        }
    }

}
void MediaController::deleteSong(wxArrayString toDeletePath) {
    for(auto iter : toDeletePath){
        if(wxFileName(iter).FileExists() && getSongFromPlaylist(iter) != nullptr){
            playlist->deleteFromPlaylist(getSongFromPlaylist(iter));
        }
    }
}


void MediaController::prevSong() {
    auto toPlay = playlist->getPlaying();
    if(toPlay != nullptr) {
        if (toPlay->isLoop()) {
            tellPlaylist(toPlay->getSongPath());
        }
        else {
            if (toPlay->getID() - 1 != -1) {
                auto nextSongID = toPlay->getID() - 1;
                for (auto iter: playlist->getPlayList())
                    if (iter->getID() == nextSongID)
                        tellPlaylist(iter->getSongPath());
            }
        }
    }
}

void MediaController::nextSong() {
    auto toPlay = playlist->getPlaying();
    if(toPlay != nullptr) {
        if (toPlay->isLoop()) {
            tellPlaylist(toPlay->getSongPath());
        }
        else {
            if (toPlay->getID() + 1 < playlist->getPlayList().size()) {
                auto nextSongID = toPlay->getID() + 1;
                for (auto iter: playlist->getPlayList())
                    if (iter->getID() == nextSongID)
                        tellPlaylist(iter->getSongPath());
            }
        }
    }
}


void MediaController::setLoop() {
    auto songToLoop = playlist->getPlaying();
    if(songToLoop != nullptr) {
        songToLoop->setLoop(!songToLoop->isLoop());
    }
}

void MediaController::shuffleList() {
    if(!playlist->getPlayList().empty()) {

        std::vector<long> indexes;
        long seed = std::chrono::system_clock::now().time_since_epoch().count();
        //the indexes are always in this order
        for (long i = 0; i < playlist->getPlayList().size(); i++) {
            indexes.push_back(i);
        }

        std::shuffle(indexes.begin(), indexes.end(), std::default_random_engine(seed));
        playlist->songChanged(&indexes);

        indexes.clear();
    }
}
bool MediaController::save() {
    wxFileOutputStream fileOStream(wxT("savedSession.txt"));
    if (fileOStream.IsOk() && !playlist->getPlayList().empty()) {
        wxTextOutputStream textOStream(fileOStream);
        auto tmp = playlist->getPlayList();
        for (auto i:tmp) {
            textOStream << i->getSongPath() << "\n";
        }
        cout << "the list was saved" << endl;
        return true;
    } else {
        cout << "wxFileOutputStream object not available" <<endl;
        return false;
    }
}

bool MediaController::load() {

    wxTextFile text(wxT("savedSession.txt"));
    if(text.Exists()) {
        text.Open();

        if (!text.GetFirstLine().IsSameAs(wxEmptyString)) {
            wxArrayString tmp;
            auto str = text.GetFirstLine();
            while(!text.Eof()){
                if(wxFileName(str).FileExists())
                    tmp.Add(str);
                str = text.GetNextLine();
            }
            text.Close();
            addFile(&tmp);
            cout<<"everything okay, songs added" << endl;
        }
        else
            cout << "Empty file" << endl;
        return true;
    }
    else {
        cout << "The file doesn't exist" << endl;
        return false;
    }
}

void MediaController::tellPlaylist(wxString songPath){
    if(wxFileName(songPath).FileExists())
        playlist->nowPlaying(getSongFromPlaylist(songPath));
}

Song* MediaController::getSongFromPlaylist(wxString path){
    if(path != wxEmptyString && wxFileName(path).FileExists()) {
        for (auto iter : playlist->getPlayList()) {
            if (iter->getSongPath().IsSameAs(path))
                return iter;
        }
    }
    else
        return nullptr;
}
