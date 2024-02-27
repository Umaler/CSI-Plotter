#include <iostream>
#include <stdio.h>

#include <gtkmm.h>

#include "DB/DB.hpp"
#include "GUI/MainWindow.hpp"
#include "DataSource.hpp"

#include "DBPlotter.hpp"

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create();

    return app->make_window_and_run<WMG::MainWindow>(argc, argv);
}
