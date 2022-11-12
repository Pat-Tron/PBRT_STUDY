#include "AABB.h"

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

std::tuple<double, double> AABB::hit(const Ray &ray) const {
    double t0x{ (minBound.x - ray.origin.x) * ray.directionReciprocal.x };
    double t0y{ (minBound.y - ray.origin.y) * ray.directionReciprocal.y };
    double t0z{ (minBound.z - ray.origin.z) * ray.directionReciprocal.z };
    double t1x{ (maxBound.x - ray.origin.x) * ray.directionReciprocal.x };
    double t1y{ (maxBound.y - ray.origin.y) * ray.directionReciprocal.y };
    double t1z{ (maxBound.z - ray.origin.z) * ray.directionReciprocal.z };
    if (!ray.xPositive) std::swap(t0x, t1x);
    if (!ray.yPositive) std::swap(t0y, t1y);
    if (!ray.zPositive) std::swap(t0z, t1z);
    return std::make_tuple(std::max(t0x, std::max(t0y, t0z)), std::min(t1x, std::min(t1y, t1z)));
}

AABB AABB::operator*(const Transformation &trans) const {
    /*
    Corner notation inside specific int lattice

                      ¡ü y
                     |
                    |
                   E--------F
                 /|       /|
               / |      / |
             H--------G  |
            |  A-----|--B--------¡ú x
           | /      | /
          |/       |/
         D--------C
       /
     ¡ý z

    */
    Vec3 A = minBound, G = maxBound;
    Vec3 B = Vec3(G.x, A.y, A.z) * trans;
    Vec3 C = Vec3(G.x, A.y, G.z) * trans;
    Vec3 D = Vec3(A.x, A.y, G.z) * trans;
    Vec3 E = Vec3(A.x, G.y, A.z) * trans;
    Vec3 F = Vec3(G.x, G.y, A.z) * trans;
    Vec3 H = Vec3(A.x, G.y, G.z) * trans;
    A *= trans;
    G *= trans;
    
    return AABB(
        minVec3(minVec3(minVec3(minVec3(minVec3(minVec3(minVec3(A, B), C), D), E), F), G), H),
        maxVec3(maxVec3(maxVec3(maxVec3(maxVec3(maxVec3(maxVec3(A, B), C), D), E), F), G), H)
    );
}