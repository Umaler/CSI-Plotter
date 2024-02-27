#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <memory>
#include <vector>
#include <functional>
#include "../DataSource.hpp"
#include "../DataPlotter.hpp"

class PlotWindow : public DataPlotter {
public:
    PlotWindow();

    Glib::ustring getTitle() override;

private:

    struct Selection{
        int idFrom = -1;
        int idTo = -1;
        int id_packetFrom = -1;
        int id_packetTo = -1;
        int id_measurementFrom = -1;
        int id_measurementTo = -1;
        int num_subFrom = -1;
        int num_subTo = -1;
    } selection;

    void setPath(Glib::ustring newPath);

    Glib::ustring dbPath;

    const std::string title = "DB Plot";
    const unsigned int width = 1280, height = 920;

    Gtk::Grid grid;

    void onSourceSelected(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*);

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        ModelColumns()
        { add(sourceName); }

        Gtk::TreeModelColumn<Glib::ustring> sourceName;
        inline static const Glib::ustring sourceColumnName = "Источники данных";

    } columnName;
    Glib::RefPtr<Gtk::TreeStore> sourcesTreeContent;
    Gtk::ScrolledWindow scrollWindow;
    Gtk::TreeView sourcesTree;

    Gtk::PLplot::Canvas canvas;
    std::unique_ptr<Gtk::PLplot::PlotData2D> plotData;
    Gtk::PLplot::Plot2D plot;

    Gtk::Box bottomPanelBox;

};
