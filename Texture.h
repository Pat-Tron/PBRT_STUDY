#pragma once

#include "Vec3.h"
#include "Color.h"
#include "utility.h"
#include <algorithm>

struct Texture {
    double scale{ 1.0 };
    Vec3 offset;
    Texture() = default;
    Texture(double s, Vec3 o = Vec3()) : scale(1.0 / s) , offset(o) {}
    // value
    virtual Color v(double u, double v, const Vec3 &p) const = 0;
};

struct ConstantTexture : public Texture {
    Color albedo;

    ConstantTexture() = default;
    ConstantTexture(Color a) : albedo(a) {}
    virtual Color v(double u, double v, const Vec3 &p) const override { return albedo; }
    operator std::shared_ptr<Texture>() { return std::make_shared<ConstantTexture>(*this); }
};

struct CheckerTexture : public Texture {
    std::shared_ptr<Texture> odd, even;

    CheckerTexture() = default;
    CheckerTexture(std::shared_ptr<Texture> o, std::shared_ptr<Texture> e, double s = 1.0) :
        Texture(s), odd(o), even(e) {}
    virtual Color v(double u, double v, const Vec3 &p) const override {
        int xOdd{ static_cast<int>(ceil((p.x + 0.01) * scale)) & 1 };
        int yOdd{ static_cast<int>(ceil((p.y + 0.01) * scale)) & 1 };
        int zOdd{ static_cast<int>(ceil((p.z + 0.01) * scale)) & 1 };
        if (xOdd ^ yOdd ^ zOdd) return odd->v(u, v, p);
        else return even->v(u, v, p);
    }
    operator std::shared_ptr<Texture>() { return std::make_shared<CheckerTexture>(*this); }
};

struct PerlinNoise : public Texture {
    std::array<double, 256> raws{ 0.0 };
    std::array<int, 256> permutationX{ 0 };
    std::array<int, 256> permutationY{ 0 };
    std::array<int, 256> permutationZ{ 0 };

    // Turbulence
    // Inspired by Houdini noise fractal parameters. Not sure it's correct.
    int octaves{ 0 };  // fractal depth
    double lacunarity{ 0.0 };  // frequency multiplication
    double roughness{ 0.5 };  // Attenuation or falloff
    double normalizeFactor{ 0.0 };
    bool fold{ false };

    PerlinNoise() = default;
    PerlinNoise(double s, bool f = false, int o = 0, double l = 2.0, double r = 0.5, Vec3 offset = Vec3()) :
        Texture(s, offset), octaves(o), lacunarity(l), roughness(r), fold(f) {
        normalizeFactor = (1.0 - roughness) / (1.0 - pow(roughness, octaves + 1.0));

        // URGB: Uniform Random Bit Generator
        static std::default_random_engine URGB1(1), URGB2(2), URGB3(3);
        for (int i{ 0 }; i < 256; ++i) {
            raws[i] = rand01();
            permutationX[i] = i;
            permutationY[i] = i;
            permutationZ[i] = i;
        }
        std::shuffle(permutationX.begin(), permutationX.end(), URGB1);
        std::shuffle(permutationY.begin(), permutationY.end(), URGB2);
        std::shuffle(permutationZ.begin(), permutationZ.end(), URGB3);
    }
    virtual Color v(double u, double v, const Vec3 &p) const override;
    operator std::shared_ptr<Texture>() { return std::make_shared<PerlinNoise>(*this); }

private:
    Vec3 randomGradiant(const Vec3 &p) const;
    double randomDouble(const Vec3 &p) const;
    double plainPerlin(const Vec3 &p) const;
    double smoothstep(double a0, double a1, double w) const;
};

inline double PerlinNoise::randomDouble(const Vec3 &p) const {
    return raws[
        permutationX[static_cast<int>(p.x) & 0xFF] ^
        permutationY[static_cast<int>(p.y) & 0xFF] ^
        permutationZ[static_cast<int>(p.z) & 0xFF]
    ];
}

inline Vec3 PerlinNoise::randomGradiant(const Vec3 &p) const {
    // 单位半球面均匀撒点 https://zhuanlan.zhihu.com/p/340929847
    Vec3 flooredP{ p.vecFloor() };
    float phi = randomDouble(flooredP) * 2.0 * PI;

    flooredP.x += 10;
    float theta = acos(1.0 - 2 * randomDouble(flooredP));
    return Vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
}

inline double PerlinNoise::smoothstep(double a0, double a1, double w) const {
    // 6阶Hermite插值多项式
    // https://zhuanlan.zhihu.com/p/148623774
    // 计算方法
    // https://en.wikipedia.org/wiki/Hermite_polynomials
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}