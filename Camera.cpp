#include "Camera.h"
#include "utility.h"
#include <omp.h>

void Camera::initialization() {
    // Right-hand coordinate system
    orientation = faceAt - position;
    distanceToFocus = orientation.length() * defocusScale;

    orientation.normalize();
    Vec3 globalUp{ 0.0, 1.0, 0.0 };
    right = (orientation ^ globalUp).normalized();
    up = (right ^ orientation).normalized();
    Vec3 filmCenter;
    if (aperture >= 0.0) {
        // defocus setting
        lensRadius = aperture * 0.5;
        filmWidth *= distanceToFocus / focal;
        focal = distanceToFocus;
    }
    filmCenter = position + orientation * focal;
    filmHeight = filmWidth * resHeight / resWidth;
    leftDownCorner = filmCenter - right * filmWidth * 0.5 - up * filmHeight * 0.5;

    timeIntervel = timeEnd - timeStart;
    Primitive::timeStart = timeStart;
    Primitive::timeEnd = timeEnd;
    Primitive::motionBlur = motionBlur;
}

Vec3 Camera::sampleInCircle() {
    double radius{ rand01() };
    double angle{ rand01() * 2.0 * PI };
    return (right * cos(angle) + up * sin(angle)) * radius * lensRadius;
}


Ray Camera::getRay(double u, double v) {
    // Indices are counted row by row, left to right, top to bottom.
    v = 1.0 - v;
    Vec3 target = leftDownCorner + u * filmWidth * right + v * filmHeight * up;

    Vec3 newP{ position };
    if(aperture >= 0.0) newP += sampleInCircle();
    if (motionBlur) return Ray(newP, target - newP, timeStart + rand01() * timeIntervel);
    else return Ray(newP, target - newP);
}

Color Camera::render(const Ray &ray, const BVH &bvh, int depth) const {
    HitRec rec;
    if (bvh.hit(ray, 0.0000001, 1e10, rec)) {
        Color albedo{ rec.mat->texture->v(rec.uv, rec.p) };
        if (rec.mat->LIGHT) return albedo; 
        else if (depth < maxDepth) {
            ++depth;
            return render(rec.mat->scatter(ray, rec), bvh, depth) * albedo * rec.mat->reflectance;
        } else return Color();
    } else return background(ray);
}

const std::vector<std::vector<Color>> &Camera::randerLoop(const std::vector<primPointer> &constPrims) {
    initialization();

    std::vector<primPointer> prims{ constPrims };
    for (auto primp : prims) primp->makeAABB();
    BVH bvh{ prims, prims.begin(), prims.end()};
    //std::cout << "BVH tree:\n" << std::endl;
    //bvh.printSelf();

    // Rendering loop
    std::cout << "\nRendering start." << std::endl;
    double uStep{ 1.0 / resWidth / antialiasing };
    double vStep{ 1.0 / resHeight / antialiasing };

    //Color result;
    omp_set_num_threads(15);
#pragma omp parallel for schedule(dynamic, 1) // OpenMP
    for (int row{ 0 }; row < resHeight; ++row) {
        std::cout << "Rendering ROW " << row + 1 << " of " << resHeight
            << " . Thread: " << omp_get_thread_num() << std::endl;
        for (int col{ 0 }; col < resWidth; ++col) {
            double u{ 1.0 * col / resWidth };
            double v{ 1.0 * row / resHeight };

            Color result;
            for (int ui{ 0 }; ui < antialiasing; ++ui) {
                for (int vi{ 0 }; vi < antialiasing; ++vi) {
                    Ray r = getRay(u + ui * uStep, v + vi * vStep);
                    result += render(r, bvh);
                }
            }

            pixels[row][col] = (result / antialiasing / antialiasing).clamp();
        }
    }
    std::cout << "\nRendering finished" << std::endl;
    return pixels;
}