//
// Created by Dan on 7/8/23.
//

#include "MainFrame.hpp"
#include "ScrollWindowNavGrid.hpp"
#include "FolderButton.hpp"


#include <algorithm>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;


#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <wx/artprov.h>
#include <wx/display.h>
#include <wx/object.h>


ScrollWindowNavGrid::ScrollWindowNavGrid(wxWindow* parent)
        : wxScrolledWindow(parent, wxID_ANY),
          m_currentRow(0),
          m_currentCol(0),
          m_numCols(NUM_COLS),
          m_parent(parent)
{
    wxInitAllImageHandlers();

    // Get the screen size
    wxDisplay display;
    wxRect screenRect = display.GetGeometry();
    int screenWidth = screenRect.GetWidth();
    int screenHeight = screenRect.GetHeight();

    // Calculate the maximum button size based on screen size
    maxWidth = (screenWidth / NUM_COLS)*0.9;
    maxHeight = (screenHeight / NUM_COLS)*0.9;

    buttonSize = wxSize(maxWidth, maxHeight);


    // base dir
//     wxString directoryPath = "/Users/dan/Desktop/uwcold/vsco";
//     wxString curDirPath = "/Users/dan/Documents";
//    wxString directoryPath = "/Users/dan/Desktop/uwcold/instaloader";
//    wxString directoryPath = "/Users/dan/Desktop/uwcold";
//    wxArrayString directoryContents;

    // base dir tagging
    curBaseDir = "/Users/dan/Documents";
    baseDirs.push(curBaseDir);

    // sequence of directory traversal
    curDir = curBaseDir;
    curDirStack.push(curDir);

    m_scrollableSizer = new wxBoxSizer(wxVERTICAL);

    ReRender(curBaseDir);

    // where rerender used to be

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
//        m_currentCol = m_curDirItemCount/m_numRows -1;
        m_currentCol = m_curDirItemCount % m_numCols - 1;
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
    std::printf("index: %d    curRow: %d      curCol: %d\n",index, m_currentRow, m_currentCol);
    wxWindow* child = m_gridSizer->GetItem(index)->GetWindow();
    if (child) {
        child->SetFocus();
        // print out full path of child element
        std::cout << "In focus" << std::endl;
        std::cout << child->GetLabel() << std::endl;
        FolderButton* c = wxDynamicCast(child, FolderButton);
//        FolderButton* c = dynamic_cast<FolderButton*>(child);
        if (c){
            // successfully converted
            //        const wxFileName& curfn = c->getFileName();
            std::cout << "successfully converted" << std::endl;
//            wxString fstring = c->getItemName();
//            std::cout << fstring << std::endl;
        }else{
            std::cout << "wtf fail dynamic type conversion" << std::endl;
        }



    }
}

bool ScrollWindowNavGrid::IsDirectory(fs::path& fileName)
{
    return fs::is_directory(fileName);
}

bool ScrollWindowNavGrid::IsImageFile(fs::path& fileName)
{
//    wxString extension = fileName.GetExt().Lower();
    std::string extension = fileName.extension();

    // List of supported image file extensions
    static const std::string supportedExtensions[] = {
            ".bmp", ".jpg", ".jpeg", ".png", ".gif", ".tiff"
    };
    for (const std::string& supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
        {
            return true;
        }
    }
    return false;
}

bool ScrollWindowNavGrid::IsVideoFile(fs::path& fileName)
{
    auto extension = fileName.extension();

    // List of supported video file extensions
    static const std::string supportedExtensions[] = {
            ".mp4", ".avi", ".mov", ".mkv", ".wmv"
    };

    // Check if the file extension is in the list of supported extensions
    for (const auto& supportedExtension : supportedExtensions)
    {
        if (extension == supportedExtension)
        {
            return true;
        }
    }

    return false;
}

