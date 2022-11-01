#include "Material.h"

Vec3 Material::randomSampleInHemiSphere(const Vec3 &normal, bool uniform, double range) const {
    // 单位半球面均匀撒点 https://zhuanlan.zhihu.com/p/340929847
    float phi = rand01() * 2.0 * PI;
    float theta = (uniform ? acos(1.0 - rand01()) : rand01() * PI * 0.5) * range;
    Vec3 pos{ sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi) };

    // quaternion rotation
    Vec3 up{ 0.0, 1.0, 0.0 };
    double angle{ acos(normal * up) };
    Vec3 axis{ (up ^ normal).normalized() };
    double c{ cos(angle) };
    return pos * c + axis * (1 - c) * (axis * pos) + (axis ^ pos) * sin(angle);
}