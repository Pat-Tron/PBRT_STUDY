#pragma once
//#include "Vector.h"
#include "utility.h"

struct Transformation {
    // Scale Translation RotationX RotationY RotationZ
    enum tfType { S, T, RX, RY, RZ };

    std::array<double, 12> components{
        1., 0., 0., 0.,
        0., 1., 0., 0.,
        0., 0., 1., 0.
    };

    Transformation() = default;
    Transformation(const std::array<double, 12> &c) : components(c) {}
    Transformation(tfType tt, double degree);  // rotationX, rotationY, rotationZ
    Transformation(tfType tt, double x, double y, double z);  // scale, translation

    Transformation operator *(const Transformation &trans) const;
    friend std::ostream &operator<<(std::ostream &s, const Transformation &trans);
};

inline Transformation::Transformation(tfType tt, double degree) {
    // Passed by degree, not radian
    degree = degree / 180. * PI;
    double s{ sin(degree) }, c{ cos(degree) };

    switch (tt) {
    case RX: components = { 1., 0., 0., 0., 0., c, -s, 0., 0., s, c, 0. }; break;
    case RY: components = { c, 0., s, 0., 0., 1., 0., 0., -s, 0., c, 0. }; break;
    case RZ: components = { c, -s, 0., 0., s, c, 0., 0., 0., 0., 1., 0. }; break;
    default: throw "Wrong transformation type!";
    }
}

inline Transformation::Transformation(tfType tt, double x, double y, double z) {
    // scale, translation
    switch (tt) {
    case S: components = { x, 0., 0., 0., 0., y, 0., 0., 0., 0., z, 0. }; break;
    case T: components = { 1., 0., 0., x, 0., 1., 0., y, 0., 0., 1., z }; break;
    default: throw "Wrong transformation type!";
    }
}

inline Transformation Transformation::operator *(const Transformation &trans) const {
    Transformation result;
    for (int row{ 0 }; row < 3; row++) {
        for (int column{ 0 }; column < 4; column++) {
            result.components[row * 4 + column] =
                components[row * 4 + 0] * trans.components[0 * 4 + column] +
                components[row * 4 + 1] * trans.components[1 * 4 + column] +
                components[row * 4 + 2] * trans.components[2 * 4 + column];
            if (column == 3) result.components[row * 4 + column] += components[row * 4 + 3];
        }
    }
    return result;
}

inline std::ostream &operator<<(std::ostream &s, const Transformation &trans) {
    for (int i{ 0 }; i < 3; i++) {
        s << "["
            << '\t' << trans.components[i * 4 + 0] << ", "
            << '\t' << trans.components[i * 4 + 1] << ", "
            << '\t' << trans.components[i * 4 + 2] << ", "
            << '\t' << trans.components[i * 4 + 3] << " ]\n";
    }
    return s;
}