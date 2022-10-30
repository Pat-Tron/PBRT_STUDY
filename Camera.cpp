#include "Camera.h"

Camera::Camera(int w, int h, double fs, double xs) :
    width(w), height(h), xscale(xs) {
    focal = fs * xscale;
    yscale = xscale * height / width;
    pixelSize = xscale / width;
    // Screen center: (0, 0, 0)
    leftDownCorner = Vec3(-xscale * 0.5, -yscale * 0.5, 0.0) + \
        Vec3(pixelSize * 0.5, pixelSize * 0.5, 0.0);
    position.z = -focal;
}

Ray Camera::getRay(double u, double v) {
    // Indices are counted row by row, left to right, top to bottom.
    v = 1.0 - v;
    Vec3 target = leftDownCorner + Vec3(xscale * u, yscale * v, 0.0);
    return Ray(position, target - position);
}