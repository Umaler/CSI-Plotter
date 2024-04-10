#pragma once

#include <gtkmm.h>

#include <string>
#include "PlotWindow.hpp"
#include "RealTimeWindow.hpp"
#include "DataSourcePlotWindow.hpp"
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
    void onOpenDSPWindow();
    void onOpenRTDSWindow();

    const std::string title = "DB plotter";
    const unsigned int width = 400, height = 400;

    Gtk::Box mainBox;

    Gtk::Button openDBButton{"_Print graph by data from database", true};
    Gtk::Button openRTButton{"_Print real time graph", true};
    Gtk::Button openTestButton{"_Test window", true};
    Gtk::Button openDSPButton{"_Open Data Source Plot Window", true};
    Gtk::Button openRTDSButton{"_Open Real Time DS Window", true};

    std::unique_ptr<PlotWindow> plotWindow;
    std::unique_ptr<RealTimeWindow> rtWindow;
    std::unique_ptr<TestWindow> tWindow;
    std::unique_ptr<DataSourcePlotWindow> dspWindow;
    std::unique_ptr<DataSourcePlotWindow> rtdsWindow;

};

}
