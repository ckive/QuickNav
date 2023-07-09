//
// Created by Dan on 7/8/23.
//

#ifndef QUICKNAV_SCROLLWINDOWNAVGRID_HPP
#define QUICKNAV_SCROLLWINDOWNAVGRID_HPP

#include <wx/wx.h>

const int NUM_COLS = 5;

class ScrollWindowNavGrid : public wxScrolledWindow {
public:
    ScrollWindowNavGrid(wxWindow *parent);

    void OnCharHook(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent &event);

private:
    wxWindow *m_parent;
    wxGridSizer *m_gridSizer;
    int m_currentRow;
    int m_currentCol;
    int m_numCols;
    int m_numRows;
    int m_curDirItemCount;



    // only way to navigate to next/prev folder is through the first and last elements.
    // other ways of jumping out of index wraps around within the current directory
    void MoveFocusUp();
    void MoveFocusDown();
    void MoveFocusLeft();
    void MoveFocusRight();
    void SetFocusToCurrentElement();

//    void OnKeyDown(wxKeyEvent &event);



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

class HelpDialog : public wxDialog {
public:
    HelpDialog(wxWindow* parent);
};

#endif //QUICKNAV_SCROLLWINDOWNAVGRID_HPP
