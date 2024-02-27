#pragma once
#include <memory>
#include <gtkmm/window.h>
#include <glibmm/ustring.h>

class DataPlotter : public Gtk::Window {
public:

    virtual Glib::ustring getTitle() = 0;   //used in list of plotters

};
