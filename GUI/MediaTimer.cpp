//
// Created by azrael on 31/07/17.
//

#include "MediaTimer.h"
void MediaTimer::Notify() {

    wxMediaCtrl *media = m_frame->mediaCtrl;

    // Number of minutes/seconds total
    wxLongLong llLength = media->Length();
    int nMinutes = (int) (llLength / 60000).GetValue();
    int nSeconds = (int) ((llLength % 60000) / 1000).GetValue();

    // Duration string (i.e. MM:SS)
    wxString sDuration;
    sDuration.Printf(wxT("%2i:%02i"), nMinutes, nSeconds);


    // Number of minutes/seconds total
    wxLongLong llTell = media->Tell();
    nMinutes = (int) (llTell / 60000).GetValue();
    nSeconds = (int) ((llTell % 60000) / 1000).GetValue();

    // Position string (i.e. MM:SS)
    wxString sPosition;
    sPosition.Printf(wxT("%2i:%02i"), nMinutes, nSeconds);



    // Setup the slider and gauge min/max values
    m_frame->mediaSlider->SetRange(0, (int) (llLength / 1000).GetValue());



    // if the slider is not being dragged then update it with the song position
    if (!m_frame->IsBeingDragged)
        m_frame->mediaSlider->SetValue((long) (llTell / 1000).GetValue());

    m_frame->statusBar->SetStatusText(sPosition + "/" + sDuration , 2);
}

