#pragma once

#include "utility.h"
#include "Ray.h"
#include "Color.h"
#include "Texture.h"
#include <tuple>

struct Material;
struct HitRec {
    double t{ 0.0 };
    Vec3 p, normal;
    Vec2 uv;
    std::shared_ptr<Material> mat;
};

struct Material {
    double reflectance{ 1.0 };
    bool LIGHT{ false };
    std::shared_ptr<Texture> texture;

    Material() = default;
    Material(const PALETTE &p, double r = 1.0) :
        texture(std::make_shared<ConstantTexture>(ConstantTexture(p))), reflectance(r) {}
    Material(const Color &c, double r = 1.0) :
        texture(std::make_shared<ConstantTexture>(ConstantTexture(c))), reflectance(r) {}
    template <typename TextureType>
    Material(const TextureType &t, double r = 1.0) :
        texture(std::make_shared<TextureType>(t)), reflectance(r) {}
    Vec3 reflect(const Vec3 &in, const Vec3 &normal) const { return in - 2 * (in * normal) * normal; }
    std::tuple<Vec3, double> randomSampleInHemiSphere(const Vec3 &normal, double range = 1.0) const;
    Vec3 randomSampleInSphere() const {
        float phi = rand01() * 2.0 * PI;
        float theta = acos(1.0 - 2 * rand01());
        double sinTheta{ sin(theta) };
        return Vec3(sinTheta * cos(phi), cos(theta), sinTheta * sin(phi));
    }
    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const = 0;
};

struct Lambertian : public Material {
    Lambertian() = default;
    template <typename TextureType>
    Lambertian(const TextureType &t, double r = 1.0) : Material(t, r) {}
    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const override {
        // C++17 ÷ß≥÷ auto [a, b] = ....;
        const auto &tmp{ randomSampleInHemiSphere(rec.normal) };
        return std::make_tuple(
            Ray(rec.p, std::get<0>(tmp), rayIn.time),
            std::get<1>(tmp) * PI_RECIPROCAL  // PDF: cos(theta) / PI , importance sampling
        );
    }
};

struct Metal : public Material {
    double fuzz{ 0.0 };

    Metal() = default;
    template <typename TextureType>
    Metal(const TextureType &t, double f = 0.0, double r = 1.0) : Material(t, r), fuzz(f) {}

    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const override {
        Vec3 reflected{ reflect(rayIn.direction.normalized(), rec.normal)};
        if (fuzz == 0.0) return std::make_tuple(Ray(rec.p, reflected, rayIn.time), 1.0);
        else {
            const auto &tmp{ randomSampleInHemiSphere(reflected, fuzz) };
            return std::make_tuple(Ray(rec.p, std::get<0>(tmp), rayIn.time), 1.0);
        }
    }
};

struct Dielectric : public Material {
    // GLASS, Index of Refraction
    double IOR{ 1.44 }, IORR{ 1.0 / 1.44 };  // Reciprocal of IOR
    Dielectric() = default;
    template <typename TextureType>
    Dielectric(const TextureType &t, double ior = 1.44, double r = 1.0) :
        Material(t, r), IOR(ior), IORR(1.0 / ior), criticalAngle(asin(1.0 / IOR)) {}

    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const override {
        return std::make_tuple(
            Ray(rec.p, refract(rayIn.direction.normalized(), rec.normal), rayIn.time), 1.0);
    }

private:
    double criticalAngle{ asin(1.0 / 1.44) };  // ¡ŸΩÁΩ«
    double schlick(double cosine) const;
    Vec3 refract(const Vec3 &dirIn, const Vec3 &normal) const;
};

struct DiffuseLight : public Material {
    DiffuseLight() = default;
    template <typename TextureType>
    DiffuseLight(const TextureType &t) : Material(t, 0.0) { Material::LIGHT = true; }
    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const override {
        return std::make_tuple(Ray(), 1.0); }
};

struct Isotropic : public Material {
    Isotropic() = default;
    template <typename TextureType>
    Isotropic(const TextureType &t, double r = 1.0) : Material(t, r) {}
    virtual std::tuple<Ray, double> scatter(const Ray &rayIn, const HitRec &rec) const override {
        return std::make_tuple(Ray(rec.p, randomSampleInSphere(), rayIn.time), 1.0);
    }
};