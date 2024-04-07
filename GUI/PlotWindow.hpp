#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <memory>
#include <utility>
#include <vector>
#include <mutex>
#include <queue>
#include <array>
#include <thread>
#include <functional>
#include <SQLiteCpp/SQLiteCpp.h>

#include "FieldChooser.hpp"
#include "ChoosersPanel.hpp"

class PlotWindow : public Gtk::ApplicationWindow {
public:
    PlotWindow();

    Glib::ustring getTitle();

private:

    typedef int64_t DB_LIMITS_T;

    struct Selection {
        DB_LIMITS_T idFrom = -1;
        DB_LIMITS_T idTo = -1;
        DB_LIMITS_T id_packetFrom = -1;
        DB_LIMITS_T id_packetTo = -1;
        DB_LIMITS_T id_measurementFrom = -1;
        DB_LIMITS_T id_measurementTo = -1;
        DB_LIMITS_T num_subFrom = -1;
        DB_LIMITS_T num_subTo = -1;
    } selection;

    void setPath(Glib::ustring newPath);

    Glib::ustring dbPath;

    const std::string title = "DB Plot";
    const unsigned int width = 1280, height = 920;

    Gtk::Grid grid;

    void onSourceSelected(Glib::ustring table, Glib::ustring field);

    FieldChooser fieldChooser;
    ChoosersPanel choosersPanel;

    Gtk::PLplot::Canvas canvas;
    std::unique_ptr<Gtk::PLplot::PlotData2D> plotData;
    Gtk::PLplot::Plot2D plot;

    void onNewDataArrived();

    std::unique_ptr<std::thread, std::function<void(std::thread*)>> dataCatcherThread;
    std::atomic<bool> toStopCatcher;
    void dataCatcher(Glib::ustring table, Glib::ustring field);

    Glib::Dispatcher newDataCollected;
    std::mutex commonBufferM;
    std::queue<std::pair<size_t, double>> commonBuffer;

};
