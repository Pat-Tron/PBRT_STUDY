#pragma once

#include "Ray.h"
#include "Material.h"

#include <vector>
#include <memory>

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
    Material *mat;
    Sphere() = default;
    Sphere(double r, Vec3 c, Material *m) : radius(r), center(c), mat(m) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
};

inline Color background(const Ray &ray) {
    //return Color(0x000000);
    double c{ ray.direction.normalized().y * 0.5 + 0.5};
    return c * Color(0.8, 0.8, 1.0) + (1.0 - c) * Color(0xFFFFFF);
}
