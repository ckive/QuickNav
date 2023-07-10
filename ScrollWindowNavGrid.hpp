//
// Created by Dan on 7/8/23.
//

#ifndef QUICKNAV_SCROLLWINDOWNAVGRID_HPP
#define QUICKNAV_SCROLLWINDOWNAVGRID_HPP

#include <wx/wx.h>
#include <wx/filename.h>

const int NUM_COLS = 5;

class ScrollWindowNavGrid : public wxScrolledWindow {
public:
    ScrollWindowNavGrid(wxWindow* parent);

    // only way to navigate to next/prev folder is through the first and last elements.
    // other ways of jumping out of index wraps around within the current directory
    void MoveFocusUp();
    void MoveFocusDown();
    void MoveFocusLeft();
    void MoveFocusRight();

    void HandleSpace();

private:
    wxGridSizer *m_gridSizer;
    int m_currentRow;
    int m_currentCol;
    int m_numCols;
    int m_numRows;
    int m_curDirItemCount;


    void SetFocusToCurrentElement();

    bool IsDirectory(const wxFileName &fileName);
    bool IsImageFile(const wxFileName &fileName);
    bool IsVideoFile(const wxFileName &fileName);

};

#endif //QUICKNAV_SCROLLWINDOWNAVGRID_HPP
