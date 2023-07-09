//
// Created by Dan on 7/8/23.
//

#include "ScrollWindowNavGrid.hpp"

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <vector>
#include <wx/artprov.h>

#include "FolderPanel.hpp"

ScrollWindowNavGrid::ScrollWindowNavGrid(wxWindow* parent)
        : wxScrolledWindow(parent, wxID_ANY),
          m_currentRow(0),
          m_currentCol(0),
          m_numCols(NUM_COLS),
          m_parent(parent)
{
    // timer for key press
    dblclick_timer = new wxTimer(this, wxID_ANY);

    // bind timer to window
    this->Bind(wxEVT_TIMER, &ScrollWindowNavGrid::onTimer, this, wxID_ANY);


//    this->SetBackgroundColour(wxColour(0, 150, 0));
    std::printf("numRows: %d        numCols: %d\n", m_numCols, m_numRows);
    // Create a box sizer to hold the icons
    wxBoxSizer* scrollableSizer = new wxBoxSizer(wxVERTICAL);

    // wasd movement
    Bind(wxEVT_KEY_UP, &ScrollWindowNavGrid::OnKeyUp, this);
    Bind(wxEVT_CHAR_HOOK, &ScrollWindowNavGrid::OnCharHook, this);


    // base dir
    // wxString directoryPath = "/Users/dan/Desktop/uwcold/vsco";
    wxString directoryPath = "/Users/dan/Desktop/uwcold/instaloader";
    wxArrayString subfolderNames;

    wxDir dir(directoryPath);
    if (!dir.IsOpened())
    {
        wxLogError("Failed to open directory: %s", directoryPath);
        return;
    }

    wxString subfolder;
    bool cont = dir.GetFirst(&subfolder, wxEmptyString, wxDIR_DIRS);
    while (cont)
    {
        // Exclude "." and ".." entries
        if (subfolder != wxT(".") && subfolder != wxT(".."))
        {
            subfolderNames.Add(subfolder);
        }
        cont = dir.GetNext(&subfolder);
    }
    // sort alphabetically
    subfolderNames.Sort();

    // set number of rows now
    m_curDirItemCount = (int)subfolderNames.GetCount();
    m_numRows = m_curDirItemCount / m_numCols;
    if (subfolderNames.GetCount()%m_numCols != 0)
        m_numRows++;

    // instead of from DIP, get from current client size or something.
    const int WIDTH = FromDIP(200);
    const int HEIGHT = FromDIP(200);

    m_gridSizer = new wxGridSizer(m_numCols);

    wxBitmap folderIcon = wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(WIDTH, HEIGHT));
    wxBitmapBundle folderIconBundle = wxArtProvider::GetBitmapBundle(wxART_FOLDER, wxART_OTHER, wxSize(WIDTH, HEIGHT));

    // Load and add images to the grid sizer
    for (const wxString& sf : subfolderNames)
    {
        auto folderPanel = new FolderPanel(this, sf, &folderIcon);
        folderPanel->Bind(wxEVT_KEY_UP, &ScrollWindowNavGrid::OnKeyUp, this);
        m_gridSizer->Add(folderPanel, 0, wxALIGN_CENTER | wxALL, 10);
//        wxButton* folderButton = new wxButton(this, wxID_ANY,sf);
//        folderButton->SetBitmap(folderIconBundle, wxTOP);
//        folderButton->Bind(wxEVT_KEY_UP, &ScrollWindowNavGrid::OnKeyUp, this);
//        m_gridSizer->Add(folderButton, 0, wxALIGN_CENTER | wxALL, 10);
    }

    scrollableSizer->Add(m_gridSizer, 0, wxEXPAND);
    SetSizer(scrollableSizer);

    SetScrollRate(10, 10);
    this->EnableScrolling(true, true);
}


void ScrollWindowNavGrid::onTimer(wxTimerEvent& event) {
    // Timer expired, log "1 Q press" to status bar
    wxLogStatus("Timer Expired, we here!");
    qDoublePress = false; // Timer expired, reset double press state
}

HelpDialog::HelpDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Shortcuts Help") {

    const wxString text = "The following shows the supported shortcuts of this app:\n"
                          "Q-H - help\n"
                          "Q-R - random from base\n"
                          "Q-S - shuffle cur order\n"
                          "Q-B - tag new base\n"
                          "Q-F - flatten\n"
                          "Q-G - gallery\n"
                          "Q-P - to path\n"
                          "Q-E - cursor to end\n"
                          "Q-T - terminal\n"
                          "Q-Q - quit\n"
                          "Q-BB - untag base\n"
                          "Q-FF - unflatten\n"
                          "Thank you for using QuickNav";

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, text);
    sizer->Add(staticText, wxSizerFlags().Center().Border(wxALL, 10));

    SetSizerAndFit(sizer);
}


