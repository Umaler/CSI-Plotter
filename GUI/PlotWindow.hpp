#pragma once

#include <gtkmm.h>
#include <gtkmm-plplot.h>

#include <memory>
#include <utility>
#include <vector>
#include <functional>

class PlotWindow : public Gtk::ApplicationWindow {
public:
    PlotWindow();

    Glib::ustring getTitle();

private:

    typedef int64_t DB_LIMITS_T;

    struct Selection{
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
    class ChooserLimiter : public Gtk::Grid {
    public:
        ChooserLimiter(Glib::ustring fieldName);

        std::pair<DB_LIMITS_T, DB_LIMITS_T> getLimits();

    private:
        Glib::RefPtr<Gtk::Adjustment> bottomBoundAdj;
        Gtk::SpinButton bottomBoundButton;
        Gtk::Frame bottomBoundFrame;

        Glib::RefPtr<Gtk::Adjustment> topBoundAdj;
        Gtk::SpinButton topBoundButton;
        Gtk::Frame topBoundFrame;

        Gtk::CheckButton shouldChooseButton;

    } idChooser, idPacketChooser, idMeasChooser, numSubChooser;

};
