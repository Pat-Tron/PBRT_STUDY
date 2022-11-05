#include "AABB.h"

double AABB::padding = 0.0;

//bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
//    // slower
//    Vec3 t0{ (minBound - ray.origin) / ray.direction };
//    Vec3 t1{ (maxBound - ray.origin) / ray.direction };
//    if (t0.x > t1.x) std::swap(t0.x, t1.x);
//    if (t0.y > t1.y) std::swap(t0.y, t1.y);
//    if (t0.z > t1.z) std::swap(t0.z, t1.z);
//    return t0.max(tMin) < t1.min(tMax);
//}

bool AABB::hit(const Ray &ray, double tMin, double tMax) const {
    double t0x{ (minBound.x - ray.origin.x) / ray.direction.x };
    double t0y{ (minBound.y - ray.origin.y) / ray.direction.y };
    double t0z{ (minBound.z - ray.origin.z) / ray.direction.z };
    double t1x{ (maxBound.x - ray.origin.x) / ray.direction.x };
    double t1y{ (maxBound.y - ray.origin.y) / ray.direction.y };
    double t1z{ (maxBound.z - ray.origin.z) / ray.direction.z };
    if (t0x > t1x) std::swap(t0x, t1x);
    if (t0y > t1y) std::swap(t0y, t1y);
    if (t0z > t1z) std::swap(t0z, t1z);
    tMin = std::max(tMin, std::max(t0x, std::max(t0y, t0z)));
    tMax = std::min(tMax, std::min(t1x, std::min(t1y, t1z)));
    return tMin < tMax;
}