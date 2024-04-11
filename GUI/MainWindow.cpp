#include "MainWindow.hpp"

#include <iostream>

#include "../DataSources/DBSource.hpp"
#include "../DataSources/RTSource.hpp"
#include "../DataSources/DBPhaseDiffSource.hpp"

namespace WMG {

MainWindow::MainWindow() :
    mainBox(Gtk::Orientation::VERTICAL)
{
    set_title(title);
    set_default_size(width, height);

    set_child(mainBox);

    mainBox.set_margin(5);
    mainBox.set_spacing(5);
    mainBox.set_expand();

    mainBox.append(openDSPButton);
    openDSPButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenDSPWindow) );

    mainBox.append(openRTDSButton);
    openRTDSButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenRTDSWindow) );

    mainBox.append(openDiffPhaseDB);
    openDiffPhaseDB.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenDBPDWindow) );
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

