//
// Created by Dan on 7/9/23.
//

#ifndef QUICKNAV_HELPDIALOG_HPP
#define QUICKNAV_HELPDIALOG_HPP

#include <wx/wx.h>

class HelpDialog : public wxDialog {
public:
    HelpDialog(wxWindow* parent): wxDialog(parent, wxID_ANY, "Shortcuts Help") {

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
    };
};

#endif //QUICKNAV_HELPDIALOG_HPP
