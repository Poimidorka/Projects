#pragma once

#include "point.h"

#include <unordered_map>
#include <vector>

class Topology {
public:
    virtual std::vector<Point> Lookup() const = 0;
    virtual size_t MeasureDistance(const Point& from, const Point& to) const = 0;
    virtual void Move(const Point& point) = 0;
};
