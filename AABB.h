#pragma once
#include "Ray.h"
#include <tuple>

struct AABB {
    // Acceleration algorithm: Axis-Aligned Bounding Box
    Vec3 minBound{ INFINITY, INFINITY, INFINITY };
    Vec3 maxBound{ -INFINITY, -INFINITY, -INFINITY };
    Vec3 center;
    double area{ 0.0 };
    double padding{ 0.00001 };

    AABB() = default;
    AABB(const Vec3 &min, const Vec3 &max, double pad = 0.00001);
    bool hit(const Ray &ray, double tMin, double tMax) const;
    std::tuple<double, double> hit(const Ray &ray) const;
    AABB operator+(const AABB &b) const;
    AABB &operator+=(const AABB &b);
    AABB operator*(const Transformation &trans) const;
    void expand() { minBound -= Vec3(padding); maxBound += Vec3(padding); }
    int longestAxis() const;
    friend std::ostream &operator<<(std::ostream &os, const AABB &ab);
};

inline AABB::AABB(const Vec3 &min, const Vec3 &max, double pad) :
    minBound(min), maxBound(max), padding(pad) {
    expand();
    center = (minBound + maxBound) * 0.5;
    // half area
    Vec3 len{ maxBound - minBound };
    area = len.x * len.y + len.y * len.z + len.z * len.x;
}

inline AABB AABB::operator+(const AABB &b) const {
    return AABB(minVec3(minBound, b.minBound), maxVec3(maxBound, b.maxBound));
}

inline AABB &AABB::operator+=(const AABB &b) {
    minBound = minVec3(minBound, b.minBound);
    maxBound = maxVec3(maxBound, b.maxBound);
    return *this;
}

inline int AABB::longestAxis() const {
    Vec3 len{ maxBound - minBound };
    double maxLen{ len.max() };
    for (int i{ 0 }; i < 3; ++i) if (len[i] == maxLen)return i;
}

inline std::ostream &operator<<(std::ostream &os, const AABB &ab) {
    os << "minBound: " << ab.minBound << ", " << "maxBound: " << ab.maxBound << std::endl;
    return os;
}