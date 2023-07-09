//
// Created by Dan on 7/3/23.
//

#include "MainApp.hpp"
#include "MainFrame.hpp"
#include <iostream>

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    // Get the screen size
    wxSize screenSize = wxGetDisplaySize();

    // Retrieve the screen width and height
    int screenWidth = screenSize.GetWidth();
    int screenHeight = screenSize.GetHeight();

    MainFrame* mainframe = new MainFrame("Quick Nav v1.0", screenWidth, screenHeight);
    mainframe->Show();
    return true;
}

