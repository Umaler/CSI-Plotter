#pragma once

#include <gtkmm.h>

#include <string>
#include "DataSourcePlotWindow.hpp"

namespace WMG {

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:

    void onOpenDSPWindow();
    void onOpenRTDSWindow();
    void onOpenDBPDWindow();

    const std::string title = "DB plotter";
    const unsigned int width = 400, height = 400;

    Gtk::Box mainBox;

    Gtk::Button openDSPButton{"_Open Data Source Plot Window", true};
    Gtk::Button openRTDSButton{"_Open Real Time DS Window", true};
    Gtk::Button openDiffPhaseDB{"_Open from DB with phase diff", true};

    std::unique_ptr<DataSourcePlotWindow> dspWindow;
    std::unique_ptr<DataSourcePlotWindow> rtdsWindow;
    std::unique_ptr<DataSourcePlotWindow> dbpdWindow;

};

}
