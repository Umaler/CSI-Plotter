#pragma once

#include <limits>

struct Boundary {
    double min = 0;
    double max = std::numeric_limits<double>::max();
};

struct Boundaries {
    Boundary id;
    Boundary packId;
    Boundary measId;
    Boundary numSub;
};
