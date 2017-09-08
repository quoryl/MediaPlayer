//
// Created by azrael on 31/07/17.
//

#include "DaeumApp.h"

using namespace std;

wxIMPLEMENT_APP(DaeumApp);


bool DaeumApp::OnInit() {
    auto playlist = new Playlist;
    auto controller = new MediaController(playlist);
    MainFrame* mainFrame = new MainFrame(controller, playlist, nullptr, wxID_ANY, wxT("musicPlayer"), wxDefaultPosition,wxSize(1000, 400));

    mainFrame->Show(true);
    return true;

}
