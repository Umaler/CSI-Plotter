#include <iostream>
#include <stdio.h>

#include <gtkmm.h>

#include "GUI/MainWindow.hpp"

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create();

    return app->make_window_and_run<WMG::MainWindow>(argc, argv);
}
