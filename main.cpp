// Start with RayTracing in One Weekend.

#include "imageIO.h"
#include "Camera.h"
#include "Geometry.h"
#include "Transformation.h"

int main() {
    clock_t globalTimeStart = clock();

    Camera camera(1080, 1080, 1);
    camera.position = Vec3(0, 5, 25.);
    camera.focal = 2.0;
    camera.antialiasing = 2;
    camera.maxDepth = 2;
    //camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt = Vec3(0.0, 5, 0.0);
    camera.NO_BG = true;
    //camera.motionBlur = true;
    

    camera.antialiasing = 20; camera.maxDepth = 20;
    
    using TF = Transformation;
    Geometry geos = Geometry() +
        Square(Lambertian(WHITE, 0.9), 10) * TF(TF::T, 0, 0, 0) +
        Square(Lambertian(WHITE, 0.9), 10) * TF(TF::RX, 90) * TF(TF::T, 0, 5, -5) +
        Square(Lambertian(WHITE, 0.9), 10) * TF(TF::RX, 180) * TF(TF::T, 0, 10, 0) +
        Square(Lambertian(GREEN, 0.9), 10) * TF(TF::RZ, -90) * TF(TF::T, -5, 5, 0) +
        Square(Lambertian(RED, 0.9), 10) * TF(TF::RZ, 90) * TF(TF::T, 5, 5, 0) +
        PrimBall(DiffuseLight(Color(10.0)), 10) * TF(TF::T, 0, 19.8, 0) +
        Cuboid(Lambertian(ORANGE), 3,   5) * TF(TF::RY,  15) * TF(TF::T, -2.5, 0, -1) +
        Cuboid(Lambertian(PURPLE), 3.3, 8) * TF(TF::RY, -30) * TF(TF::T, 2.5, 0, -1) +
        Cuboid(Dielectric((Color(BLUE) * 1.5).clamp()), 1.5) * TF(TF::RY, 60) * TF(TF::RX, 45) * TF(TF::T, -2.3, 6, -2);
    
    auto pixels = camera.randerLoop(geos.prims);
    
    outputPic("image", PIC_FORMAT::QOI, pixels);

    timeInfo(globalTimeStart);
    return 0;
}