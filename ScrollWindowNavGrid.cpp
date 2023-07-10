//
// Created by Dan on 7/8/23.
//

#include "MainFrame.hpp"
#include "ScrollWindowNavGrid.hpp"
#include "FolderButton.hpp"

#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/artprov.h>
#include <wx/filename.h>
#include <wx/mimetype.h>


ScrollWindowNavGrid::ScrollWindowNavGrid(wxWindow* parent)
        : wxScrolledWindow(parent, wxID_ANY),
          m_currentRow(0),
          m_currentCol(0),
          m_numCols(NUM_COLS)
{
    wxInitAllImageHandlers();

    // Create a box sizer to hold the icons
    wxBoxSizer* scrollableSizer = new wxBoxSizer(wxVERTICAL);


    // base dir
    // wxString directoryPath = "/Users/dan/Desktop/uwcold/vsco";
//    wxString directoryPath = "/Users/dan/Desktop/uwcold/instaloader";
    wxString directoryPath = "/Users/dan/Desktop/uwcold/fav2";
//    wxArrayString directoryContents;
    std::vector<wxFileName> directoryContents;


    wxDir dir(directoryPath);
    if (!dir.IsOpened())
    {
        wxLogError("Failed to open directory: %s", directoryPath);
        return;
    }

    wxString directoryElement;
    bool cont = dir.GetFirst(&directoryElement, wxEmptyString, wxDIR_DIRS|wxDIR_FILES);
    while (cont)
    {
        wxFileName fileElement = wxFileName(directoryPath, directoryElement);
        directoryContents.push_back(fileElement);
        cont = dir.GetNext(&directoryElement);
    }

////     this is for flattening. it recursively gets all files
//    m_curDirItemCount = (int)dir.GetAllFiles(directoryPath, &directoryContents);

    // sort alphabetically
//    directoryContents.Sort();
    std::sort(directoryContents.begin(), directoryContents.end(), [](const wxFileName& fileName1, const wxFileName& fileName2) {
        return fileName1.GetFullName().CmpNoCase(fileName2.GetFullName()) < 0;
    });

    // set number of rows now
    m_curDirItemCount = (int)directoryContents.size();
    m_numRows = m_curDirItemCount / m_numCols;
    if (directoryContents.size()%m_numCols != 0)
        m_numRows++;

    // instead of from DIP, get from current client size or something.
//    const int WIDTH = FromDIP(100);
//    const int HEIGHT = FromDIP(100);

    const int WIDTH = 100;
    const int HEIGHT = 100;

    m_gridSizer = new wxGridSizer(m_numCols);

    wxBitmapBundle folderIconBundle = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, wxSize(WIDTH, HEIGHT));
    wxBitmapBundle questionIconBundle = wxArtProvider::GetBitmapBundle(wxART_QUESTION, wxART_OTHER, wxSize(WIDTH,
                                                                                                           HEIGHT));
    wxBitmapBundle videoIconBundle = wxArtProvider::GetBitmapBundle(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(WIDTH,
                                                                                                         HEIGHT));

    // Load and add images to the grid sizer
    for (const wxFileName& dirElement : directoryContents)
    {
        wxButton* folderButton;
        if (IsDirectory(dirElement)){
            folderButton = new FolderButton(this, folderIconBundle,dirElement.GetName());
        }else if (IsImageFile(dirElement)){
//            std::printf("IMAGE\n");
//            wxPrintf("fp: %s\n", dirElement.GetFullPath());
//            wxImage image(dirElement.GetFullPath());
            wxImage image;
            if (image.LoadFile(dirElement.GetFullPath())){
                image.Rescale(WIDTH, HEIGHT, wxIMAGE_QUALITY_NEAREST);  // fastest
//                image.Rescale(WIDTH, HEIGHT, wxIMAGE_QUALITY_HIGH);  // best
                wxBitmap bitmap(image);
                wxBitmapBundle bitmapBundle(bitmap);
                folderButton = new FolderButton(this, bitmapBundle,dirElement.GetName());
            }else{
                folderButton = new FolderButton(this, videoIconBundle,dirElement.GetName());
            }

        }else if (IsVideoFile(dirElement)){
            folderButton = new FolderButton(this, videoIconBundle,dirElement.GetName());
            std::printf("VIDEO\n");
        }else{
            // question mark
            std::printf("QUESTIONMARK");
            folderButton = new FolderButton(this, questionIconBundle,dirElement.GetName());
        }

        folderButton->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, dynamic_cast<MainFrame*>(parent));
        m_gridSizer->Add(folderButton, 0, wxALIGN_CENTER | wxALL, 10);
    }
    // set first in focus
    wxWindow* firstwindow = m_gridSizer->GetChildren().GetFirst()->GetData()->GetWindow();
    dynamic_cast<MainFrame*>(parent)->focusWindowsPush(firstwindow);
    firstwindow->SetFocus();

    scrollableSizer->Add(m_gridSizer, 0, wxEXPAND);
    SetSizer(scrollableSizer);

    SetScrollRate(10, 10);
    this->EnableScrolling(true, true);
}





