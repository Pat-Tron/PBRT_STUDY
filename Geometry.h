#pragma once

#include "Primitive.h"
#include "Transformation.h"

struct Geometry {
    std::vector<primPointer> prims;
    Geometry() = default;
    Geometry(const std::vector<primPointer> &psp) : prims(psp) {}

    template <typename geoType>
    Geometry &operator+(geoType geo) {
        for (auto primp : geo.prims) prims.push_back(primp);
        return *this;
    }

    Geometry &operator*(const Transformation &trans) {
        for (auto primp : prims) primp->transform(trans);
        return *this;
    }
};

struct Square : public Geometry {
    // Default center at: (0, 0, 0)
    // Default normal: (0, 1, 0)
    double xLen, zLen;
    Square() = default;
    template <typename MaterialType>
    Square(const MaterialType &mat, double xl, double zl = 0.0) :
        xLen(xl), zLen(zl ? zl : xl) {
        // counterclock wise
        Vec3 A { Vec3( xLen * 0.5, 0.0,  zLen * 0.5)};
        Vec3 B { Vec3( xLen * 0.5, 0.0, -zLen * 0.5)};
        Vec3 C { Vec3(-xLen * 0.5, 0.0, -zLen * 0.5)};
        Vec3 D { Vec3(-xLen * 0.5, 0.0,  zLen * 0.5)};
        Vec2 uvA(1.0, 0.0), uvB(1.0, 1.0), uvC(0.0, 1.0), uvD(0.0, 0.0);
        prims.push_back(std::make_shared<Triangle>(Triangle(A, B, D, uvA, uvB, uvD, mat)));
        prims.push_back(std::make_shared<Triangle>(Triangle(B, C, D, uvB, uvC, uvD, mat)));
    }
};

struct Cuboid : public Geometry {
    // Default bottom center at: (0, 0, 0)
    double xLen, zLen, height;
    Cuboid() = default;
    template <typename MaterialType = Lambertian>
    Cuboid(const MaterialType &mat, double xl, double h = 0., double zl = 0.) :
        xLen(xl), zLen(zl ? zl : xl), height(h ? h : xl) {
        using TF = Transformation;
        Geometry container = Geometry() +
            Square(mat, xLen, zLen) * TF(TF::RX, 180) +                                                 // bottom
            Square(mat, xLen, zLen) * TF(TF::T, 0, height, 0) +                                         // up
            Square(mat, xLen, height) * TF(TF::RX,  90) * TF(TF::T, 0, height * 0.5, zLen *  0.5) +     // front
            Square(mat, xLen, height) * TF(TF::RX, -90) * TF(TF::T, 0, height * 0.5, zLen * -0.5) +     // back
            Square(mat, height, zLen) * TF(TF::RZ,  90) * TF(TF::T, xLen * -0.5, height * 0.5, 0) +     // left
            Square(mat, height, zLen) * TF(TF::RZ, -90) * TF(TF::T, xLen *  0.5, height * 0.5, 0);      // right
        prims = container.prims;
    }
};

struct PrimBall : public Geometry {
    PrimBall() = default;
    template <typename MaterialType>
    PrimBall(const MaterialType &mat, double r, Vec3 v = Vec3()) {
        prims.push_back(std::make_shared<Sphere>(Sphere(r, mat, v)));
    }
};

struct RandomBalls : public Geometry {
    // TODO: Adaptation of random square. This version is too old to generate ball at random square.

    std::vector<int>palette {
        0xb6bce2, 0x5f608c, 0x228867, 0x50d7b3, 0xc6cedf,
        0xefeada, 0xfcd75d, 0x4eb8ba, 0xf1503d, 0xf08484        
    };

    RandomBalls() = default;
    template <typename MaterialType>
    RandomBalls(
        int count, double radius, const Square &square,
        const MaterialType &mat = 0, double bias = 0.5) {
        bool hasUserSpecializedMaterial{ typeid(mat) != typeid(int) };

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
};
