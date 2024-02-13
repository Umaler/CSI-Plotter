#include "PlotWindow.hpp"

PlotWindow::PlotWindow(Gtk::Window& w) :
    canvas(),
    plot("X", "Y")
{
    set_transient_for(w);

    set_title(title);
    set_default_size(width, height);

    set_child(canvas);
    canvas.set_expand();
    canvas.add_plot(plot);
    plot.hide_legend();

}

void PlotWindow::setDataSource(std::shared_ptr<DataSource> dataSource) {
    auto data = dataSource->getDataSet();
    if(plotData)
        plot.remove_data(*plotData);
    plotData.reset(new Gtk::PLplot::PlotData2D(data->first, data->second));
    plot.add_data(*plotData);
}

