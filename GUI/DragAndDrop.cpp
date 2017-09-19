//
// Created by azrael on 18/09/17.
//


#include "DragAndDrop.h"
DragAndDrop::DragAndDrop(MainFrame* frame) : target(frame), ctrl(target->controller){}

bool DragAndDrop::IsFormatSupported(wxString fileName) {

    if(wxFileName(fileName).GetExt() == "mp3")
        return true;
    else{
        cout << "the file is not supported" << endl;
        return false;
    }

}

bool DragAndDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &files) {
    wxArrayString validPaths;
    for(auto iter : files){
        if(IsFormatSupported(iter)){
            validPaths.push_back(iter);
        }
    }
    ctrl->addFile(&validPaths);
}

