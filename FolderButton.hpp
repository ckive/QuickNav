//
// Created by Dan on 7/9/23.
//

#ifndef QUICKNAV_FOLDERBUTTON_HPP
#define QUICKNAV_FOLDERBUTTON_HPP

#include <wx/wx.h>

class FolderButton : public wxButton
{
public:
    FolderButton(ScrollWindowNavGrid* parent, const wxBitmapBundle& iconBundle, wxString label, wxSize size,
                 wxFileName fn)
            : wxButton(parent, wxID_ANY, label, wxDefaultPosition, size), m_label(label), m_fn(fn), m_parent(parent)
    {

        SetBitmap(iconBundle, wxTOP);

        // Bind keyboard events to event handlers in the custom button class
//        Bind(wxEVT_KEY_DOWN, &FolderButton::handleSpacePress, this);
//        Bind(wxEVT_KEY_UP, &FolderButton::OnKeyUp, this);
    }

    wxFileName& getFileName(){
        std::cout << m_fn.GetFullPath() << std::endl;
        return m_fn;
    }

    const wxString& getItemName(){
        return m_label;
    }



private:
    ScrollWindowNavGrid *m_parent;
    wxFileName m_fn;
    wxString m_label;
//    void handleSpacePress(wxKeyEvent& event)
//    {
//        std::printf("FB Space handling\n");
//        int keyCode = event.GetKeyCode();
//        if (keyCode == 32){
//            // space bar is pressed
//            m_parent->DiveIntoDirectory(fn);
//        }else{
//            event.Skip(); // Allow event propagation
//        }
//        m_parent->Hide();
//    }

//    void OnKeyUp(wxKeyEvent& event)
//    {
//        // Handle key up event for the custom button
//        // ...
//
//        event.Skip(); // Allow event propagation
//    }

};

#endif //QUICKNAV_FOLDERBUTTON_HPP
