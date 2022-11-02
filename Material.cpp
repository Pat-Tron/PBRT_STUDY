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

double Dielectric::schlick(double cosine) const {
    // reflectivity that varies with angle
    // simple polynomial approximation by Christophe Schlick
    double r0{ (1.0 - IOR) / (1.0 + IOR) }; r0 *= r0;
    return r0 + (1.0 - r0) * pow(1 - cosine, 5);
}

Vec3 Dielectric::refract(const Vec3 &dirIn, const Vec3 &normal) const {
    // Snell's law: n1 * sin o1 = n2 * sin o2
    double cosineIn{ dirIn * normal };  // 入射角的cos

    if (cosineIn <= 0.0) {
        // Outside of dielectric
        double cosineOut{ sqrt(1.0 - IORR * IORR * (1.0 - cosineIn * cosineIn)) };

        // reflectivity
        double reflectProb{ schlick(-cosineIn) };
        if(rand01() < reflectProb) return reflect(dirIn, normal);
        else return IORR *(dirIn - cosineIn * normal) - normal * cosineOut;
    }
    else {
        // Inside of dielectric
        // Discriminant could be more COMPUTATIONAL COMPLEX : acos(cosineIn) >= criticalAngle
        double discriminant{ 1.0 - IOR * IOR * (1.0 - cosineIn * cosineIn) };
        // Total reflection: Exit angle have no real solution
        if (discriminant <= 0.0) return reflect(dirIn, -normal);
        // Half reflection
        else {
            // reflectivity
            double reflectProb{ schlick(IOR * cosineIn) };
            if (rand01() < reflectProb) return reflect(dirIn, -normal);
            else return IOR * (dirIn - cosineIn * normal) + normal * sqrt(discriminant);
        }
    }
}