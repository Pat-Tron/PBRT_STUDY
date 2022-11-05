#pragma once

#include <vector>
#include <memory>
#include "Ray.h"
#include "Material.h"
#include "AABB.h"


inline Color background(const Ray &ray, const Vec3 &up) {
    double c{ (ray.direction.normalized() * up * up).y };
    double bandwidth{ 0.02 };
    c = c < -bandwidth ? -1.0 : (c < bandwidth ? c / bandwidth : 1.0);
    c = c * 0.5 + 0.5;
    return c * Color(0xBBBBFF) + (1.0 - c) * Color(0xffac9b);
}

struct Primitive {
    static double timeStart, timeEnd;
    Material *mat{ nullptr };
    Vec3 velocity;
    bool moving{ false };
    AABB box;
    Primitive() = default;
    Primitive(Material *mt, const Vec3 &v = Vec3()) : mat(mt), velocity(v), moving(v.length()) {}
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const = 0;
    virtual AABB makeAABB() const = 0;
    virtual void printSelf() const = 0;
};

using primPointer = std::shared_ptr<Primitive>;

struct Primitives : public Primitive {
    size_t primAmounts{ 0 };
    const std::vector<primPointer> &prims;
    Primitives() = default;
    Primitives(const std::vector<primPointer> &ps) : primAmounts(ps.size()), prims(ps) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    virtual AABB makeAABB() const override { return AABB(); }
    virtual void printSelf() const override {}
};

struct Sphere : public Primitive {
    double radius{ 1.0 };
    Vec3 center;
    Sphere() = default;
    Sphere(double r, Vec3 c, Material *m, Vec3 v = Vec3()) :
        Primitive(m, v), radius(r), center(c) { box = makeAABB(); }
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    AABB makeAABB() const override {
        if (!moving) return AABB(center - Vec3(radius), center + Vec3(radius));
        Vec3 centerT0 = center + timeStart * velocity;
        Vec3 centerT1 = center + timeEnd * velocity;
        AABB abT0 = AABB(centerT0 - Vec3(radius), centerT0 + Vec3(radius));
        AABB abT1 = AABB(centerT1 - Vec3(radius), centerT1 + Vec3(radius));
        return abT0 + abT1;
    }
    virtual void printSelf() const override { std::cout << "Sphere   "; }
};

struct Triangle : public Primitive {
    Vec3 A, B, C, AB, AC, ACswitchXZ, normal;  // counterclockwise

    Triangle() = default;
    Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, Material *m) :
        Primitive(m), A(a), B(b), C(c), AB(A - B), AC(A - C), ACswitchXZ(AC.switchXZ()) {
        normal = (AB ^ AC).normalized();
        box = makeAABB();
    };

    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    AABB makeAABB() const override { return AABB(minVec3(minVec3(A, B), C), maxVec3(maxVec3(A, B), C)); }
    virtual void printSelf() const override { std::cout << "Triangle "; }
};

struct BVH : public Primitive {
    // Bounding Volume Hierarchy, container node
    std::shared_ptr<Primitive> left, right;
    BVH() = default;
    BVH(const std::vector<primPointer> &constPrims, size_t start, size_t end);

    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    AABB makeAABB() const override { return left->box + right->box; }
    virtual void printSelf() const override;
};