#include "MainWindow.hpp"

namespace WMG {

MainWindow::MainWindow() :
    canvas(),
    plot("X", "Y"),
    x(5),
    y(5),
    plotData(x, y)
{
    set_title(title);
    set_default_size(width, height);

    canvas.set_hexpand(true);
    canvas.set_vexpand(true);

    plot.hide_legend();

    canvas.add_plot(plot);

    grid.attach(canvas, 1, 0, 1, 2);

    set_child(grid);

    std::generate(x.begin(), x.end(), [](){static int i = 1; return i++;});
    std::generate(y.begin(), y.end(), [](){static int i = 1; return i++;});

    plotData.

    plot.add_data(plotData);

    timeout_connect = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_timeout), 5), 5);

}

bool MainWindow::on_timeout(int timer_number) {
    x.push_back(x.back() + 0.2);
    y.push_back(y.back() * 0.999);

    static bool b = false;
    std::cout << "A" << b;
    b = !b;

    return true;
}

}

