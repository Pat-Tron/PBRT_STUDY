#pragma once

#include "Ray.h"
#include "Shape.h"

enum PRESET { P1K, P2K, P4K };

struct Resolution {
    int width, height;
    Resolution(int w, int h) : width(w), height(h) {}
};

struct Camera {
    const std::array<Resolution, 3> presets{
        Resolution(1920, 1080),
        Resolution(2560, 1440),
        Resolution(3840, 2160)
    };
    // Right-hand coordinate system
    Vec3 orientation{ 0.0, 0.0, 1.0 };
    Vec3 position{ 0.0, 0.0, 0.0 };    
    int resWidth{ 500 };
    int resHeight{ 500 };
    double focal{ 5.0 };
    int antialiasing{ 2 };
    int maxDepth{ 5 };
    double filmWidth{ 1.0 };
    std::vector<std::vector<Color>> pixels;

    Camera() = default;
    Camera(int w, int h) : resWidth(w), resHeight(h),
        pixels(resHeight, std::vector<Color>(resWidth)) {}
    Camera(PRESET prst, double zoom = 1.0) :
        resWidth(presets[prst].width * zoom), resHeight(presets[prst].height * zoom),
        pixels(resHeight, std::vector<Color>(resWidth)) {}
    void faceAt(const Vec3 &target) { orientation = target - position; }
    void randerLoop(const Primitives &prims);

private:
    double filmHeight{ 0.0 };
    Vec3 leftDownCorner, right, up;
    void initialization();
    Color render(const Ray &ray, const Primitives &prims) const;
    Ray getRay(double u, double v);
};