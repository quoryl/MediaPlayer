//
// Created by azrael on 31/07/17.
//



#include "MediaController.h"


MediaController::MediaController(Playlist* pList) : playlist(pList){

}

void MediaController::searchItem(wxString text) {


}

void MediaController::addFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *mediaControl) {

    wxString path = filePicker->GetPath();
    auto fileName = filePicker -> GetFileName();
    if(fileName.IsOk()) {
        bool a = mediaControl -> Load(path);
        std::cout << a;
    }
        wxListItem listItem;
        listItem.SetAlign(wxLIST_FORMAT_LEFT);

        int nID;

        listItem.SetId(nID = list->GetItemCount());
        listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_STATE);
        listItem.SetState(wxLIST_STATE_SELECTED);
        listItem.SetData(new wxString(path));

        //cast an int to a wxString

        wxString s;
        s << nID;


        list->InsertItem(listItem);
        list->SetItem(nID, 0, s);
        list->SetItem(nID, 1, wxFileName(path).GetName());
        list->SetItem(nID, 2, wxT("Unknown"));
        list->SetItem(nID, 3, wxT("Unknown"));


}


void MediaController::deleteSong() {

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



map<wxString, wxString> MediaController::getMetadata(wxFilePickerCtrl *picker) {
    /*//we need a const char* for FileName constructor so we need cast path(wxString)
     using namespace TagLib;

     wxString filePath = picker -> GetPath();

     const char* charPath = (filePath.mbc_str());
     FileRef TagMain{FileName(charPath)};

    if(!TagMain.isNull() && TagMain.tag()) {

         TagLib::Tag *tag = TagMain.tag();
         wstring _tagTitle = (tag->title()).toWString();
         wstring _tagAlbum = (tag->album()).toWString();
         wstring _tagArtist = (tag->artist()).toWString();
         wstring _tagGenre = (tag->genre()).toWString();

         wxString tagTitle(_tagTitle);
         wxString tagAlbum(_tagAlbum);
         wxString tagArtist(_tagArtist);
         wxString tagGenre(_tagGenre);

         std::map<wxString,wxString> metadata = {{"title", tagTitle}, {"album", tagAlbum}, {"artist", tagArtist},{"genre", tagGenre}};
         return metadata;

    }
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