#pragma once

#include <gtkmm.h>
#include <memory>
#include "../DataSources/DataSource.hpp"
#include "ExtendablePlot.hpp"
#include "FieldChooser.hpp"
#include "ChoosersPanel.hpp"

class DataSourcePlotWindow : public Gtk::ApplicationWindow {
public:

    DataSourcePlotWindow(std::unique_ptr<DataSource> ds);

private:
    void onDataArrived(std::vector<std::vector<std::pair<double, double>>> data);

    Gtk::Grid mainGrid;

    std::unique_ptr<DataSource> source;
    ExtendablePlot ep;
    DataSet plotDataSet;
    FieldChooser fieldChooser;
    ChoosersPanel choosersPanel;


};