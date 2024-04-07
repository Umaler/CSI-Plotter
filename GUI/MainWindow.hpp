#pragma once

#include <gtkmm.h>

#include <string>
#include "PlotWindow.hpp"
#include "RealTimeWindow.hpp"

namespace WMG {

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:

    void onOpenGraphClicked();
    void onOpenRTGraphClecked();

    const std::string title = "DB plotter";
    const unsigned int width = 400, height = 400;

    Gtk::Box mainBox;

    Gtk::Button openDBButton{"_Print graph by data from database", true};
    Gtk::Button openRTButton{"_Print real time graph", true};

    std::unique_ptr<PlotWindow> plotWindow;
    std::unique_ptr<RealTimeWindow> rtWindow;

};

}
