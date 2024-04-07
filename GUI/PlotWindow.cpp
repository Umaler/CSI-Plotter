#include "PlotWindow.hpp"

#include <future>
#include <chrono>
#include <exception>
#include <fstream>
#include "../utils/DBDescriptor.hpp"

PlotWindow::PlotWindow() :
    plot("ID", "Value"),
    canvas(),
    fieldChooser(dbDescriptor),

    toStopCatcher(false),
    dataCatcherThread(nullptr,
                      [&] (std::thread* t) -> void {
                          toStopCatcher.store(true);
                          t->join();
                          delete t;
                      }
                     )
{
    /// Initialization window
    Gtk::FileChooserDialog initDialog(*this, "Choose a database file", Gtk::FileChooser::Action::OPEN, true);
    bool initDialogWorkedCorrectly = false;

    initDialog.set_modal();

    initDialog.signal_response().connect([&](int response_id) {
                                                switch(response_id) {
                                                    case Gtk::ResponseType::OK:
                                                        setPath(initDialog.get_file()->get_path());
                                                        initDialogWorkedCorrectly = true;
                                                        break;

                                                    default:
                                                        initDialogWorkedCorrectly = false;
                                                        break;
                                                }
                                          });
    initDialog.signal_close_request().connect([&]() {
                                                  initDialog.hide();
                                                  return true;
                                              },
                                              false);

    initDialog.add_button("Open", Gtk::ResponseType::OK);
    initDialog.add_button("Cancel", Gtk::ResponseType::CANCEL);

    bool correctDB = false;
    while(!correctDB) {
        while(!initDialogWorkedCorrectly) {  //wait until initDialog worked
            initDialog.show();
            Glib::MainContext::get_default()->iteration(true);  //main loop iteration
        }
        try {
            SQLite::Database db(dbPath, SQLite::OPEN_READONLY);
            if(dbDescriptor.checkCompliance(db))
                correctDB = true;
            else
                initDialogWorkedCorrectly = false;
        }
        catch(const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            initDialogWorkedCorrectly = false;
            continue;
        }
    }

    /// Main window
    set_title(title);
    set_default_size(width, height);

    set_child(grid);

    fieldChooser.signalOnChoosed().connect(sigc::mem_fun(*this, &PlotWindow::onSourceSelected));
    grid.attach(fieldChooser, 0, 0, 1, 2);

    Gtk::AspectFrame geometry(Gtk::Align::CENTER, Gtk::Align::CENTER, float(width)/float(height));
    geometry.set_child(canvas);
    grid.attach(geometry, 1, 0);
    canvas.set_expand();
    canvas.add_plot(plot);
    plot.hide_legend();

    grid.attach(choosersPanel, 1, 1);

    newDataCollected.connect(sigc::mem_fun(*this, &PlotWindow::onNewDataArrived));
}

Glib::ustring PlotWindow::getTitle() {
    return Glib::ustring("DBPlotter for: ") + dbPath;
}

void PlotWindow::setPath(Glib::ustring newPath) {
    dbPath = newPath;
}

void PlotWindow::onNewDataArrived() {
    const auto start{std::chrono::steady_clock::now()};
    if(!plotData)
        return;

    commonBufferM.lock();
    while(!commonBuffer.empty()) {
        auto val = commonBuffer.front();
        commonBuffer.pop();

        plotData->add_datapoint(val.first, val.second);
    }
    commonBufferM.unlock();
    const auto end{std::chrono::steady_clock::now()};
    const auto elapsed_seconds{end - start};
    static std::ofstream logs("elapsed_seconds.log", std::ios_base::trunc);
    logs << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_seconds).count() << std::endl;
}

void PlotWindow::dataCatcher(Glib::ustring table, Glib::ustring field) {
    std::queue<std::pair<size_t, double>> localBuffer;
    const size_t bufSize = 10;

    SQLite::Database db(dbPath.c_str());
    std::string requestBegin;
        requestBegin += "SELECT id, " + field + " FROM " + table + " WHERE id >= ? AND id <= ? AND id_packet >= ? AND id_packet <= ? AND id_measurement >= ? AND id_measurement <= ? AND num_sub >= ? AND num_sub <= ?";
    SQLite::Statement querry(db, requestBegin);
    auto idBounds = choosersPanel.getIdLimits();
    querry.bind(1, idBounds.first); querry.bind(2, idBounds.second);
    auto idPackBounds = choosersPanel.getidPacketLimits();
    querry.bind(3, idPackBounds.first); querry.bind(4, idPackBounds.second);
    auto idMeasBounds = choosersPanel.getIdMeasLimits();
    querry.bind(5, idMeasBounds.first); querry.bind(6, idMeasBounds.second);
    auto numSubBounds = choosersPanel.getNumSubLimits();
    querry.bind(7, numSubBounds.first); querry.bind(8, numSubBounds.second);

    while (!toStopCatcher.load()) {
        try {
            for(size_t i = 0; i < bufSize; i++) {
                if(!querry.executeStep()) {
                    toStopCatcher.store(true);
                    break;
                }
                unsigned long long id = querry.getColumn(0).getInt64();
                double value = querry.getColumn(1);
                localBuffer.push({id, value});
            }
        }
        catch(...) {
            std::cerr << querry.getErrorMsg() << std::endl;
            toStopCatcher.store(true);
        }

        bool commonBufferFree = false;
        while(!commonBufferFree && !toStopCatcher.load()) {
            commonBufferM.lock();
            commonBufferFree = commonBuffer.empty();
            if(commonBufferFree)
                std::swap(commonBuffer, localBuffer);
            commonBufferM.unlock();
        }
        newDataCollected.emit();
    }
}

void PlotWindow::onSourceSelected(Glib::ustring table, Glib::ustring field) {
    if(plotData)
        plot.remove_data(*plotData);
    plotData.reset(new Gtk::PLplot::PlotData2D(std::vector<double>{}, std::vector<double>{}));
    plot.add_data(*plotData);

    dataCatcherThread.reset();
    toStopCatcher.store(false);
    dataCatcherThread.reset(new std::thread(&PlotWindow::dataCatcher, this, table, field));
}