void ScrollWindowNavGrid::ReRender(fs::path &curdir) {
    // here we render the grid with information given the current dir

    // for each FolderButton in grid, delete it
    m_scrollableSizer->Clear(true);

//    std::vector<fs::path> directoryContents;
    std::vector<fs::directory_entry> directoryContents;
    std::cout << "Cur dir: ";
    std::cout << curdir << std::endl;

    for (const auto& dir_entry : fs::directory_iterator(curdir)){
        if (!dir_entry.path().filename().empty() && !dir_entry.path().filename().string().empty() && dir_entry.path().filename().string()[0] == '.') {
            continue; // Skip hidden files
        }
        directoryContents.push_back(dir_entry);
    }

    // sort alphabetically
    std::sort(directoryContents.begin(), directoryContents.end());

    // set number of rows now
    m_curDirItemCount = (int)directoryContents.size();
    std::printf("TOTAL COUNT IN DIR: %d", m_curDirItemCount);
    m_numRows = m_curDirItemCount / m_numCols;
    if (directoryContents.size()%m_numCols != 0)
        m_numRows++;

    m_gridSizer = new wxGridSizer(m_numCols);

    wxSize defaultIconSize = wxSize(maxWidth*0.9, maxHeight*0.9);

    wxBitmapBundle folderIconBundle = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, defaultIconSize);
    wxBitmapBundle questionIconBundle = wxArtProvider::GetBitmapBundle(wxART_QUESTION, wxART_OTHER, defaultIconSize);
    wxBitmapBundle videoIconBundle = wxArtProvider::GetBitmapBundle(wxART_MISSING_IMAGE, wxART_OTHER, defaultIconSize);

    // Load and add images to the grid sizer
    for (fs::path dirElement : directoryContents)
    {
        wxString label = wxString(dirElement.stem());
        wxButton* folderButton;
        if (IsDirectory(dirElement)){
            folderButton = new FolderButton(this, folderIconBundle, label, buttonSize,
                                            dirElement);
        }else if (IsImageFile(dirElement)){
            wxImage image;
            if (image.LoadFile(wxString(dirElement))){
                // get aspect ratio
                int width = image.GetWidth();
                int height = image.GetHeight();
                if (width > maxWidth || height > maxHeight)
                {
                    double aspectRatio = static_cast<double>(width) / height;
                    if (aspectRatio > 1.0)
                    {
                        width = maxWidth*0.9;
                        height = static_cast<int>(width / aspectRatio)*0.9;
                    }
                    else
                    {
                        height = maxHeight*0.9;
                        width = static_cast<int>(height * aspectRatio)*0.9;
                    }
                }else{
                    std::printf("WHY HERE?\n");
                }

                image.Rescale(width, height, wxIMAGE_QUALITY_NEAREST);  // fastest
//                image.Rescale(WIDTH, HEIGHT, wxIMAGE_QUALITY_NEAREST);  // fastest
//                image.Rescale(WIDTH, HEIGHT, wxIMAGE_QUALITY_HIGH);  // best
                wxBitmap bitmap(image);
                wxBitmapBundle bitmapBundle(bitmap);
                folderButton = new FolderButton(this, bitmapBundle,label,buttonSize, dirElement);
            }else{
                folderButton = new FolderButton(this, videoIconBundle,label,buttonSize, dirElement);
            }

        }else if (IsVideoFile(dirElement)){
            folderButton = new FolderButton(this, videoIconBundle,label,buttonSize,dirElement);
        }else{
            // question mark
            folderButton = new FolderButton(this, questionIconBundle,label,buttonSize, dirElement);
        }

        folderButton->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, dynamic_cast<MainFrame*>(m_parent));
        m_gridSizer->Add(folderButton, 1, wxALIGN_CENTER | wxALL, 5);
    }
    // check we have children right
    std::printf("Have %d items\n", (int)m_gridSizer->GetItemCount());

    // set first in focus
    wxWindow* firstwindow = m_gridSizer->GetChildren().GetFirst()->GetData()->GetWindow();
    dynamic_cast<MainFrame*>(m_parent)->focusWindowsPush(firstwindow);
    firstwindow->SetFocus();

    m_scrollableSizer->Add(m_gridSizer, 0, wxEXPAND);
    SetSizer(m_scrollableSizer);

    // rerender
    Layout();
}

// Either enters a dir or displays media
void ScrollWindowNavGrid::HandleSpace() {
    /*
     * push curdir to stack
     * if focus window is directory
     *      update internal directory, rerender grid with new directory items
     * if media
     *      window popup of the image, (subsequent keypresses kills this window and renders another window)
     *
     */
    curDirStack.push(curDir);

    // current window
    int index = m_currentRow * m_numCols + m_currentCol;
//    FolderButton* child =  static_cast<FolderButton*>(m_gridSizer->GetItem(index)->GetWindow());
    wxWindow* childwindow =  m_gridSizer->GetItem(index)->GetWindow();

    FolderButton* child = wxDynamicCast(childwindow, FolderButton);

    fs::path curfn = fs::path(child->getFileName());
//    auto fstring = child->getItemName();
//    std::cout << fstring << std::endl;

    if (IsDirectory(curfn)) {
        // Rerender
        std::printf("Is a Dir, rerendering: ");
        ReRender(curfn);
    }else{
        std::printf("IS SOMETHING ELSE");
    }

    curDir = curfn;

}

void ScrollWindowNavGrid::HandleEsc() {
    // goes backwards in dir

    if (!curDirStack.empty()){
        auto target_dir = curDirStack.top();
        curDirStack.pop();
        curDir = target_dir;
        ReRender(target_dir);
    }
    // empty do nothing


}



