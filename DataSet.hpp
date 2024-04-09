#pragma once

#include <limits>
#include <vector>
#include <sigc++/sigc++.h>

class DataSet {
public:

    struct Extrems {
        double maxX = std::numeric_limits<double>::lowest();
        double minX = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();
        double minY = std::numeric_limits<double>::max();
    };


    void addDataPoint(double x, double y);

    template <typename Iterator>    //iterators must point to pair<x, y>
    void addData(Iterator begin, Iterator end);

    sigc::signal<void(DataSet&)> signalOnChanged() const;

    size_t getNumberOfPoints() const;

    const double* getFirstElementAddress() const;
    size_t getSizeOfBuffer() const;
    Extrems getExtremums() const;

private:
    void sort();

    void addPoint(double x, double y);
    Extrems extr;

    sigc::signal<void(DataSet&)> signalChanged;

    std::vector<double> points; //contains points as double x, double y, double x, double y...

};
