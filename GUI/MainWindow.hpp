#pragma once

#include <gtkmm.h>

#include <string>
#include "PlotWindow.hpp"

namespace WMG {

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:

    void onOpenGraphClicked();

    const std::string title = "DB plotter";
    const unsigned int width = 800, height = 600;

    Gtk::Box mainBox;

    Gtk::Button openButton{"_Print graph", true};

    std::unique_ptr<PlotWindow> plotWindow;

};

}
