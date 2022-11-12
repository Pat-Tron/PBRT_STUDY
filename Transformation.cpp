#pragma once
#include "Transformation.h"

Transformation::Transformation(tfType tt, double degree) {
    // Passed by degree, not radian
    degree = degree / 180. * PI;
    double s{ sin(degree) }, c{ cos(degree) };

    switch (tt) {
    case RX: components = { 1., 0., 0., 0., 0., c, -s, 0., 0., s, c, 0., 0., 0., 0., 1. }; break;
    case RY: components = { c, 0., s, 0., 0., 1., 0., 0., -s, 0., c, 0., 0., 0., 0., 1. }; break;
    case RZ: components = { c, -s, 0., 0., s, c, 0., 0., 0., 0., 1., 0., 0., 0., 0., 1. }; break;
    default: throw "Wrong transformation type!";
    }
}

Transformation::Transformation(tfType tt, double x, double y, double z) {
    // scale, translation
    switch (tt) {
    case S: components = { x, 0., 0., 0., 0., y, 0., 0., 0., 0., z, 0., 0., 0., 0., 1. }; break;
    case T: components = { 1., 0., 0., x, 0., 1., 0., y, 0., 0., 1., z, 0., 0., 0., 1. }; break;
    default: throw "Wrong transformation type!";
    }
}

Transformation Transformation::operator * (const Transformation &trans) const {
    Transformation result;
    for (int row{ 0 }; row < 3; row++) {
        for (int column{ 0 }; column < 4; column++) {
            result.components[row * 4 + column] =
                trans.components[row * 4 + 0] * components[0 * 4 + column] +
                trans.components[row * 4 + 1] * components[1 * 4 + column] +
                trans.components[row * 4 + 2] * components[2 * 4 + column];
            if (column == 3) result.components[row * 4 + column] += trans.components[row * 4 + 3];
        }
    }
    return result;
}

Transformation Transformation::operator / (double n) const {
    Transformation result;
    for (int row{ 0 }; row < 3; row++) {
        for (int column{ 0 }; column < 4; column++) {
            result.components[row * 4 + column] = components[row * 4 + column] / n;
        }
    }
    return result;
}

std::ostream &operator<<(std::ostream &s, const Transformation &trans) {
    for (int i{ 0 }; i < 4; i++) {
        s << "["
            << '\t' << trans.components[i * 4 + 0] << ", "
            << '\t' << trans.components[i * 4 + 1] << ", "
            << '\t' << trans.components[i * 4 + 2] << ", "
            << '\t' << trans.components[i * 4 + 3] << " ]\n";
    }
    return s;
}

Transformation Transformation::transposed() const {
    auto &cp = components;
    return Transformation({
        cp[0], cp[4], cp[8], cp[12],
        cp[1], cp[5], cp[9], cp[13],
        cp[2], cp[6], cp[10], cp[14],
        cp[3], cp[7], cp[11], cp[15]
        });
}

double Transformation::determinant() const {
    auto &cp = components;
    return cp[0] * (cp[5] * cp[10] - cp[6] * cp[9]) -
        cp[1] * (cp[4] * cp[10] - cp[6] * cp[8]) +
        cp[2] * (cp[4] * cp[9] - cp[5] * cp[8]);
}

Transformation Transformation::inverted() const {
    double dt{ determinant() };
    if (dt == 0.) throw "Transformation is singular (zero determinant).";
    Transformation trans;

    for (int row{ 0 }; row < 4; row++) {
        for (int column{ 0 }; column < 4; column++) {
            double m[9];
            for (int i{ 0 }; i < 3; i++) {
                for (int j{ 0 }; j < 3; j++) {
                    m[i * 3 + j] = components[(i < row ? i : i + 1) * 4 + (j < column ? j : j + 1)];
                }
            }
            double mDeterminant = m[0] * (m[4] * m[8] - m[5] * m[7]) -
                m[1] * (m[3] * m[8] - m[5] * m[6]) +
                m[2] * (m[3] * m[7] - m[4] * m[6]);
            // 行号列号同奇同偶则余子式为正，反之为负（异或）
            trans.components[row * 4 + column] = ((row & 1) ^ (column & 1) ? -1 : 1) * mDeterminant;
        }
    }
    return trans.transposed() / dt;
}