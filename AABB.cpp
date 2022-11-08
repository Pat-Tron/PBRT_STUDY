#include "AABB.h"

double AABB::padding = 0.00001;

bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
    double t0x{ (minBound.x - ray.origin.x) * ray.directionReciprocal.x };
    double t0y{ (minBound.y - ray.origin.y) * ray.directionReciprocal.y };
    double t0z{ (minBound.z - ray.origin.z) * ray.directionReciprocal.z };
    double t1x{ (maxBound.x - ray.origin.x) * ray.directionReciprocal.x };
    double t1y{ (maxBound.y - ray.origin.y) * ray.directionReciprocal.y };
    double t1z{ (maxBound.z - ray.origin.z) * ray.directionReciprocal.z };
    if (!ray.xPositive) std::swap(t0x, t1x);
    if (!ray.yPositive) std::swap(t0y, t1y);
    if (!ray.zPositive) std::swap(t0z, t1z);
    tMin = std::max(tMin, std::max(t0x, std::max(t0y, t0z)));
    tMax = std::min(tMax, std::min(t1x, std::min(t1y, t1z)));
    return tMin < tMax;
}