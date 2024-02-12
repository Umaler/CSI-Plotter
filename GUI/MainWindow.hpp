#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <string>

namespace WMG {

class MainWindow : public Gtk::Window {
public:
    MainWindow();

private:
    const std::string title = "Basic application";
    const unsigned int width = 1024, height = 576;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        ModelColumns() {
            add(m_col_id);
            add(m_col_name);
        }

        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };

    ModelColumns m_Columns;

    Gtk::TreeView treeView;
    Glib::RefPtr<Gtk::TreeStore> ref_treeStore;

    Gtk::Grid grid;

    sigc::connection timeout_connect;

    bool on_timeout(int timer_number);

    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::Plot2D plot;
    std::vector<double> x, y;
    Gtk::PLplot::PlotData2D plotData;

};

}
