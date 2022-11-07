#pragma once

#include "Vec3.h"
#include "Color.h"

struct Texture {
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
    double scale{ 1.0 };

    CheckerTexture() = default;
    CheckerTexture(std::shared_ptr<Texture> o, std::shared_ptr<Texture> e, double s = 1.0) :
        odd(o), even(e), scale(1.0 / s) {}
    virtual Color v(double u, double v, const Vec3 &p) const override {
        int xOdd{ static_cast<int>(ceil((p.x + 0.01) * scale)) & 1 };
        int yOdd{ static_cast<int>(ceil((p.y + 0.01) * scale)) & 1 };
        int zOdd{ static_cast<int>(ceil((p.z + 0.01) * scale)) & 1 };
        if (xOdd ^ yOdd ^ zOdd) return odd->v(u, v, p);
        else return even->v(u, v, p);
    }
    operator std::shared_ptr<Texture>() { return std::make_shared<CheckerTexture>(*this); }
};