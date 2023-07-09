//
// Created by Dan on 7/7/23.
//

// The Panel for displaying a folder with name of folder in a grid. Handles focus highlighting

#ifndef QUICKNAV_FOLDERPANEL_HPP
#define QUICKNAV_FOLDERPANEL_HPP

#include <wx/wx.h>
#include <wx/artprov.h>

#include "ScrollWindowNavGrid.hpp"

class FolderPanel : public wxPanel {
public:
    FolderPanel(wxWindow* parent, wxString sfn, wxBitmap* folderIcon) : wxPanel(parent) {
        // Set initial focus state
        m_hasFocus = false;

        wxBoxSizer* folderSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticBitmap* folderIconBitmap = new wxStaticBitmap(this, wxID_ANY, *folderIcon);
        wxStaticText* text = new wxStaticText(this, wxID_ANY, sfn);

        folderSizer->Add(folderIconBitmap, 0, wxALIGN_CENTER | wxALL, 10);
        folderSizer->Add(text, 0, wxALIGN_CENTER | wxALL, 5);

        // Bind focus events
        Bind(wxEVT_SET_FOCUS, &FolderPanel::OnSetFocus, this);
        Bind(wxEVT_KILL_FOCUS, &FolderPanel::OnKillFocus, this);
        Bind(wxEVT_PAINT, &FolderPanel::OnPaint, this);

    }

private:
    bool m_hasFocus;

    void OnPaint(wxPaintEvent& event) {
        wxPaintDC dc(this);

        // Draw the panel contents

        // Draw border if panel has focus
        if (m_hasFocus) {
            wxPen borderPen(wxColour(0, 0, 255), 2); // Blue border pen
            dc.SetPen(borderPen);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(wxPoint(0, 0), GetSize());
        }
    }

    void OnSetFocus(wxFocusEvent& event) {
        m_hasFocus = true;
        Refresh(); // Request repainting to update the border
//        event.Skip();
    }

    void OnKillFocus(wxFocusEvent& event) {
        m_hasFocus = false;
        Refresh(); // Request repainting to remove the border
//        event.Skip();
    }


};


#endif //QUICKNAV_FOLDERPANEL_HPP
