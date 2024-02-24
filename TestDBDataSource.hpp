#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "DataSource.hpp"

class TestDBDataSource : public DataSource {
public:
    TestDBDataSource(const std::string fileName) : _fileName(fileName) {}

    virtual std::shared_ptr< std::pair<std::vector<double>, std::vector<double>> > getDataSet() {
        auto result = std::make_shared<std::pair<std::vector<double>, std::vector<double>>>();

        try
        {
            SQLite::Database db(_fileName);

            SQLite::Statement query(db, "SELECT num_sub, ffa FROM amplitude WHERE id < ?");

            query.bind(1, 57);

            while (query.executeStep())
            {
                int num_sub = query.getColumn(0);
                result->first.push_back(num_sub);

                double ffa  = query.getColumn(1);
                result->second.push_back(ffa);
            }
        }
        catch (std::exception& e)
        {
            std::cout << "exception: " << e.what() << std::endl;
        }

        return result;
    }

    virtual std::shared_ptr< std::vector< std::shared_ptr<DataSource> > > getChildDataSources() {
        return std::make_shared<std::vector< std::shared_ptr<DataSource> > >();
    }

    virtual Glib::ustring getName() {
        return "Тестовый источник из БД";
    }

private:
    const std::string _fileName;

};
