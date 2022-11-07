#pragma once

#include "utility.h"
#include "Ray.h"
#include "Color.h"
#include "Texture.h"

struct Material;
struct HitRec { double t{ 0.0 }; Vec3 p; Vec3 normal; std::shared_ptr<Material> mat; };

struct Material {
    double reflectance{ 1.0 };
    std::shared_ptr<Texture> texture;

    Material() = default;
    Material(std::shared_ptr<Texture> tp) : texture(tp) {}
    Material(const Color &a = 0xFFFFFF) : texture(std::make_shared<ConstantTexture>(a)) {}
    Vec3 reflect(const Vec3 &in, const Vec3 &normal) const { return in - 2 * (in * normal) * normal; }
    Vec3 randomSampleInHemiSphere(const Vec3 &normal, bool uniform = true, double range = 1.0) const;
    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const = 0;
};

struct Lambertian : public Material {
    Lambertian() = default;
    Lambertian(const Color &a) : Material(a) {}
    Lambertian(std::shared_ptr<Texture> tp) : Material(tp) {}

    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        return Ray(rec.p, randomSampleInHemiSphere(rec.normal), rayIn.time);
    }
    operator std::shared_ptr<Material>() { return std::make_shared<Lambertian>(*this); }
};

struct Metal : public Material {
    double fuzz{ 0.0 };

    Metal() = default;
    Metal(const Color &a, double f = 0.0) : Material(a), fuzz(f) {}
    Metal(std::shared_ptr<Texture> tp, double f = 0.0) : Material(tp), fuzz(f) {}

    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        Vec3 reflected{ reflect(rayIn.direction.normalized(), rec.normal)};
        if (fuzz == 0.0) return Ray(rec.p, reflected, rayIn.time);
        else return Ray(rec.p, randomSampleInHemiSphere(reflected, false, fuzz), rayIn.time);
    }
    operator std::shared_ptr<Material>() { return std::make_shared<Metal>(*this); }
};

struct Dielectric : public Material {
    // GLASS, Index of Refraction
    double IOR{ 1.44 }, IORR{ 1.0 / 1.44 };  // Reciprocal of IOR
    Dielectric() = default;
    Dielectric(std::shared_ptr<Texture> tp, double ior = 1.44) :
        Material(tp), IOR(ior), IORR(1.0 / ior), criticalAngle(asin(1.0 / IOR)) {}
    Dielectric(const Color &a, double ior = 1.44) :
        Material(a), IOR(ior), IORR(1.0 / ior), criticalAngle(asin(1.0 / IOR)) {}

    virtual Ray scatter(const Ray &rayIn, const HitRec &rec) const override {
        return Ray(rec.p, refract(rayIn.direction.normalized(), rec.normal), rayIn.time);
    }
    operator std::shared_ptr<Material>() { return std::make_shared<Dielectric>(*this); }

private:
    double criticalAngle{ asin(1.0 / 1.44) };  // ¡ŸΩÁΩ«
    double schlick(double cosine) const;
    Vec3 refract(const Vec3 &dirIn, const Vec3 &normal) const;
};