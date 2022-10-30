#include "Shape.h"

bool Primitives::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    HitRec tempRec;
    bool hitOrNot{ false };
    double closestSoFar{ tMax };

    for (size_t i{ 0 }; i < primAmounts; ++i) {
        if (prims[i]->hit(ray, tMin, closestSoFar, tempRec)) {
            hitOrNot = true;
            closestSoFar = rec.t;
            rec = tempRec;
        }
    }
    return hitOrNot;
}

bool Sphere::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    /*
        p(t): point at Ray of parameter "t"
            -> p(t) = o + td

        equation: (p(t) - center)^2 = R^2
            -> (o + td - center)^2 = R^2
            -> t^2*d^2 + 2*t*(d*(o-center)) + (o-center)^2 - R^2 = 0
            -> delta(discriminant) = b^2 - 4ac
    */
    Vec3 co{ ray.origin - center };
    const Vec3 &d = ray.direction;
    double a{ d * d }, b{ co * d }, c{ co * co - radius * radius };
    double discriminant = b * b - a * c;

    if (discriminant < 0.0) return false;
    else {
        double root{ (-b - sqrt(discriminant)) / a };
        if (root < tMax && root > tMin) {
            rec.t = root;
            rec.p = ray.pointAtT(root);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        root = (-b + sqrt(discriminant)) / a;
        if (root < tMax && root > tMin) {
            rec.t = root;
            rec.p = ray.pointAtT(root);
            rec.normal = (rec.p - center) / radius;
            return true;
        } else return false;
    }
}
