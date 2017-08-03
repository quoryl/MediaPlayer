//
// Created by azrael on 31/07/17.
//
#include <memory>
#include "DaeumApp.h"
#include "../Model/Song.h"
#include "../Controller/MediaController.h"
using namespace std;

wxIMPLEMENT_APP(DaeumApp);


bool DaeumApp::OnInit() {

    MainFrame* mainFrame = new MainFrame(nullptr, wxID_ANY, wxT("DaeumApp"), wxDefaultPosition, wxSize(900, 400));
    mainFrame->Show(true);
    return true;

}

