#include "DataSourcePlotWindow.hpp"

DataSourcePlotWindow::DataSourcePlotWindow(std::unique_ptr<DataSource> ds) :
    source(std::move(ds)),
    fieldChooser(source->getDescriptor()),
    plotDataSet(new DataSet)
{
    mainGrid.attach(fieldChooser, 0, 0, 1, 2);
    mainGrid.attach(ep, 1, 0);
    ep.set_expand();

    mainGrid.attach(choosersPanel, 1, 1);
    choosersPanel.set_hexpand();
    choosersPanel.signalNewBounds().connect(sigc::mem_fun(*source, &DataSource::setBoundaries));

    plotDataSet->setColor(Gdk::RGBA("black"));
    ep.addDataSet(plotDataSet);
    std::shared_ptr<DataSet> testS(new DataSet);
    testS->addDataPoint(0, 10);
    testS->addDataPoint(500, 142);
    testS->setColor(Gdk::RGBA("red"));
    ep.addDataSet(testS);

    fieldChooser.set_vexpand();
    fieldChooser.signalOnChoosed().connect([&](std::string table, std::string field) {
        source->stopCollection();
        plotDataSet->clear();
        source->addCollectionType(table, field);
    });

    source->signalOnNewDataArrived().connect(sigc::mem_fun(*this, &DataSourcePlotWindow::onDataArrived));

    set_child(mainGrid);

    set_default_size(1280, 920);
}

void DataSourcePlotWindow::onDataArrived(std::vector<std::vector<std::pair<double, double>>> data) {
    if(!data.empty())
        plotDataSet->addData(data[0].begin(), data[0].end());
}
