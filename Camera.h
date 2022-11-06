#pragma once

#include "Ray.h"
#include "Primitive.h"

enum PRESET { P1K, P2K, P4K };

struct Resolution {
    int width, height;
    Resolution(int w, int h) : width(w), height(h) {}
};

struct Camera {
    // Film
    // TODO: tuple
    const std::array<Resolution, 3> presets{
        Resolution(1920, 1080),
        Resolution(2560, 1440),
        Resolution(3840, 2160)
    };
    Vec3 orientation{ 0.0, 0.0, 1.0 };
    Vec3 position{ 0.0, 0.0, 0.0 };    
    int resWidth{ 500 };
    int resHeight{ 500 };

    // Lens
    double focal{ 5.0 };
    double aperture{ 0.0 };  // diameter
    double defocusScale{ 1.0 };

    // Render
    int antialiasing{ 2 };
    int maxDepth{ 5 };

    // Motion blur
    bool motionBlur{ false };
    double FPS{ 30.0 };
    double timeStart{ 0.0 }, timeEnd{ 1.0 / FPS }, timeIntervel{ 0.0 };

    std::vector<std::vector<Color>> pixels;

    Camera() = default;
    Camera(int w, int h) : resWidth(w), resHeight(h),
        pixels(resHeight, std::vector<Color>(resWidth)) {}
    Camera(PRESET prst, double zoom = 1.0) :
        resWidth(static_cast<int>(presets[prst].width * zoom)),
        resHeight(static_cast<int>(presets[prst].height * zoom)),
        pixels(resHeight, std::vector<Color>(resWidth)) {}
    void faceAt(const Vec3 &target) {
        orientation = target - position;
        distanceToFocus = orientation.length() * defocusScale;
    }
    void randerLoop(const std::vector<primPointer> &constPrims);

private:
    double filmWidth{ 1.0 };
    double filmHeight{ 0.0 };
    double distanceToFocus{ 0.0 };
    double lensRadius{ 0.0 };
    Vec3 leftDownCorner, right, up;
    void initialization();
    Vec3 sampleInCircle();
    Color render(const Ray &ray, const BVH &bvh) const;
    Ray getRay(double u, double v);
};