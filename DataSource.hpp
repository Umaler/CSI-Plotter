#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <functional>

class DataSource {
public:

    //virtual void setCallback_onNewData(std::function<void(DataSource&)>) = 0;

    virtual operator bool() = 0;

    virtual DataSource& operator>>(std::pair<double, double>&) = 0;
    virtual std::shared_ptr< std::vector<double> > getDataSet() = 0;

};
