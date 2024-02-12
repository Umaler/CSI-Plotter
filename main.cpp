#include <iostream>
#include <stdio.h>

#include <sqlite_orm/sqlite_orm.h>
#include <gtkmm.h>

#include "DB/DB.hpp"
#include "GUI/MainWindow.hpp"
#include "DataSource.hpp"

using namespace sqlite_orm;

int main(int argc, char *argv[])
{

    struct amp {
        int id;
        int id_packet;
        int id_measurement;
        int num_sub;
        double ffa;
        double fsa;
        double fta;
        double sfa;
        double ssa;
        double sta;
        double tfa;
        double tsa;
        double tta;
    };

    static auto storage = make_storage("/home/de/DB/Exp12.01.24.sqlite",
                                make_table("amplitude",
                                           make_column("id", &amp::id, primary_key()),
                                           make_column("id_packet", &amp::id_packet),
                                           make_column("id_measurement", &amp::id_measurement),
                                           make_column("num_sub", &amp::num_sub),
                                           make_column("ffa", &amp::ffa),
                                           make_column("fsa", &amp::fsa),
                                           make_column("fta", &amp::fta),
                                           make_column("sfa", &amp::sfa),
                                           make_column("ssa", &amp::ssa),
                                           make_column("sta", &amp::sta),
                                           make_column("tfa", &amp::tfa),
                                           make_column("tsa", &amp::tsa),
                                           make_column("tta", &amp::tta)
                                          )
                                );

    //storage.sync_schema();
    auto a = storage.get_all<amp>(where(c(&amp::id_packet) == 2));
    for(const auto& i : a) {
        std::cout << i.id << std::endl;
    }

    auto app = Gtk::Application::create();

    return app->make_window_and_run<WMG::MainWindow>(argc, argv);
}
