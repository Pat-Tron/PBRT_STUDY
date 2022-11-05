#include "Camera.h"
#include "utility.h"

void Camera::initialization() {
    // Right-hand coordinate system
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

Color Camera::render(const Ray &ray, const Primitives &prims) const {
    HitRec rec;
    static int depth{ 0 };
    if (prims.hit(ray, 0.0000001, 1e10, rec)) {
        if (depth < maxDepth) {
            ++depth;
            return render(rec.mat->scatter(ray, rec), prims) * rec.mat->albedo * rec.mat->reflectance;
        } else {
            depth = 0;
            return rec.mat->albedo;
        }
    } else {
        depth = 0;
        return background(ray, up);
    }
}

void Camera::randerLoop(const std::vector<primPointer> &prims) {
    initialization();

    Primitives Prims(prims);

    // Rendering loop
    std::cout << "Rendering start." << std::endl;
    double uStep{ 1.0 / resWidth / antialiasing };
    double vStep{ 1.0 / resHeight / antialiasing };

    for (int row{ 0 }; row < resHeight; ++row) {
        std::cout << "\rRendering ROW " << row + 1 << " of " << resHeight << " .";
        for (int col{ 0 }; col < resWidth; ++col) {
            double u{ 1.0 * col / resWidth };
            double v{ 1.0 * row / resHeight };

            for (int ui{ 0 }; ui < antialiasing; ++ui) {
                for (int vi{ 0 }; vi < antialiasing; ++vi) {
                    Ray r = getRay(u + ui * uStep, v + vi * vStep);
                    pixels[row][col] += render(r, Prims);
                }
            }

            pixels[row][col] /= antialiasing * antialiasing;
        }
    }
    std::cout << "\nRendering finished" << std::endl;
}