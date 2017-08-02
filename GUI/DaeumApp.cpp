//
// Created by azrael on 31/07/17.
//
#include <memory>
#include "DaeumApp.h"

using namespace std;

wxIMPLEMENT_APP(DaeumApp);

bool DaeumApp::OnInit() {

    unique_ptr<MainFrame> mainFrame(new MainFrame(nullptr, wxID_ANY, wxT("Some Title")));
    mainFrame->Show(true);
    return true;
}
