//
// Created by azrael on 18/09/17.
//


#include "DragAndDrop.h"
DragAndDrop::DragAndDrop(MainFrame* frame) : target(frame){}

bool DragAndDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &files) {
    wxArrayString validPaths;
    for(auto iter : files){
        if(wxFileName(iter).GetExt() == "mp3" || wxFileName(iter).GetExt() == "wav"){
            validPaths.push_back(iter);
        }
        else
            cout << "the file is not supported" << endl;
    }
    if(target != nullptr && target->controller != nullptr) {
        target->controller->addFile(&validPaths);
        return true;
    }
    return false;
}

