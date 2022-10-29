#pragma once

#include "Ray.h"

struct Camera {

    // Right-hand coordinate system
    Vec3 orientation{ 0.0, 0.0, 1.0 };
    Vec3 position{ 0.0, 0.0, 0.0 };
    int width{ 500 };
    int height{ 500 };
    double focal{ 5.0 };


    Camera() = default;
    Camera(int w, int h, double fs, double xs);

    Ray getRay(int x_index, int y_index);

private:
    double pixelSize;
    double xscale{ 2.0 };
    double yscale{ 0.0 };
    Vec3 leftDownCorner;

};