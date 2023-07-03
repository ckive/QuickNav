//
// Created by Dan on 7/3/23.
//

#ifndef QUICKNAV_MAINFRAME_HPP
#define QUICKNAV_MAINFRAME_HPP


#include <wx/wx.h>

class mainFrame : public wxFrame {
public:
    mainFrame(const wxString &title, int maxX, int maxY);
};


#endif //QUICKNAV_MAINFRAME_HPP