void ScrollWindowNavGrid::MoveFocusUp() {
    m_currentRow--;
    if (m_currentRow < 0) {
        m_currentRow = m_numRows-1;
    }
    if (m_currentRow*m_numCols + m_currentCol >= m_curDirItemCount){
        // OOR wrap around
        m_currentRow--;
    }
    SetFocusToCurrentElement();
}

void ScrollWindowNavGrid::MoveFocusDown() {
    m_currentRow++;
    if (m_currentRow >= m_numRows) {
        m_currentRow = 0;
    }
    if (m_currentRow*m_numCols + m_currentCol >= m_curDirItemCount){
        // OOR go to first item (0,0)
        m_currentRow = 0;
    }
    SetFocusToCurrentElement();
}

void ScrollWindowNavGrid::MoveFocusLeft() {
    m_currentCol--;
    if (m_currentCol < 0) {
        m_currentCol = m_numCols-1;
        m_currentRow--;
    }
    if (m_currentRow < 0){
        // TODO go to prev folder (for now go to end of cur folder)
        m_currentRow = m_numRows-1;
        m_currentCol = m_curDirItemCount/m_numRows -1;
    }

    SetFocusToCurrentElement();
}

void ScrollWindowNavGrid::MoveFocusRight() {
    m_currentCol++;
    if (m_currentCol >= m_numCols) {
        m_currentCol = 0;
        m_currentRow++;
    }
    // if out of
    if (m_currentRow*m_numCols + m_currentCol >= m_curDirItemCount){
        // TODO goto next folder (for now go to start of cur folder)
        m_currentRow = 0;
        m_currentCol = 0;
    }
    SetFocusToCurrentElement();
}

void ScrollWindowNavGrid::SetFocusToCurrentElement() {
    int index = m_currentRow * m_numCols + m_currentCol;
    std::printf("init: %d    curRow: %d      curCol: %d\n",index, m_currentRow, m_currentCol);
    wxWindow* child = m_gridSizer->GetItem(index)->GetWindow();
    if (child) {
        child->SetFocus();
    }
}

bool ScrollWindowNavGrid::IsDirectory(const wxFileName& fileName)
{
    return wxFileName::DirExists(fileName.GetFullPath());
}

bool ScrollWindowNavGrid::IsImageFile(const wxFileName& fileName)
{
    wxString extension = fileName.GetExt().Lower();

    // List of supported image file extensions
    static const wxString supportedExtensions[] = {
            "bmp", "jpg", "jpeg", "png", "gif", "tiff"
            // Add more supported image file extensions as needed
    };
    for (const wxString& supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
        {
            return true;
        }
    }
    return false;
}

bool ScrollWindowNavGrid::IsVideoFile(const wxFileName& fileName)
{
    wxString extension = fileName.GetExt().Lower();

    // List of supported video file extensions
    static const wxString supportedExtensions[] = {
            "mp4", "avi", "mov", "mkv", "wmv"
            // Add more supported video file extensions as needed
    };

    // Check if the file extension is in the list of supported extensions
    for (const wxString& supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
        {
            return true;
        }
    }

    return false;
}



// Either enters a dir or displays media
void ScrollWindowNavGrid::HandleSpace() {
    /*
     * if focus window is directory
     *      add cur path to stack
     *
     */
}



