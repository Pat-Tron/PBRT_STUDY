#pragma once

#include "Vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;
    double time{ -1.0 };

    Ray() = default;
    Ray(const Vec3 &o, const Vec3 &d, const double &t = -1.0) : origin(o), direction(d), time(t) {}
    Vec3 pointAtT(double t) const { return origin + direction * t; }
};