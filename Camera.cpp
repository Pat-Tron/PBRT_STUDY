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

Ray Camera::getRay(int x_index, int y_index) {
    // Indices are counted row by row, left to right, top to bottom.
    y_index = height - 1 - y_index;
    Vec3 target = leftDownCorner + Vec3(x_index * pixelSize, y_index * pixelSize, 0.0);
    return Ray(position, target - position);
}