#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <memory>
#include <functional>

class RealTimeWindow : public Gtk::ApplicationWindow {
public:

    RealTimeWindow();

private:

    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::PlotData2D plotData;
    Gtk::PLplot::Plot2D plot;

    void onNewDataArrived();

    std::unique_ptr<std::thread, std::function<void(std::thread*)>> dataCatcherThread;
    std::atomic<bool> toStopCatcher;
    void dataCatcher();

    Glib::Dispatcher newDataCollected;
    std::mutex commonBufferM;
    std::queue<double> commonBuffer;

};
