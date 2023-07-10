//
// Created by Dan on 7/3/23.
//

#ifndef QUICKNAV_MAINFRAME_HPP
#define QUICKNAV_MAINFRAME_HPP


#include <wx/wx.h>
#include "ScrollWindowNavGrid.hpp"
#include <stack>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, int maxX, int maxY);
    void OnKeyUp(wxKeyEvent &event);
    void OnCharHook(wxKeyEvent& event);

    void focusWindowsPush(wxWindow* window);
    wxWindow* focusWindowsPop();

private:

    wxString curDir;

    std::stack<wxString> m_baseDirs;     // stack of tagged basedirs

    std::stack<wxWindow*> m_focusWindows;     // stack of focus windows
//    m_focusWindows.push(new wxWindow());

    wxStatusBar* statusBar;


    void DiveIntoDirectory(wxString folderName);

    wxBoxSizer* m_mainSizer;


        // scroll window
    ScrollWindowNavGrid* m_scrollableWindow;


    // tracking double key press
    bool qDoublePress = false;  // quit
    bool bDoublePress = false;  // untag base
    bool fDoublePress = false;  // unflatten
    // tracking state of the shortcut keys
    bool qKeyPressed = false;   // modifier Key Q
    bool hKeyPressed = false;   // help
    bool rKeyPressed = false;   // random choice from base
    bool sKeyPressed = false;   // shuffle
    bool bKeyPressed = false;   // tag new base
    bool fKeyPressed = false;   // flatten
    bool gKeyPressed = false;   // gallery
    bool pKeyPressed = false;   // popup for navivating to new path
    bool eKeyPressed = false;   // cursor jump to end
    bool tKeyPressed = false;   // terminal popup

    // timer
    void onTimer(wxTimerEvent &event);

    wxTimer* dblclick_timer;



};



#endif //QUICKNAV_MAINFRAME_HPP
