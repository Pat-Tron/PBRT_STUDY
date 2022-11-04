#pragma once
#include <cmath>
#include <iostream>
#include <array>

struct Vec3 {
    std::array<double, 3> v{ 0.0, 0.0, 0.0 };
    double &x{ v[0] }, &y{ v[1] }, &z{ v[2] };

    Vec3() = default;
    Vec3(double _x, double _y, double _z) : v({_x, _y, _z}) {}
    Vec3(double n) : v({n, n, n}) {}
    Vec3(const Vec3 &vec) : v(vec.v) {}
    
    Vec3 operator+() const { return *this; };
    Vec3 operator-() const { return Vec3(-x, -y, -z); };

    Vec3 operator+(const Vec3 &vec) const { return Vec3(x + vec.x, y + vec.y, z + vec.z); }
    Vec3 operator-(const Vec3 &vec) const { return Vec3(x - vec.x, y - vec.y, z - vec.z); }
    double operator*(const Vec3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; }
    Vec3 operator*(const double &n) const { return Vec3(x * n, y * n, z * n); }
    Vec3 operator/(const double &n) const { return Vec3(x / n, y / n, z / n); }

    Vec3 &operator+=(const Vec3 &vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vec3 &operator-=(const Vec3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vec3 &operator*=(const double &n) { x *= n; y *= n; z *= n; return *this; }
    Vec3 &operator/=(const double &n) { x /= n; y /= n; z /= n; return *this; }

    Vec3 &operator=(const Vec3 &vec) { x = vec.x; y = vec.y; z = vec.z; return *this; }
    const double &operator[](int i) const { return v[i]; }
    Vec3 operator^(const Vec3 &vec) {
        // Cross
        return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }
    friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec);
    friend Vec3 operator*(const double &n, const Vec3 &vec);
    friend Vec3 minVec3(const Vec3 &v1, const Vec3 &v2);
    friend Vec3 maxVec3(const Vec3 &v1, const Vec3 &v2);

    double length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 &normalize() { *this *= 1.0 / length(); return *this; }
    Vec3 normalized() const { return *this * (1.0 / length());}
    Vec3 switchXZ() const { return Vec3(z, y, x); }
};

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