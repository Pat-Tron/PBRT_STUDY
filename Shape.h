#pragma once

#include "Ray.h"
#include "Color.h"

#include <vector>
#include <memory>

struct HitRec { double t{ 0.0 }; Vec3 p; Vec3 normal; };
struct Primitive {
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const = 0;
};

struct Primitives : public Primitive {
    size_t primAmounts{ 0 };
    std::vector<std::shared_ptr<const Primitive>> prims;
    Primitives() = default;
    Primitives(const std::vector<std::shared_ptr<const Primitive>> &ps) : primAmounts(ps.size()), prims(ps) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
};

struct Sphere : public Primitive {
    double radius{ 1.0 };
    Vec3 center;
    Color albedo;
    Sphere() = default;
    Sphere(double r, Vec3 c, int al) : radius(r), center(c), albedo(Color(al)) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
};

inline Color background(const Ray &ray) {
    return ray.direction.getUnit() * 0.5 + Vec3(0.5, 0.5, 0.5);
}
