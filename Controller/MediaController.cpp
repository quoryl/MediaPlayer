//
// Created by azrael on 31/07/17.
//



#include "MediaController.h"
#include "../Model/Song.h"

MediaController::MediaController(Song *s) : song(s) {}

void MediaController::Search() {

}

void MediaController::AddFile(wxFilePickerCtrl *filePicker, wxListCtrl *list, wxMediaCtrl *mediaController) {

    auto path = filePicker->GetPath();

    //we need a const char* for FileName constructor so we need cast path(wxString)
    const char* charPath = (path.mbc_str());
    TagLib::FileRef TagMain{TagLib::FileName(charPath)};

    if(!TagMain.isNull() && TagMain.tag()) {

        TagLib::Tag *tag = TagMain.tag();
        wstring _tagTitle = (tag->title()).toWString();
        wxString tagTitle(_tagTitle);
//FIXME ^

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
        //list->SetItem(nID, 1, wxFileName(path).GetName());
        list->SetItem(nID, 1, tagTitle);
        list->SetItem(nID, 2, wxT("Unknown"));
        list->SetItem(nID, 3, wxT("Unknown"));

        if (nID % 2) {
            listItem.SetBackgroundColour(wxColour(255, 235, 200));
            list->SetItem(listItem);
        }

    }
}

void MediaController::Delete() {

}

void MediaController::Shuffle() {

}

void MediaController::Previous() {

}

void MediaController::Play() {

}

void MediaController::Next() {

}

void MediaController::Repeat() {

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
