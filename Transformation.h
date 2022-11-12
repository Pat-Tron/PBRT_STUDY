#pragma once
#include "utility.h"
#include <array>

struct Transformation {
    // Scale Translation RotationX RotationY RotationZ
    enum tfType { S, T, RX, RY, RZ };

    std::array<double, 16> components{
        1., 0., 0., 0.,
        0., 1., 0., 0.,
        0., 0., 1., 0.,
        0., 0., 0., 1.
    };

    Transformation() = default;
    Transformation(const std::array<double, 16> &c) : components(c) {}
    Transformation(tfType tt, double degree);  // rotationX, rotationY, rotationZ
    Transformation(tfType tt, double x, double y, double z);  // scale, translation

    double determinant() const;
    Transformation inverted() const;
    Transformation transposed() const;
    Transformation operator *(const Transformation &trans) const;
    Transformation operator /(double n) const;
    double &operator [](int i) { return components[i]; }
    friend std::ostream &operator<<(std::ostream &s, const Transformation &trans);
};