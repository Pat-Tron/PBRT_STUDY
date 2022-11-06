#pragma once

#include "Primitive.h"

struct Geometry {
    std::vector<primPointer> prims;
    Geometry() = default;
    //operator std::shared_ptr<Geometry>() { return std::make_shared<Geometry>(*this); }
};

struct GeometryContainer : public Geometry {
    GeometryContainer &operator+(std::shared_ptr<Geometry> gp) {
        for (auto primp : gp->prims) prims.push_back(primp);
        return *this;
    }
    operator std::shared_ptr<Geometry>() { return std::make_shared<GeometryContainer>(*this); }
};

struct Square : public Geometry {
    double xLen, zLen;
    Vec3 center;
    Square() = default;
    Square(std::shared_ptr<Material> mat, double xl, double zl, Vec3 c = Vec3()) :
        xLen(xl), zLen(zl), center(c) {
        // counterclock wise
        Vec3 A { Vec3( xLen * 0.5, 0.0,  zLen * 0.5) + center };
        Vec3 B { Vec3( xLen * 0.5, 0.0, -zLen * 0.5) + center };
        Vec3 C { Vec3(-xLen * 0.5, 0.0, -zLen * 0.5) + center };
        Vec3 D { Vec3(-xLen * 0.5, 0.0,  zLen * 0.5) + center };
        prims.push_back(std::make_shared<Triangle>(Triangle(A, B, D, mat)));
        prims.push_back(std::make_shared<Triangle>(Triangle(B, C, D, mat)));
    }
    Square(std::shared_ptr<Material> mat, double len, Vec3 center = Vec3()) : Square(mat, len, len, center) {}
    operator std::shared_ptr<Geometry>() { return std::make_shared<Square>(*this); }
};

struct PrimBall : public Geometry {
    PrimBall() = default;
    PrimBall(std::shared_ptr<Material> mat, double r, Vec3 c, Vec3 v = Vec3()) {
        prims.push_back(std::make_shared<Sphere>(Sphere(r, c, mat, v)));
    }
    operator std::shared_ptr<Geometry>() { return std::make_shared<PrimBall>(*this); }
};

struct RandomBalls : public Geometry {
    std::vector<int>palette {
        0xb6bce2, 0x5f608c, 0x228867, 0x50d7b3, 0xc6cedf,
        0xefeada, 0xfcd75d, 0x4eb8ba, 0xf1503d, 0xf08484        
    };

    RandomBalls() = default;
    RandomBalls(
        int count, double radius, const Square &square,
        std::shared_ptr<Material> mat = nullptr, double bias = 0.5) {
        bool hasUserSpecializedMaterial{ mat };

        for (int i{ 0 }; i < count; ++i) {
            double randX{ (rand01() - 0.5) * square.xLen };
            double randZ{ (rand01() - 0.5) * square.zLen };
            double randR{ (rand01() * 2 - 1.0) * radius * bias + radius };
            if (!hasUserSpecializedMaterial) {
                int colIndex{ static_cast<int>(rand01() * 10) };
                int matIndex{ static_cast<int>(rand01() * 3) };
                if (matIndex == 0)
                    mat = std::make_shared<Lambertian>(Lambertian(palette[colIndex]));
                else if (matIndex == 1)
                    mat = std::make_shared<Metal>(Metal(palette[colIndex], rand01()));
                else
                    mat = std::make_shared<Dielectric>(Dielectric(palette[colIndex], 1.3));
            }
            prims.push_back(std::make_shared<Sphere>(Sphere(
                randR, Vec3(randX, randR, randZ), mat)));
        }
    }
    operator std::shared_ptr<Geometry>() { return std::make_shared<RandomBalls>(*this); }
};
