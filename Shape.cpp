#include "Shape.h"
#include <numeric>

double Primitive::timeStart = 0.0;
double Primitive::timeEnd = 0.0;

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
    Vec3 tmpVec1{ BA.x * OA.y - OA.x * BA.y, OA.x * BA.z - BA.x * OA.z, BA.y * OA.z - OA.y * BA.z };
    Vec3 tmpVec2{ CA.y * D.z - D.y * CA.z, D.x * CA.z - CA.x * D.z, CA.x * D.y - D.x * CA.y };
    double determinant{ BA * tmpVec2 };

    double t{ -(CAswitchXZ * tmpVec1) / determinant };
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

BVH::BVH(std::vector<primPointer> &prims, itrt start, itrt end) {
    auto primCount{ end - start };
    
    if (primCount == 1) {
        // Subtree with one leaf node
        left = right = *start;
        box = left->box;
    }
    else if (primCount == 2) {
        // Subtree with two leaf node
        left = *start;
        right = *(start + 1);
        box = left->box + right->box;
    }
    else {
        // Build current BVH node box
        using pP = primPointer;
        auto pointerBoxSum = [](AABB &a, pP b) -> AABB { return a + b->box; };
        box = std::accumulate(start, end, box, pointerBoxSum);

        // Choose which axis used for sorting as ref.(longestAxis)
        int axis{ box.longestAxis() };
        auto pointerCompare = [axis](pP a, pP b) -> bool { return a->centroid[axis] < b->centroid[axis]; };
        std::sort(start, end, pointerCompare);

        if (primCount == 3 || primCount == 4) {
            // Nodes less than 4 would produce huge waste, so no need to use SAH
            left = std::make_shared<BVH>(prims, start, start + 2);
            right = std::make_shared<BVH>(prims, start + 2, end);
        } else {
            /*
                SAH: Surface Area Heuristic
                When evaluating potential partitions, the one that minimized
                the surface area of the sum of volumes of the sub-trees is
                almost always good.
                https://www.cnblogs.com/lookof/p/3546320.html
                http://15462.courses.cs.cmu.edu/fall2015/lecture/acceleration/slide_024

                cost(A,B)   time cost in subtree A and B
                    = p(A) * ¡Æt(i) in A  + p(B) * ¡Æt(j) in B
                    = p(A) * n in A  + p(B) * m in B
                    p(x): probability of hiting subtree x's AABB
                    t(x): time cost in calulating hitting leaf node which assumed as constant
            */

            double minSAH{ INFINITY };
            int bestIndex{ 0 };

            // Prestore all "primCount-1" subtree-area possibilities of tree partition.
            std::vector<double> areas(primCount), leftArea(primCount), rightArea(primCount);
            std::transform(start, end, areas.begin(), [](pP a) -> double { return a->box.area; });
            // partial_sum: (1, 2, 3, 4) to (1, 3, 6, 10), or (1, 1, 1, 1) to (1, 2, 3, 4)
            std::partial_sum(areas.begin(), areas.end(), leftArea.begin());
            std::partial_sum(areas.rbegin(), areas.rend(), rightArea.rbegin());
            // Find which index has the best partition according to SAH.
            for (int i = 0; i < primCount - 1; ++i) {
                double currentTreeCost = (i + 1) * leftArea[i] + (primCount - i - 1) * rightArea[i + 1];
                if (currentTreeCost < minSAH) {
                    minSAH = currentTreeCost;
                    bestIndex = i;
                }
            }
            // Build subtree
            left = std::make_shared<BVH>(prims, start, start + bestIndex + 1);
            right = std::make_shared<BVH>(prims, start + bestIndex + 1, end);
        }
    }
}


bool BVH::hit(const Ray &ray, double tMin, double tMax, HitRec &rec) const {
    if (box.hit(ray, tMin, tMax)) {
        bool lHit{ left->hit(ray, tMin, tMax, rec) };
        if (left == right) return lHit;
        else {
            tMax = lHit ? rec.t : tMax;
            bool rHit{ right->hit(ray, tMin, tMax, rec) };
            return lHit || rHit;
        }
    } else return false;
}

void BVH::printSelf() const {
    static int depth{ 0 };

    for (int i = 0; i < depth; ++i) std::cout << "   ";
    if (std::string(typeid(*left).name()) != std::string("struct BVH")) {
        std::cout << "L: " << left << ' ';
        left->printSelf();
        std::cout << std::endl;
    } else {
        std::cout << "L: " << left << "\n";
        depth++;
        left->printSelf();
        depth--;
    }


    for (int i = 0; i < depth; ++i) std::cout << "   ";
    if (std::string(typeid(*right).name()) != std::string("struct BVH")) {
        std::cout << "R: " << right << ' ';
        right->printSelf();
        std::cout << std::endl;
    } else {
        std::cout << "R: " << right << "\n";
        depth++;
        right->printSelf();
        depth--;
    }
}