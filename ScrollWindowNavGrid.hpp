//
// Created by Dan on 7/8/23.
//

#ifndef QUICKNAV_SCROLLWINDOWNAVGRID_HPP
#define QUICKNAV_SCROLLWINDOWNAVGRID_HPP

#include <stack>
#include <filesystem>
namespace fs = std::filesystem;

#include <wx/wx.h>
#include <wx/filename.h>


const int NUM_COLS = 5;
const float SHRINKAGE = 0.9;

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
    wxWindow* m_parent;
    int maxWidth;
    int maxHeight;
    wxSize buttonSize;

    fs::path curBaseDir;

    std::stack<fs::path> baseDirs;

    // Create a box sizer to hold the icons
    wxBoxSizer* m_scrollableSizer;

    wxGridSizer *m_gridSizer;
    int m_currentRow;
    int m_currentCol;
    int m_numCols;
    int m_numRows;
    int m_curDirItemCount;


    void SetFocusToCurrentElement();

    bool IsDirectory(fs::path &fileName);
    bool IsImageFile(fs::path &fileName);
    bool IsVideoFile(fs::path &fileName);
    void ReRender(fs::path &curdir);

};

#endif //QUICKNAV_SCROLLWINDOWNAVGRID_HPP
