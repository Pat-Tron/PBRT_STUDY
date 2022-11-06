#pragma once

#include "Vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;
    Vec3 directionReciprocal;
    double time{ -1.0 };
    bool xPositive{ true }, yPositive{ true }, zPositive{ true };

    Ray() = default;
    Ray(const Vec3 &o, const Vec3 &d, const double &t = -1.0) :
        origin(o), direction(d), directionReciprocal(d.reciprocal()),
        xPositive(d.x > 0), yPositive(d.y > 0), zPositive(d.z > 0),
        time(t) {}
    Vec3 pointAtT(double t) const { return origin + direction * t; }
};