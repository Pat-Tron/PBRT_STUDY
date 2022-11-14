#include "Material.h"

std::tuple<Vec3, double> Material::randomSampleInHemiSphere(const Vec3 &normal, double range) const {
    // 单位半球面均匀撒点 https://zhuanlan.zhihu.com/p/340929847

    /*
        r0 = rand01(), r1 = rand01(). r0,r1: [0,1)
          phi = r0 * 2 * PI
        theta = acos(1 - r1)

        cos(phi) = cos(2*PI*r0)
        sin(phi) = sin(2*PI*r0)
        cos(theta) = 1 - r1
        sin(theta) = sqrt(1 - cos^2(theta)) = sqrt(2*r1-r1*r1)
            (For theta is on the interval of [0, PI /2], sin(theta)>=0.)
    */
    double r0{ rand01() }, r1{ rand01() };
    double phi{2.0 * PI * r0}, sinTheta{ sqrt((2.0 - r1) * r1) };
    Vec3 pos{ cos(phi) * sinTheta, 1 - r1, sin(phi) * sinTheta };

    if (fabs(normal.x) < 0.00001 && fabs(normal.z) < 0.00001) {
        // When normal almost pointing up(0, 1, 0)
        if (normal.y > 0.0) return std::make_tuple(pos, 1.0 - r1);
        // When normal almost pointing down(0, -1, 0)
        else return  std::make_tuple(-pos, 1.0 - r1);
    }

    // Coordinate transform
    //// NewY: normal
    //Vec3 newX{ (normal ^ Vec3(0.0, 1.0, 0.0)).normalized() };
    //Vec3 newZ{ (newX ^ normal).normalized() };
    //return std::make_tuple(newX * pos.x + normal * pos.y + newZ * pos.z, 1.0 - r1);

    // Quaternion rotation. Faster than coordinate transform
    double c{ normal.y }, s{ sqrt(1.0 - c * c) };  // c: cos, normal * up(0, 1, 0), s: sin
    Vec3 axis{ normal.z, 0.0, - normal.x };  // up(0, 1, 0) ^ normal
    axis.normalize();
    return std::make_tuple(pos * c + axis * (1 - c) * (axis * pos) + (axis ^ pos) * s, 1.0 - r1);
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