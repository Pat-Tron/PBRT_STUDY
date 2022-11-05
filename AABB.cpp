#include "AABB.h"

double AABB::padding = 0.0;

//bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
//    for (int i{ 0 }; i < 3; ++i) {
//        double t0{ (minBound[i] - ray.origin[i]) / ray.direction[i] };
//        double t1{ (maxBound[i] - ray.origin[i]) / ray.direction[i] };
//        if (isnan(t0) || isnan(t1)) return false;
//        if (ray.direction[i] < 0.0) std::swap(t0, t1);
//        if (t0 > tMax || t1 < tMin) return false;
//        tMin = std::max(tMin, t0);
//        tMax = std::min(tMax, t1);
//    }
//    return true;
//}

bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
    Vec3 t0{ (minBound - ray.origin) / ray.direction };
    Vec3 t1{ (maxBound - ray.origin) / ray.direction };
    if (t0.x > t1.x) std::swap(t0.x, t1.x);
    if (t0.y > t1.y) std::swap(t0.y, t1.y);
    if (t0.z > t1.z) std::swap(t0.z, t1.z);
    return t0.max(tMin) < t1.min(tMax);
}