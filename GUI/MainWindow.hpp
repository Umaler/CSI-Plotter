#pragma once

#include <gtkmm.h>

#include <string>
#include "PlotWindow.hpp"
#include "RealTimeWindow.hpp"
#include "../DataSources/DBSource.hpp"

namespace WMG {

class TestWindow : public Gtk::ApplicationWindow {
public:
    TestWindow();

private:
    SQLite::Database db;
    DBSource dbsource;

};

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:

    void onOpenGraphClicked();
    void onOpenRTGraphClecked();
    void onOpenTestWindow();

    const std::string title = "DB plotter";
    const unsigned int width = 400, height = 400;

    Gtk::Box mainBox;

    Gtk::Button openDBButton{"_Print graph by data from database", true};
    Gtk::Button openRTButton{"_Print real time graph", true};
    Gtk::Button openTestButton{"_Test window", true};

    std::unique_ptr<PlotWindow> plotWindow;
    std::unique_ptr<RealTimeWindow> rtWindow;
    std::unique_ptr<TestWindow> tWindow;

};

}
