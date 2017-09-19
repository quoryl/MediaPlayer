//
// Created by azrael on 18/09/17.
//


#include "DragAndDrop.h"
DragAndDrop::DragAndDrop(MainFrame* frame) : target(frame), ctrl(target->controller){}

bool DragAndDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &files) {
    wxArrayString validPaths;
    for(auto iter : files){
        if(wxFileName(iter).GetExt() == "mp3" || wxFileName(iter).GetExt() == "wav"){
            validPaths.push_back(iter);
        }
        else
            cout << "the file is not supported" << endl;
    }
    ctrl->addFile(&validPaths);
}

