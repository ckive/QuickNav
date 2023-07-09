//
// Created by Dan on 7/3/23.
//

#ifndef QUICKNAV_MAINFRAME_HPP
#define QUICKNAV_MAINFRAME_HPP


#include <wx/wx.h>
#include "ScrollWindowNavGrid.hpp"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, int maxX, int maxY);
private:

//    void onKeyUp(wxKeyEvent &event);

//    void onKeyDown(wxKeyEvent &event);

//    void onKeyEvent(wxKeyEvent &event);




    // keyboard shortcut functions

    // scroll window
    ScrollWindowNavGrid* scrollableWindow;


};



#endif //QUICKNAV_MAINFRAME_HPP
