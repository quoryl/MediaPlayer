//
// Created by azrael on 31/07/17.
//
#include <memory>
#include "DaeumApp.h"
using namespace std;

wxIMPLEMENT_APP(DaeumApp);


bool DaeumApp::OnInit() {
    /* new model and new controller to pass as parameters to view . But the song may change. The subject is not
     * constant -> FIXME*/
    MainFrame* mainFrame = new MainFrame(nullptr, wxID_ANY, wxT("DaeumApp"), wxDefaultPosition, wxSize(900, 400));
    mainFrame->Show(true);
    return true;

}

