#include "MainWindow.hpp"
#include "../DataSourcesRoot.hpp"

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

    mainBox.append(openButton);
    openButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenGraphClicked) );
}

void MainWindow::onOpenGraphClicked() {
    if(plotWindow)
        return;

    plotWindow.reset(new PlotWindow);
    plotWindow->signal_hide().connect( [&](){plotWindow.reset();} );
    plotWindow->signal_unmap().connect( [&](){plotWindow.reset();} );
    plotWindow->show();
}
}

