#pragma once

#include <gtkmm.h>

class ChooserLimiter : public Gtk::Grid {
    public:
        ChooserLimiter(Glib::ustring fieldName);

        std::pair<int64_t, int64_t> getLimits() const;

    private:
        Glib::RefPtr<Gtk::Adjustment> bottomBoundAdj;
        Gtk::SpinButton bottomBoundButton;
        Gtk::Frame bottomBoundFrame;

        Glib::RefPtr<Gtk::Adjustment> topBoundAdj;
        Gtk::SpinButton topBoundButton;
        Gtk::Frame topBoundFrame;

        Gtk::CheckButton shouldChooseButton;

    };

class ChoosersPanel : public Gtk::Box {
public:

    ChoosersPanel();

    std::pair<int64_t, int64_t> getIdLimits() const;
    std::pair<int64_t, int64_t> getidPacketLimits() const;
    std::pair<int64_t, int64_t> getIdMeasLimits() const;
    std::pair<int64_t, int64_t> getNumSubLimits() const;


private:
    ChooserLimiter idChooser, idPacketChooser, idMeasChooser, numSubChooser;

};
