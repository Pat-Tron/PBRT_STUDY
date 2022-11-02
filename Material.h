#pragma once

#include <random>
#include "Ray.h"
#include "Color.h"


inline double rand01() { 
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    static std::uniform_real_distribution<double> distr(0.0, 1.0);
    return distr(generator);
}

constexpr double PI{ 3.141592653589793238462643 };

struct Material;
struct HitRec { double t{ 0.0 }; Vec3 p; Vec3 normal; Material *mat{ nullptr }; };

struct Material {
    Color albedo;
    double reflectance{ 1.0 };
    Material() = default;
    Material(const Color &a = 0xFFFFFF) : albedo(a) {}
    Vec3 reflect(const Vec3 &in, const Vec3 &normal) const { return in - 2 * (in * normal) * normal; }
    Vec3 randomSampleInHemiSphere(const Vec3 &normal, bool uniform = true, double range = 1.0) const;
    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const = 0;
};

struct Lambertian : public Material {
    Lambertian() = default;
    Lambertian(const Color &a) : Material(a) {}

    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        return Ray(rec.p, randomSampleInHemiSphere(rec.normal));
    }
};

struct Metal : public Material {
    double fuzz{ 0.0 };

    Metal() = default;
    Metal(const Color &a, double f = 0.0) : Material(a), fuzz(f) {}

    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        Vec3 reflected{ reflect(rayIn.direction.normalized(), rec.normal)};
        if (fuzz == 0.0) return Ray(rec.p, reflected);
        else return Ray(rec.p, randomSampleInHemiSphere(reflected, false, fuzz));
    }
};

struct Dielectric : public Material {
    // GLASS, Index of Refraction
    double IOR{ 1.44 }, IORR{ 1.0 / 1.44 };  // Reciprocal of IOR
    Dielectric() = default;
    Dielectric(const Color &a, double ior = 1.44) :
        Material(a), IOR(ior), IORR(1.0 / ior) {
        criticalAngle = asin(1.0 / IOR);
    }
    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        return Ray(rec.p, refract(rayIn.direction.normalized(), rec.normal));
    }

private:
    double criticalAngle{ asin(1.0 / 1.44) };  // ¡ŸΩÁΩ«
    double schlick(double cosine) const;
    Vec3 refract(const Vec3 &dirIn, const Vec3 &normal) const;
};