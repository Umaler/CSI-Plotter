#include "MainWindow.hpp"
#include "../TestDataSource.hpp"
#include "../TestDBDataSource.hpp"

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

    mainBox.append(findOpenGrid);

    findOpenGrid.attach(findFileTextView, 0, 0);
    findFileTextView.set_hexpand();

    findOpenGrid.attach(findFileButton, 1, 0);
    findFileButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenFileClicked) );

    findOpenGrid.attach(openButton, 1, 1);
    openButton.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::onOpenGraphClicked) );

    DB_info.set_expand();
    DB_info.set_editable(false);
    mainBox.append(DB_info);

}

void MainWindow::onOpenFileClicked() {
    auto dialog = new Gtk::FileChooserDialog("Please choose a file", Gtk::FileChooser::Action::OPEN);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect([&, dialog](int response_id){
                                            if(response_id != Gtk::ResponseType::OK) {
                                                delete dialog;
                                                return;
                                            }
                                            auto filename = dialog->get_file()->get_path();
                                            findFileTextView.get_buffer()->set_text(filename);
                                            delete dialog;
                                      });
    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);
    dialog->show();
}

void MainWindow::onOpenGraphClicked() {
    if(plotWindow)
        return;

    plotWindow.reset(new PlotWindow(*this));
    plotWindow->setDataSource(std::make_shared<TestDBDataSource>(findFileTextView.get_buffer()->get_text()));
    plotWindow->signal_hide().connect( [&](){plotWindow.reset();} );
    plotWindow->signal_unmap().connect( [&](){plotWindow.reset();} );
    plotWindow->show();
}

bool MainWindow::onClosePlotWindow() {
    plotWindow.reset();
}

}

