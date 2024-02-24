#include "PlotWindow.hpp"

PlotWindow::PlotWindow(Gtk::Window& w) :
    canvas(),
    plot("X", "Y")
{
    set_transient_for(w);

    set_title(title);
    set_default_size(width, height);

    set_child(grid);

    sourcesTreeContent = Gtk::TreeStore::create(columnName);
    sourcesTree.set_model(sourcesTreeContent);

    auto row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "Test source 1";
    row = *(sourcesTreeContent->append());
    row[columnName.sourceName] = "Test source 2";
    sourcesTree.append_column(ModelColumns::sourceColumnName, columnName.sourceName);
    sourcesTree.signal_row_activated().connect( sigc::mem_fun(*this, &PlotWindow::onSourceSelected) );
    grid.attach(sourcesTree, 0, 0, 1, 2);

    grid.attach(canvas, 1, 0);
    canvas.set_expand();
    canvas.add_plot(plot);
    plot.hide_legend();

}

void PlotWindow::onSourceSelected(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*) {
    const auto iter = sourcesTreeContent->get_iter(path);
    if(!iter)
        return;
    const auto& row = *iter;
    auto data = static_cast<std::shared_ptr<DataSource>>(row[columnName.dataSource])->getDataSet();
    if(plotData)
        plot.remove_data(*plotData);
    plotData.reset(new Gtk::PLplot::PlotData2D(data->first, data->second));
    plot.add_data(*plotData);
}

void PlotWindow::setDataSource(std::shared_ptr<DataSource> dataSource) {
    ds = dataSource;
    auto data = ds->getDataSet();
    if(plotData)
        plot.remove_data(*plotData);
    plotData.reset(new Gtk::PLplot::PlotData2D(data->first, data->second));
    plot.add_data(*plotData);
}

