#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <memory>
#include <functional>
#include "../DataSource.hpp"

class PlotWindow : public Gtk::Window {
public:
    PlotWindow(Gtk::Window& w);

    void setDataSource(std::shared_ptr<DataSource> dataSource);

private:
    const std::string title = "DB Plot";
    const unsigned int width = 1280, height = 920;



    Gtk::PLplot::Canvas canvas;
    std::unique_ptr<Gtk::PLplot::PlotData2D> plotData;
    Gtk::PLplot::Plot2D plot;

};
