#pragma once
#include "Ray.h"

struct AABB {
    // Acceleration algorithm: Axis-Aligned Bounding Box
    Vec3 minBound, maxBound;

    AABB() = default;
    AABB(const Vec3 &min, const Vec3 &max) : minBound(min), maxBound(max) {}
    bool hit(const Ray &ray, double tMin, double tMax) const;
    AABB operator+(const AABB &b) {
        return AABB(minVec3(minBound, b.minBound), maxVec3(maxBound, b.maxBound));
    }
    AABB &operator+=(const AABB &b) {
        minBound = minVec3(minBound, b.minBound);
        maxBound = maxVec3(maxBound, b.maxBound);
        return *this;
    }
    AABB &expand(double padding) {
        minBound -= Vec3(padding);
        maxBound += Vec3(padding);
        return *this;
    }
};

