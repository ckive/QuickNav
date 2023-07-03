//
// Created by Dan on 7/3/23.
//

#include "mainApp.hpp"
#include "mainFrame.hpp"
#include <iostream>

wxIMPLEMENT_APP(mainApp);

bool mainApp::OnInit() {
    // Get the screen size
    wxSize screenSize = wxGetDisplaySize();

    // Retrieve the screen width and height
    int screenWidth = screenSize.GetWidth();
    int screenHeight = screenSize.GetHeight();

    mainFrame* mainframe = new mainFrame("Quick Nav v1.0", screenWidth, screenHeight);
    mainframe->Show();
    return true;
}

