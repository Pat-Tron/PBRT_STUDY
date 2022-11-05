#include "AABB.h"

double AABB::padding = 0.0;

bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
    for (int i{ 0 }; i < 3; ++i) {
        double t0{ (minBound[i] - ray.origin[i]) / ray.direction[i] };
        double t1{ (maxBound[i] - ray.origin[i]) / ray.direction[i] };
        if (isnan(t0) || isnan(t1)) return false;
        if (ray.direction[i] < 0.0) std::swap(t0, t1);
        if (t0 > tMax || t1 < tMin) return false;
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
    }
    return true;
}