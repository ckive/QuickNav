//
// Created by Dan on 7/3/23.
//

#include <filesystem>

#include <wx/display.h>

#include "MainFrame.hpp"
#include "ScrollWindowNavGrid.hpp"
#include "HelpDialog.hpp"

MainFrame::MainFrame(const wxString &title, int maxX, int maxY)
        : wxFrame(nullptr, wxID_ANY, title) {

//    // Get the screen size
//    wxDisplay display;
//    wxRect screenRect = display.GetGeometry();
//    int screenWidth = screenRect.GetWidth();
//    int screenHeight = screenRect.GetHeight();



    this->SetMinSize(wxSize(maxX / 2, maxY / 2));
    this->SetClientSize(this->GetMinSize());

    // timer for key press
    dblclick_timer = new wxTimer(this, wxID_ANY);

    // wasd movement
    Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, this);
    Bind(wxEVT_CHAR_HOOK, &MainFrame::OnCharHook, this);

    // bind timer to window
    this->Bind(wxEVT_TIMER, &MainFrame::onTimer, this, wxID_ANY);

    // get cur path
    std::filesystem::path currentPath = std::filesystem::current_path();
    wxString cwd = currentPath.string();

    // status bar
    statusBar = this->CreateStatusBar();
    statusBar->SetFieldsCount(3);
    statusBar->SetStatusText(cwd);
    int widths[] = { -1, -1, -1 }; // Widths for each section (-1 means stretchable)
    statusBar->SetStatusWidths(3, widths);
    statusBar->SetStatusText("wxLog: ", 0);
    statusBar->SetStatusText("BASE Dir:", 1);
    statusBar->SetStatusText("Section 3", 2);

    m_scrollableWindow = new ScrollWindowNavGrid(this);

    m_mainSizer = new wxBoxSizer(wxVERTICAL);
    m_mainSizer->Add(m_scrollableWindow, 10, wxEXPAND);
    m_mainSizer->Add(new wxPanel(this), 1, wxEXPAND); // info panel

    this->SetSizer(m_mainSizer);

}


void MainFrame::focusWindowsPush(wxWindow* window){
    m_focusWindows.push(window);
}
wxWindow* MainFrame::focusWindowsPop(){
    wxWindow* foc_item = m_focusWindows.top();
    m_focusWindows.pop();
    return foc_item;
}


// FileSystem
// open new directory
void MainFrame::DiveIntoDirectory(wxString folderName) {

}



// Keyboard Handling
void MainFrame::OnKeyUp(wxKeyEvent& event) {
    int keyCode = event.GetKeyCode();
//    std::printf("onkeyup: %d\n", keyCode);

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
//            std::printf("keyupdefault: %d\n", keyCode);
            break;
    }
}

void MainFrame::OnCharHook(wxKeyEvent& event) {
    int keyCode = event.GetKeyCode();
//    std::printf("oncharhook: %d\n", keyCode);

    //shitty switch
    wxString ffce;
    wxPanel* replace_p;
    wxWindow* focusedWindow;

    switch (keyCode){
        case 'Q':
            qKeyPressed = true;
            if (qDoublePress) {
                // Quit Application
                wxLogStatus("Double key press detected: Q");
                qDoublePress = false; // Reset double press state
                dblclick_timer->Stop();
                Close();
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
        case 32: // space
            /*
             * TODO
             * get focused window's name
             * curdir + focwinname
             */
            std::printf("SPACE\n");

//            m_focusWindows.push(FindFocus());

            m_scrollableWindow->HandleSpace();

            break;
        case 27: // esc
            /*
             * go back to last focused window. if not displayed, display it unless at base
             */
//            std::printf("ESC\n");
            break;
        default:
//            std::printf("keychardefault\n");
            break;
    }

    // checking combos here
    if (qKeyPressed && hKeyPressed){
        wxLogStatus("Q+H Pressed");
        // Help

        // put cur focused on stack, once modal dies, restore to this object
        auto last_focus = FindFocus();

        HelpDialog helpdialog(this);
        std::printf("in modal\n");
        if (helpdialog.ShowModal() == wxID_CANCEL){
            // After modal dialog is closed, return focus to the main frame or another desired window
            std::printf("modal dead\n");
            last_focus->SetFocus();
        }
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
            m_scrollableWindow->MoveFocusUp();
        } else if (keyCode == 'A' || keyCode == 'a') {
            // 'A' or 'a': Move focus to the left within the grid
            m_scrollableWindow->MoveFocusLeft();
        } else if (keyCode == 'S' || keyCode == 's') {
            // 'S' or 's': Move focus downward within the grid
            m_scrollableWindow->MoveFocusDown();
        } else if (keyCode == 'D' || keyCode == 'd') {
            // 'D' or 'd': Move focus to the right within the grid
            m_scrollableWindow->MoveFocusRight();
        } else {
//            std::printf("oncharhook inwasd notq: %d\n", keyCode);
        }
    }
//    auto fce = FindFocus();
//    auto fcen = fce->GetLabel();
//    statusBar->SetStatusText(fce->GetLabel(),2);

}

void MainFrame::onTimer(wxTimerEvent& event) {
    // Timer expired, log "1 Q press" to status bar
    wxLogStatus("Timer Expired, we here!");
    qDoublePress = false; // Timer expired, reset double press state
}