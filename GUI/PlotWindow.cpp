#include "PlotWindow.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

PlotWindow::PlotWindow() :
    canvas(),
    plot("X", "Y"),
    bottomPanelBox(Gtk::Orientation::HORIZONTAL)
{
    /// Initialization window
    auto initDialog = new Gtk::FileChooserDialog("Choose a database file", Gtk::FileChooser::Action::OPEN); // Delete only in response lambda

    initDialog->set_transient_for(*this);
    initDialog->set_modal();

    initDialog->signal_response().connect([&, initDialog](int response_id) {
                                                if(response_id == Gtk::ResponseType::OK)
                                                    setPath(initDialog->get_file()->get_path());
                                                delete initDialog;
                                          });

    initDialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    initDialog->add_button("_Open", Gtk::ResponseType::OK);

    initDialog->show();

    /// Main window
    set_title(title);
    set_default_size(width, height);

    set_child(grid);

    sourcesTreeContent = Gtk::TreeStore::create(columnName);
    sourcesTree.set_model(sourcesTreeContent);

    // construct selector of sources

    auto row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "amplitude";
        auto childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fsa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fta";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sta";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsa";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tta";
    row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "clear_phase";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fsph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ftph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "stph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ttph";
    row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "phase";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fsph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ftph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "stph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsph";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ttph";
    row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "measurement";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fsr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ftr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "str";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ttr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ffi";

    sourcesTree.append_column(ModelColumns::sourceColumnName, columnName.sourceName);
    sourcesTree.signal_row_activated().connect( sigc::mem_fun(*this, &PlotWindow::onSourceSelected) );
    scrollWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::ALWAYS);
    scrollWindow.set_child(sourcesTree);
    grid.attach(scrollWindow, 0, 0, 1, 2);

    Gtk::AspectFrame geometry(Gtk::Align::CENTER, Gtk::Align::CENTER, float(width)/float(height));
    geometry.set_child(canvas);
    grid.attach(geometry, 1, 0);
    canvas.set_expand();
    canvas.add_plot(plot);
    plot.hide_legend();



}

Glib::ustring PlotWindow::getTitle() {
    return Glib::ustring("DBPlotter for: ") + dbPath;
}

void PlotWindow::setPath(Glib::ustring newPath) {
    dbPath = newPath;
}

void PlotWindow::onSourceSelected(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*) {
    auto iter = sourcesTreeContent->get_iter(path);
    if(!iter)
        return;
    Glib::ustring field = (*iter)[columnName.sourceName];
    auto upPath = path;
    if(upPath.up())
        iter = sourcesTreeContent->get_iter(upPath);
    if(!iter)
        return;
    Glib::ustring table = (*iter)[columnName.sourceName];

    if(plotData)
        plot.remove_data(*plotData);
    plotData.reset(new Gtk::PLplot::PlotData2D(std::vector<double>{}, std::vector<double>{}));

    SQLite::Database db(dbPath.c_str());
    std::string requestBegin;
        requestBegin += "SELECT " + field + " FROM " + table + " WHERE id_packet = 1";
    SQLite::Statement querry(db, requestBegin);
    int i = 1;
    while(querry.executeStep()) {
        double value = querry.getColumn(0);
        plotData->add_datapoint(i++, value);
    }

    plot.add_data(*plotData);
}

