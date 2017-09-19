//
// Created by azrael on 18/09/17.
//

#ifndef DAEUM_DRAGANDDROP_H
#define DAEUM_DRAGANDDROP_H
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/dnd.h> // drag and drop header
#include "MainFrame.h"

class MediaController;
class MainFrame;
class DragAndDrop : public wxFileDropTarget {


public:
    explicit DragAndDrop(MainFrame* frame);
    bool IsFormatSupported(wxString fileName);


private:
    MainFrame* target;
    MediaController* ctrl;
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& files) override;


};


#endif //DAEUM_DRAGANDDROP_H
