#pragma once

#include "DataSource.hpp"
#include "TestDBDataSource.hpp"
#include "TestDataSource.hpp"

class DataSourcesRoot : public DataSource {
public:

    virtual std::shared_ptr< std::pair<std::vector<double>, std::vector<double>> > getDataSet() {
        return std::make_shared< std::pair<std::vector<double>, std::vector<double>> >();
    }

    virtual std::shared_ptr< std::vector< std::shared_ptr<DataSource> > > getChildDataSources() {
        return sources;
    }

    virtual Glib::ustring getName() {
        return "";
    }

    static DataSourcesRoot& getRoot() {
        static DataSourcesRoot root;
        return root;
    }

    static std::shared_ptr<DataSourcesRoot> getSharedPtr() {
        auto& source = getRoot();
        return std::shared_ptr<DataSourcesRoot>(&source);
    }

private:

    std::shared_ptr< std::vector< std::shared_ptr<DataSource> > > sources;

    DataSourcesRoot() {
        // Collect all data sources
        sources->emplace_back(std::make_shared<TestDBDataSource>("/home/de/DB/Exp12.01.24.sqlite"));
        sources->emplace_back(std::make_shared<TestDataSource>());
    }

};
