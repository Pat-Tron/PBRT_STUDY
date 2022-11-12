#pragma once
#include <cmath>
#include <iostream>
#include <array>
#include "Transformation.h"

struct Vec3 {
    double x{ 0.0 }, y{ 0.0 }, z{ 0.0 };

    Vec3() = default;
    Vec3(double _x, double _y, double _z) : x(_x), y(_y),z(_z) {}
    Vec3(double n) : x(n), y(n), z(n) {}
    Vec3(const Vec3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}
    
    Vec3 operator+() const { return *this; };
    Vec3 operator-() const { return Vec3(-x, -y, -z); };

    Vec3 operator+(const Vec3 &vec) const { return Vec3(x + vec.x, y + vec.y, z + vec.z); }
    Vec3 operator-(const Vec3 &vec) const { return Vec3(x - vec.x, y - vec.y, z - vec.z); }
    Vec3 operator/(const Vec3 &vec) const { return Vec3(x / vec.x, y / vec.y, z / vec.z); }
    Vec3 operator*(const double &n) const { return Vec3(x * n, y * n, z * n); }
    Vec3 operator/(const double &n) const { return Vec3(x / n, y / n, z / n); }

    double operator*(const Vec3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; }

    Vec3 &operator+=(const Vec3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vec3 &operator-=(const Vec3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vec3 &operator*=(const double &n) { x *= n; y *= n; z *= n; return *this; }
    Vec3 &operator/=(const double &n) { x /= n; y /= n; z /= n; return *this; }

    double operator[](int n) { return n == 0 ? x : (n == 1 ? y : z); }
    Vec3 &operator=(const Vec3 &vec) { x = vec.x; y = vec.y; z = vec.z; return *this; }
    Vec3 operator^(const Vec3 &vec) const;
    Vec3 &operator*=(const Transformation &trans);
    Vec3 operator*(const Transformation &trans) const;

    friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec);
    friend Vec3 operator*(const double &n, const Vec3 &vec);
    friend Vec3 minVec3(const Vec3 &v1, const Vec3 &v2);
    friend Vec3 maxVec3(const Vec3 &v1, const Vec3 &v2);

    double length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 &normalize() { *this *= 1.0 / length(); return *this; }
    Vec3 normalized() const { return *this * (1.0 / length());}
    Vec3 switchXZ() const { return Vec3(z, y, x); }
    double max(double n = 0.0) const { return std::max(std::max(std::max(x, y), z), n); }
    double min(double n = 0.0) const { return std::min(std::min(std::min(x, y), z), n); }
    Vec3 reciprocal() const { return Vec3(1.0 / x, 1.0 / y, 1.0 / z); }
    Vec3 vecFloor() const { return Vec3(floor(x), floor(y), floor(z)); }
};

inline Vec3 Vec3::operator^(const Vec3 &vec) const {
    // Cross
    return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

inline Vec3 &Vec3::operator*=(const Transformation &trans) {
    // Actually, it is LEFT multiplied by trans matrix, not right.
    auto &a = trans.components;
    double nx = a[0] * x + a[1] * y + a[2] * z + a[3];
    double ny = a[4] * x + a[5] * y + a[6] * z + a[7];
    double nz = a[8] * x + a[9] * y + a[10] * z + a[11];
    x = nx; y = ny; z = nz;
    return *this;
}

inline Vec3 Vec3::operator*(const Transformation &trans) const {
    // Actually, it is LEFT multiplied by trans matrix, not right.
    auto &a = trans.components;
    return Vec3(
        a[0] * x + a[1] * y + a[2] * z + a[3],
        a[4] * x + a[5] * y + a[6] * z + a[7],
        a[8] * x + a[9] * y + a[10] * z + a[11]
    );
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &vec) {
    os << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
    return os;
}

inline Vec3 operator*(const double &n, const Vec3 &vec) { return vec * n; }

inline Vec3 minVec3(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x > v2.x ? v2.x : v1.x, v1.y > v2.y ? v2.y : v1.y, v1.z > v2.z ? v2.z : v1.z);
}
inline Vec3 maxVec3(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x < v2.x ? v2.x : v1.x, v1.y < v2.y ? v2.y : v1.y, v1.z < v2.z ? v2.z : v1.z);
}

struct Vec2 {
    double u{ 0.0 }, v{ 0.0 };

    Vec2() = default;
    Vec2(double _u, double _v) : u(_u), v(_v) {}
    Vec2(const Vec2 &vec) : u(vec.u), v(vec.v) {}

    Vec2 operator+(const Vec2 &vec) const { return Vec2(u + vec.u, v + vec.v); }
    Vec2 operator-(const Vec2 &vec) const { return Vec2(u - vec.u, v - vec.v); }
    Vec2 operator*(const double &n) const { return Vec2(u * n, v * n); }
};