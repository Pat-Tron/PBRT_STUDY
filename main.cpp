// Start with RayTracing in One Weekend.

#include "imageIO.h"
#include "Camera.h"
#include "Geometry.h"
#include "Transformation.h"

int main() {
    clock_t globalTimeStart = clock();

    Camera camera(1080, 1080);
    camera.position = Vec3(0, 5, 25.);
    camera.focal = 2.0;
    camera.antialiasing = 3;
    camera.maxDepth = 10;
    //camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt = Vec3(0.0, 5, 0.0);
    camera.NO_BG = true;
    //camera.motionBlur = true;
    

    camera.antialiasing = 30; camera.maxDepth = 20;
    
    using TF = Transformation;
    
    Geometry geos = Geometry() +
        Square(Lambertian(WHITE, 0.9), 10.1) +
        Square(Lambertian(WHITE, 0.9), 10.1) * (TF(TF::RX, 90) * TF(TF::T, 0, 5, -5)) +
        Square(Lambertian(WHITE, 0.9), 20) * TF(TF::RX, 180) * TF(TF::T, 0, 10, 0) +
        Square(Lambertian(GREEN, 0.9), 10.1) * TF(TF::RZ, -90) * TF(TF::T, -5, 5, 0) +
        Square(Lambertian(RED, 0.9), 10.1) * TF(TF::RZ, 90) * TF(TF::T, 5, 5, 0) +
        PrimBall(DiffuseLight(Color(9.0)), 20) * TF(TF::T, 0, 29.8, 0) +
        VolumeGeo(3.0, 3.0, 8.0, 0.5, ORANGE) * (TF(TF::RY, 40) * TF(TF::T, -2, 4, -1.5)) +
        VolumeGeo(4, 4, 4, 0.9, PerlinNoise(1.0, false, 5)) *
            (TF(TF::RX, 30) * TF(TF::RY, 30) * TF(TF::T, 0, 3.5, 2));

    auto pixels = camera.randerLoop(geos.prims);
    
    outputPic("image", PIC_FORMAT::QOI, pixels);
    timeInfo(globalTimeStart);
    return 0;
}