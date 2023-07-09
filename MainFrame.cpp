//
// Created by Dan on 7/3/23.
//

#include <filesystem>
#include <wx/stdpaths.h>
#include "MainFrame.hpp"

MainFrame::MainFrame(const wxString &title, int maxX, int maxY)
        : wxFrame(nullptr, wxID_ANY, title) {
    this->SetMinSize(wxSize(maxX / 2, maxY / 2));
    this->SetClientSize(this->GetMinSize());

    // get cur path
    std::filesystem::path currentPath = std::filesystem::current_path();
    wxString cwd = currentPath.string();

    // status bar
    wxStatusBar* statusBar = this->CreateStatusBar();
    statusBar->SetFieldsCount(3);
    statusBar->SetStatusText(cwd);
    int widths[] = { -1, -1, -1 }; // Widths for each section (-1 means stretchable)
    statusBar->SetStatusWidths(3, widths);
    statusBar->SetStatusText("wxLog: ", 0);
    statusBar->SetStatusText("BASE Dir:", 1);
    statusBar->SetStatusText("Section 3", 2);

    scrollableWindow = new ScrollWindowNavGrid(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(scrollableWindow, 1, wxEXPAND);

    this->SetSizer(mainSizer);

}
