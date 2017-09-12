//
// Created by azrael on 11/09/17.
//

#include "wx/wx.h"
#include "GUI/DaeumApp.h"


IMPLEMENT_APP_NO_MAIN(DaeumApp);

int main(int argc, char* argv[]){
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    
    return 0;
}