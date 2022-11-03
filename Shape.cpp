#include "Shape.h"

bool Primitives::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    HitRec tempRec;
    bool hitOrNot{ false };
    double closestSoFar{ tMax };

    for (size_t i{ 0 }; i < primAmounts; ++i) {
        if (prims[i]->hit(ray, tMin, closestSoFar, tempRec)) {
            hitOrNot = true;
            rec = tempRec;
            if(rec.t < closestSoFar) closestSoFar = rec.t;
        }
    }
    return hitOrNot;
}

bool Sphere::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    // Motion blur
    Vec3 actualCenter;
    if (moving) actualCenter = center + velocity * ray.time;
    else actualCenter = center;

    /*
        p(t): point at Ray of parameter "t"
            -> p(t) = o + td

        equation: (p(t) - center)^2 = R^2
            -> (o + td - center)^2 = R^2
            -> t^2*d^2 + 2*t*(d*(o-center)) + (o-center)^2 - R^2 = 0
            -> delta(discriminant) = b^2 - 4ac
    */
    Vec3 co{ ray.origin - actualCenter };
    const Vec3 &d = ray.direction;
    double a{ d * d }, b{ co * d }, c{ co * co - radius * radius };
    double discriminant = b * b - a * c;

    if (discriminant < 0.0) return false;
    else {
        double root{ (-b - sqrt(discriminant)) / a };
        if (root > tMax) return false;
        else if (root < tMin) {
            root = (-b + sqrt(discriminant)) / a;
            if (root > tMax || root < tMin) return false;
        }
        rec.t = root;
        rec.p = ray.pointAtT(root);
        rec.normal = (rec.p - actualCenter) / radius;
        rec.mat = mat;
        return true;
    }
}

bool Triangle::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    /*
        Computational process : Fundamentals of Computer Graphics, p88
        Solve equation: Ray = o + td = f(beta, gamma) = A + beta*AB + gamma*AC
        alpha(=1-beta-gamma), beta, gamma are barycentric coordinates of triangle
        if alpha, beta, gamma all in [0, 1] and alpha + beta +gamma == 1, then HIT
    */ 
    Vec3 OA{ A - ray.origin };
    const Vec3 &D{ ray.direction };
    Vec3 tmpVec1{ AB.x * OA.y - OA.x * AB.y, OA.x * AB.z - AB.x * OA.z, AB.y * OA.z - OA.y * AB.z };
    Vec3 tmpVec2{ AC.y * D.z - D.y * AC.z, D.x * AC.z - AC.x * D.z, AC.x * D.y - D.x * AC.y };
    double determinant{ AB * tmpVec2 };

    double t{ -(ACswitchXZ * tmpVec1) / determinant };
    if (t < tMin || t > tMax) return false;
    double gamma{ D.switchXZ() * tmpVec1 / determinant };
    if (gamma < 0 || gamma > 1) return false;
    double beta{ OA * tmpVec2 / determinant };
    // Due to accuracy problem, beta + gamma may slightly bigger than 1, when ray exactly hit an edge.
    if (beta < 0 || beta + gamma > 1.00000001) return false;

    rec.t = t;
    rec.p = ray.pointAtT(t);
    rec.normal = normal;
    rec.mat = mat;
    return true;
}