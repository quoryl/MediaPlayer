//
// Created by azrael on 31/07/17.
//

#include "DaeumApp.h"

using namespace std;

wxIMPLEMENT_APP(DaeumApp);


bool DaeumApp::OnInit() {
    auto playlist = new Playlist;
    auto controller = new MediaController(playlist);
    MainFrame* mainFrame = new MainFrame(controller, playlist, nullptr, wxID_ANY,
                                         wxT("DaeumApp"), wxDefaultPosition,wxSize(900, 400));

    mainFrame->Show(true);

    return true;

}
