#include "MainWindow.hpp"

#include <iostream>

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

    mainBox.append(openDBButton);
    openDBButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenGraphClicked) );

    mainBox.append(openRTButton);
    openRTButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenRTGraphClecked) );
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

}

