//
// Created by azrael on 31/07/17.
//

#include "DaeumApp.h"

using namespace std;

//wxIMPLEMENT_APP(DaeumApp);

bool DaeumApp::OnInit() {
    playlist = new Playlist;
    controller = new MediaController(playlist);
    mainFrame = new MainFrame(controller, playlist, nullptr, wxID_ANY, wxT("musicPlayer"), wxDefaultPosition,wxSize(1000, 400));
    mainFrame->Show(true);
    //this way the program automatically loads the last saved playlist the user can do it manually as well)
    controller->load();
    return true;

}
int DaeumApp::OnExit() {
    delete playlist;
    delete controller;
    // it gives a SIGSEGV; I suppose that since it is derived from wxFrame wxWidgets takes care of this class
    //delete mainFrame;

}