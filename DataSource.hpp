#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <glibmm/ustring.h>

class DataSource {
public:

    //virtual void setCallback_onNewData(std::function<void(DataSource&)>) = 0;

    virtual std::shared_ptr< std::pair<std::vector<double>, std::vector<double>> > getDataSet() = 0;
    virtual std::shared_ptr< std::vector< std::shared_ptr<DataSource> > > getChildDataSources() = 0;
    virtual Glib::ustring getName() = 0;

};
