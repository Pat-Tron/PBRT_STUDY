#pragma once

#include "Ray.h"
#include "Material.h"

#include <vector>
#include <memory>

#include "AABB.h"

struct Primitive {
    Material *mat{ nullptr };
    Primitive() = default;
    Primitive(Material *m) : mat(m) {}
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const = 0;
    virtual const AABB &aabb() const = 0;
};

struct Primitives : public Primitive {
    size_t primAmounts{ 0 };
    std::vector<std::shared_ptr<const Primitive>> prims;
    Primitives() = default;
    Primitives(const std::vector<std::shared_ptr<const Primitive>> &ps) : primAmounts(ps.size()), prims(ps) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    const AABB &aabb() const override {
        AABB bigBox{ prims[0]->aabb() };
        if (prims.size() == 1) return bigBox;
        for (int i = 1; i < prims.size(); ++i) bigBox += prims[i]->aabb();
        return bigBox;
    }
};

struct Sphere : public Primitive {
    double radius{ 1.0 };
    Vec3 center, velocity;
    bool moving{ false };
    Sphere() = default;
    Sphere(double r, Vec3 c, Material *m, Vec3 v = Vec3(0.0, 0.0, 0.0)) :
        Primitive(m), radius(r), center(c), velocity(v), moving(v.length()) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    const AABB &aabb() const override { return AABB(center - Vec3(radius), center + Vec3(radius)); }
};

struct Triangle : public Primitive {
    Vec3 A, B, C, AB, AC, ACswitchXZ, normal;  // counterclockwise

    Triangle() = default;
    Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, Material *m) :
        Primitive(m), A(a), B(b), C(c), AB(A - B), AC(A - C), ACswitchXZ(AC.switchXZ()) {
        normal = (AB ^ AC).normalized();
    };

    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    const AABB &aabb() const override { return AABB(minVec3(minVec3(A, B), C), maxVec3(maxVec3(A, B), C)); }
};

inline Color background(const Ray &ray, const Vec3 &up) {
    double c{ (ray.direction.normalized() * up * up).y };
    double bandwidth{ 0.02 };
    c = c < -bandwidth ? -1.0 : (c < bandwidth ? c / bandwidth : 1.0);
    c = c * 0.5 + 0.5;
    return c * Color(0xBBBBFF) + (1.0 - c) * Color(0xffac9b);
    //return Color(0xffac9b);
}
