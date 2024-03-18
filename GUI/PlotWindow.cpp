#include "PlotWindow.hpp"

#include <future>

PlotWindow::PlotWindow() :
    plot("ID", "Value"),
    canvas(),
    bottomPanelBox(Gtk::Orientation::HORIZONTAL),
    idChooser("id"),
    idPacketChooser("id пакета"),
    idMeasChooser("id измерения"),
    numSubChooser("номер поднесущей")
{
    /// Initialization window
    Gtk::FileChooserDialog initDialog(*this, "Choose a database file", Gtk::FileChooser::Action::OPEN, true);
    bool initDialogWorkedCorrectly = false;

    initDialog.set_modal();

    initDialog.signal_response().connect([&](int response_id) {
                                                if(response_id == Gtk::ResponseType::OK) {
                                                    setPath(initDialog.get_file()->get_path());
                                                    initDialogWorkedCorrectly = true;
                                                }
                                                else {
                                                    initDialogWorkedCorrectly = false;
                                                }
                                          });
    initDialog.signal_close_request().connect([&]() {
                                                  initDialog.hide();
                                                  return true;
                                              },
                                              false);

    initDialog.add_button("_Cancel", Gtk::ResponseType::CANCEL);
    initDialog.add_button("_Open", Gtk::ResponseType::OK);

    while(!initDialogWorkedCorrectly) {  //wait until initDialog worked
        initDialog.show();
        Glib::MainContext::get_default()->iteration(true);  //iterate main loop
    }

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
        childrow[columnName.sourceName] = "fsi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ftr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "fti";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sfi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ssi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "str";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "sti";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tfi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tsi";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "ttr";
        childrow = *(sourcesTreeContent->append(row.children()));
        childrow[columnName.sourceName] = "tti";

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

    bottomPanelBox.append(idChooser);
    bottomPanelBox.append(idPacketChooser);
    bottomPanelBox.append(idMeasChooser);
    bottomPanelBox.append(numSubChooser);
    grid.attach(bottomPanelBox, 1, 1);
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
        requestBegin += "SELECT id, " + field + " FROM " + table + " WHERE id >= ? AND id <= ? AND id_packet >= ? AND id_packet <= ? AND id_measurement >= ? AND id_measurement <= ? AND num_sub >= ? AND num_sub <= ?";
    SQLite::Statement querry(db, requestBegin);
    auto idBounds = idChooser.getLimits();
    querry.bind(1, idBounds.first); querry.bind(2, idBounds.second);
    auto idPackBounds = idPacketChooser.getLimits();
    querry.bind(3, idPackBounds.first); querry.bind(4, idPackBounds.second);
    auto idMeasBounds = idMeasChooser.getLimits();
    querry.bind(5, idMeasBounds.first); querry.bind(6, idMeasBounds.second);
    auto numSubBounds = numSubChooser.getLimits();
    querry.bind(7, numSubBounds.first); querry.bind(8, numSubBounds.second);

    while(querry.executeStep()) {
        int id = querry.getColumn(0);
        double value = querry.getColumn(1);
        plotData->add_datapoint(id, value);
    }

    plot.add_data(*plotData);
}

PlotWindow::ChooserLimiter::ChooserLimiter(Glib::ustring fieldName) :
    bottomBoundAdj(Gtk::Adjustment::create(0, 0, std::numeric_limits<DB_LIMITS_T>::max(), 1, 1)),
    bottomBoundButton(bottomBoundAdj),
    bottomBoundFrame("Нижняя граница"),

    topBoundAdj(Gtk::Adjustment::create(0, 0, std::numeric_limits<DB_LIMITS_T>::max(), 1, 1)),
    topBoundButton(topBoundAdj),
    topBoundFrame("Верхняя граница"),

    shouldChooseButton(Glib::ustring("Ограничить ") + fieldName + "?")
{
    bottomBoundFrame.set_child(bottomBoundButton);
    attach(bottomBoundFrame, 0, 0);

    topBoundFrame.set_child(topBoundButton);
    attach(topBoundFrame, 1, 0);

    attach(shouldChooseButton, 0, 1, 2);
}

std::pair<PlotWindow::DB_LIMITS_T, PlotWindow::DB_LIMITS_T> PlotWindow::ChooserLimiter::getLimits() {
    if(!shouldChooseButton.get_active())
        return {0, std::numeric_limits<PlotWindow::DB_LIMITS_T>::max()};
    return {bottomBoundButton.get_value(), topBoundButton.get_value()};
}
