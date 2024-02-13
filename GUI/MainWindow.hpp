#pragma once

#include <gtkmm.h>

#include <string>
#include "PlotWindow.hpp"

namespace WMG {

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:

    void onOpenFileClicked();
    void onOpenGraphClicked();

    bool onClosePlotWindow();

    const std::string title = "DB plotter";
    const unsigned int width = 800, height = 600;

    Gtk::Box mainBox;

    Gtk::Grid findOpenGrid;
    Gtk::Button findFileButton{"_Open", true};
    Gtk::TextView findFileTextView;
    Gtk::Button openButton{"_Print graph", true};

    Gtk::TextView DB_info;

    std::unique_ptr<PlotWindow> plotWindow;

};

}