void ScrollWindowNavGrid::OnKeyUp(wxKeyEvent& event) {
    int keyCode = event.GetKeyCode();
    std::printf("onkeyup: %d\n", keyCode);

    switch (keyCode){
        case 'Q':
            qKeyPressed = false;
            break;
        case 'H':
            hKeyPressed = false;
            break;
        case 'R':
            rKeyPressed = false;
            break;
        case 'S':
            sKeyPressed = false;
            break;
        case 'B':
            bKeyPressed = false;
            break;
        case 'F':
            fKeyPressed = false;
            break;
        case 'G':
            gKeyPressed = false;
            break;
        case 'P':
            pKeyPressed = false;
            break;
        case 'E':
            eKeyPressed = false;
            break;
        case 'T':
            tKeyPressed = false;
            break;
        default:
            std::printf("keyupdefault: %d\n", keyCode);
            break;
    }
}

void ScrollWindowNavGrid::OnCharHook(wxKeyEvent& event) {
    int keyCode = event.GetKeyCode();
    std::printf("oncharhook: %d\n", keyCode);

    switch (keyCode){
        case 'Q':
            qKeyPressed = true;
            if (qDoublePress) {
                // Quit Application
                wxLogStatus("Double key press detected: Q");
                qDoublePress = false; // Reset double press state
                dblclick_timer->Stop();
                m_parent->Close();
            } else if (bDoublePress){
                // Unset Base Dir
                wxLogStatus("Double key press detected: B");
                bDoublePress = false; // Reset double press state
                dblclick_timer->Stop();
            } else if (fDoublePress){
                // Unflatten
                wxLogStatus("Double key press detected: F");
                fDoublePress = false; // Reset double press state
                dblclick_timer->Stop();
            } else {
                wxLogStatus("Single key press detected: Q");
                qDoublePress = true; // Set double press state
                dblclick_timer->Start(400, wxTIMER_ONE_SHOT); // Restart the timer
            }
            break;
        case 'H':
            hKeyPressed = true;
            break;
        case 'R':
            rKeyPressed = true;
            break;
        case 'S':
            sKeyPressed = true;
            break;
        case 'B':
            bDoublePress = true; // kinda broken
            dblclick_timer->Start(400, wxTIMER_ONE_SHOT);
            bKeyPressed = true;
            break;
        case 'F':
            fDoublePress = true;
            dblclick_timer->Start(400, wxTIMER_ONE_SHOT);
            fKeyPressed = true;
            break;
        case 'G':
            gKeyPressed = true;
            break;
        case 'P':
            pKeyPressed = true;
            break;
        case 'E':
            eKeyPressed = true;
            break;
        case 'T':
            tKeyPressed = true;
            break;
        default:
            std::printf("keychardefault\n");
//            std::printf("keycode(mf): %d    qpressed: %s\n", keyCode, qKeyPressed ? "true" : "false");
            break;
    }

    // checking combos here
    if (qKeyPressed && hKeyPressed){
        wxLogStatus("Q+H Pressed");
        // Help

        HelpDialog helpdialog(this);
        helpdialog.ShowModal();
        hKeyPressed = false;
    } else if (qKeyPressed && rKeyPressed) {
        wxLogStatus("Q+R Pressed");
    } else if (qKeyPressed && sKeyPressed) {
        wxLogStatus("Q+S Pressed");
    } else if (qKeyPressed && bKeyPressed) {
        wxLogStatus("Q+B Pressed");
    } else if (qKeyPressed && fKeyPressed) {
        wxLogStatus("Q+F Pressed");
    } else if (qKeyPressed && gKeyPressed) {
        wxLogStatus("Q+G Pressed");
    } else if (qKeyPressed && pKeyPressed) {
        wxLogStatus("Q+P Pressed");
    } else if (qKeyPressed && eKeyPressed) {
        wxLogStatus("Q+E Pressed");
    } else if (qKeyPressed && tKeyPressed) {
        wxLogStatus("Q+T Pressed");
    }

    if (!qKeyPressed){
        if (keyCode == 'W' || keyCode == 'w') {
            // 'W' or 'w': Move focus upward within the grid
            MoveFocusUp();
        } else if (keyCode == 'A' || keyCode == 'a') {
            // 'A' or 'a': Move focus to the left within the grid
            MoveFocusLeft();
        } else if (keyCode == 'S' || keyCode == 's') {
            // 'S' or 's': Move focus downward within the grid
            MoveFocusDown();
        } else if (keyCode == 'D' || keyCode == 'd') {
            // 'D' or 'd': Move focus to the right within the grid
            MoveFocusRight();
        } else {
            std::printf("oncharhook inwasd notq: %d\n", keyCode);
        }
    }

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