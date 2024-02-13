#include "DataSource.hpp"

class TestDataSource : public DataSource {
public:

    virtual operator bool() {
        return true;
    }

    virtual DataSource& operator>>(std::pair<double, double>& p) {
        p = {x[pos], y[pos]};
        pos++;

        return *this;
    }

    virtual std::shared_ptr< std::pair<std::vector<double>, std::vector<double>> > getDataSet() {
        return std::make_shared<std::pair<std::vector<double>, std::vector<double>>>(x, y);
    }

private:
    size_t pos = 0;

    std::vector<double> x {1, 2, 3, 4, 5, 6};
    std::vector<double> y {5, 10, 20, 35, 55, 80};

};
