//
// Created by Dan on 7/3/23.
//

#include "mainFrame.hpp"
#include <iostream>

mainFrame::mainFrame(const wxString &title, int maxX, int maxY)
        : wxFrame(nullptr, wxID_ANY, title) {
    SetMinSize(wxSize(maxX / 2, maxY / 2));
    SetClientSize(GetMinSize());
}