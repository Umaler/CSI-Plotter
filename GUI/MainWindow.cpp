#include "MainWindow.hpp"

#include <iostream>

#include "../DataSources/DBSource.hpp"
#include "../DataSources/RTSource.hpp"
#include "../DataSources/DBPhaseDiffSource.hpp"

namespace WMG {

TestWindow::TestWindow() :
    db("/home/de/DB/Exp12.01.24.sqlite"),
    dbsource(std::move(db))
{
    set_default_size(100, 100);

    dbsource.signalOnNewDataArrived().connect([](std::vector<std::vector<std::pair<double, double>>> data) {
        for(const auto& i : data) {
            for(const auto& j : i) {
                std::cout << j.first << " " << j.second << std::endl;
            }
            std::cout << "------------------" << std::endl;
        }
        std::cout << "End of pack" << std::endl;
    });
    Boundaries bounds;
    bounds.id.max = 500; bounds.id.min = 0;
    dbsource.setBoundaries(bounds);
    dbsource.addCollectionType("amplitude", "ffa");
}

MainWindow::MainWindow() :
    mainBox(Gtk::Orientation::VERTICAL)
{
    set_title(title);
    set_default_size(width, height);

    set_child(mainBox);

    mainBox.set_margin(5);
    mainBox.set_spacing(5);
    mainBox.set_expand();

    mainBox.append(openDBButton);
    openDBButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenGraphClicked) );

    mainBox.append(openRTButton);
    openRTButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenRTGraphClecked) );

    mainBox.append(openTestButton);
    openTestButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenTestWindow) );

    mainBox.append(openDSPButton);
    openDSPButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenDSPWindow) );

    mainBox.append(openRTDSButton);
    openRTDSButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenRTDSWindow) );

    mainBox.append(openDiffPhaseDB);
    openDiffPhaseDB.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenDBPDWindow) );
}

void MainWindow::onOpenGraphClicked() {
    if(plotWindow)
        return;

    plotWindow.reset(new PlotWindow);
    plotWindow->signal_unmap().connect( [&](){plotWindow.reset();} );
    plotWindow->show();
}

void MainWindow::onOpenRTGraphClecked() {
    if(rtWindow)
        return;

    rtWindow.reset(new RealTimeWindow);
    rtWindow->signal_unmap().connect( [&](){rtWindow.reset();} );
    rtWindow->show();
}

void MainWindow::onOpenTestWindow() {
    if(tWindow)
        return;

    tWindow.reset(new TestWindow);
    tWindow->signal_unmap().connect( [&](){tWindow.reset();} );
    tWindow->show();
}

void MainWindow::onOpenDSPWindow() {
    if(dspWindow)
        return;

    SQLite::Database db("/home/user/Exp12.01.24.sqlite");

    dspWindow.reset(new DataSourcePlotWindow(std::make_unique<DBSource>(std::move(db))));
    dspWindow->signal_unmap().connect( [&](){dspWindow.reset();} );
    dspWindow->set_title("Data base plot");
    dspWindow->show();
}

void MainWindow::onOpenRTDSWindow() {
    if(rtdsWindow)
        return;

    rtdsWindow.reset(new DataSourcePlotWindow(std::make_unique<RTSource>()));
    rtdsWindow->signal_unmap().connect( [&](){rtdsWindow.reset();} );
    rtdsWindow->set_title("Real time plot");
    rtdsWindow->show();
}

void MainWindow::onOpenDBPDWindow() {
    if(dbpdWindow)
        return;

    SQLite::Database db("/home/user/Exp12.01.24.sqlite");

    dbpdWindow.reset(new DataSourcePlotWindow(std::make_unique<DBPhaseDiffSource>(std::move(db))));
    dbpdWindow->signal_unmap().connect( [&](){dbpdWindow.reset();} );
    dbpdWindow->set_title("Data base phase diff plot");
    dbpdWindow->show();
}

}

