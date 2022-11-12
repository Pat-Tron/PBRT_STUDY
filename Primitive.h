#pragma once

#include <vector>
#include <memory>
#include "Ray.h"
#include "Material.h"
#include "AABB.h"
#include "Transformation.h"

struct Primitive {
    static double timeStart, timeEnd;
    static bool motionBlur;
    std::shared_ptr<Material> mat;
    Vec3 centroid;
    Vec3 velocity;
    bool moving{ false };
    AABB box;
    Primitive() = default;
    template <typename MaterialType>
    Primitive(const MaterialType &m, Vec3 c = Vec3(), Vec3 v = Vec3()) :
        mat(std::make_shared< MaterialType>(m)), centroid(c), velocity(v), moving(v.length()) { moving = moving && motionBlur; }
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const = 0;
    virtual void makeAABB() = 0;
    virtual void printSelf() const = 0;
    virtual Vec2 uv(const Vec3 &p) const = 0;
    virtual void transform(const Transformation &trans) = 0;
};

using primPointer = std::shared_ptr<Primitive>;

struct BVH : public Primitive {
    // Bounding Volume Hierarchy, container node
    using itrt = std::vector<primPointer>::iterator;
    std::shared_ptr<Primitive> left, right;
    BVH() = default;
    BVH(std::vector<primPointer> &prims, itrt start, itrt end);

    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    void makeAABB() override { box = AABB(); }
    virtual void printSelf() const override;
    virtual Vec2 uv(const Vec3 &p) const override { return Vec2(); }
    virtual void transform(const Transformation &trans) override {}
};

struct Sphere : public Primitive {
    double radius{ 1.0 };
    Vec3 center;

    Sphere() = default;
    template <typename MaterialType>
    Sphere(double r, const MaterialType &m, Vec3 v = Vec3()) :
        Primitive(m, Vec3(), v), radius(r), center(Vec3()) {}
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    void makeAABB() override {
        if (!moving) box = AABB(center - Vec3(radius), center + Vec3(radius));
        Vec3 centerT0 = center + timeStart * velocity;
        Vec3 centerT1 = center + timeEnd * velocity;
        AABB abT0 = AABB(centerT0 - Vec3(radius), centerT0 + Vec3(radius));
        AABB abT1 = AABB(centerT1 - Vec3(radius), centerT1 + Vec3(radius));
        box = abT0 + abT1;
    }
    virtual void printSelf() const override { std::cout << "Sphere " << typeid(*mat).name(); }
    virtual Vec2 uv(const Vec3 &p) const override {
        double phi{ atan2(p.z, p.x) };
        double theta{ asin(p.y) };
        return Vec2(1.0 - (phi + PI) * PI_RECIPROCAL * 0.5, 1.0 - (theta + PI * 0.5) * PI_RECIPROCAL);
    }
    virtual void transform(const Transformation &trans) override {
        center *= trans; centroid = center;
    }
};

struct Triangle : public Primitive {
    Vec3 A, B, C, BA, CA, CAswitchXZ, normal;  // counterclockwise
    Vec2 uvA, uvB, uvC;

    Triangle() = default;
    template <typename MaterialType>
    Triangle(
        const Vec3 &a, const Vec3 &b, const Vec3 &c,
        const Vec2 &uva, const Vec2 &uvb, const Vec2 &uvc,
        const MaterialType &m
    ) : Primitive(m, getCentroid(a, b, c)), A(a), B(b), C(c), uvA(uva), uvB(uvb), uvC(uvc) { init(); }

    Vec3 getCentroid(Vec3 a, Vec3 b, Vec3 c) const { return (b - a + c - a) / 3.0; }
    void init() {
        centroid = getCentroid(A, B, C);
        BA = A - B;
        CA = A - C;
        CAswitchXZ = CA.switchXZ();
        normal = (BA ^ CA).normalized();
    }
    bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    void makeAABB() override { box = AABB(minVec3(minVec3(A, B), C), maxVec3(maxVec3(A, B), C)); }
    virtual void printSelf() const override { std::cout << "Triangle " << typeid(*mat).name(); }
    virtual Vec2 uv(const Vec3 &p) const override {
        // p: Centrobaric Coordinate
        return uvA * p.x + uvB * p.y +uvC * p.z;
    }
    virtual void transform(const Transformation &trans) override {
        A *= trans; B *= trans; C *= trans; init();
    }
};

struct Volume : public Primitive {
    AABB volumeBoundary;
    Transformation tf, tfi, rot;
    double density{ 1.0 };
    
    Volume() = default;
    template <typename TextureType = ConstantTexture>
    Volume(double x, double z, double y, double d = 1.0, const TextureType &t = ConstantTexture(WHITE)) :
        Primitive(Isotropic(t)), density(d),
        volumeBoundary({ Vec3(-x * 0.5, -y * 0.5, -z * 0.5), Vec3(x * 0.5, y * 0.5, z * 0.5), 0.0 }) {}
    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const override;
    virtual void makeAABB() override { box = volumeBoundary * tf; }
    virtual void printSelf() const override { std::cout << "Volume " << typeid(*mat).name(); }
    virtual Vec2 uv(const Vec3 &p) const override { return Vec2(); };
    virtual void transform(const Transformation &trans) override {
        tf = trans;
        tfi = trans.inverted();
        rot = tfi;
        rot[3] = 0.0;
        rot[7] = 0.0;
        rot[11] = 0.0;
        centroid *= trans;
    }
};