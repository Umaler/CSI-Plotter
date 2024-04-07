#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <atomic>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include "FieldChooser.hpp"
#include "ChoosersPanel.hpp"
#include "../utils/DBDescriptor.hpp"

class RealTimeWindow : public Gtk::ApplicationWindow {
public:

    RealTimeWindow();

private:

    inline static const DBDescriptor rtDescriptor
    {
        {
            "Amplitude",
            {
                "11",
                "12",
                "13",
                "21",
                "22",
                "23",
                "31",
                "32",
                "33"
            }
        },
        {
            "Phase",
            {
                "11",
                "12",
                "13",
                "21",
                "22",
                "23",
                "31",
                "32",
                "33"
            }
        }
    };

    struct DataSlice {
        double amps[3][3];
        double phas[3][3];
        size_t sub_carr_id;
    };

    void onSelectedSource(Glib::ustring, Glib::ustring);

    Gtk::Grid grid;
    FieldChooser fieldChooser;

    Glib::RefPtr<Gtk::Adjustment> subcarrAdj;
    Gtk::SpinButton subcarrSpinButton;
    Gtk::Frame subcarrFrame;

    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::Plot2D plot;
    bool isSmthPlotted = false;

    class PlotWrapper : public Gtk::PLplot::PlotData2D {
    public:

        PlotWrapper() : PlotData2D(std::vector<double>(), std::vector<double>()) {}

    };

    //std::array<PlotWrapper, 144> amps[3][3];
    //std::array<PlotWrapper, 144> phas[3][3];

    void onNewDataArrived();

    std::unique_ptr<std::thread, std::function<void(std::thread*)>> dataCatcherThread;
    std::atomic<bool> toStopCatcher;
    void dataCatcher();

    Glib::Dispatcher newDataCollected;
    std::mutex commonBufferM;
    std::queue<DataSlice> commonBuffer;

};
