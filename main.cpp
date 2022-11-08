// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Geometry.h"
#include <ctime>

int main() {
    clock_t globalTimeStart = clock();

    Camera camera(P1K, 0.5);
    camera.position = Vec3(20, 4.5, -2);
    camera.focal = 3.0;
    camera.antialiasing = 5;
    camera.maxDepth = 5;
    camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt = Vec3(0.0, 0.8, 0.0);
    camera.motionBlur = true;
    
    Camera cameraTest(P1K, 0.5);
    cameraTest.position = Vec3(0, 30, 10);
    cameraTest.focal = 3.0;
    
    auto ground = Square(Lambertian(BLUE), 7);

    GeometryContainer geos = GeometryContainer() + ground +
        PrimBall(Lambertian(YELLOW),        1.0, Vec3(-1.5, 1, 1.5)) +
        PrimBall(Metal(RED),                1.0, Vec3(0.0, 1, 0.0)) +
        PrimBall(Metal(GREEN, 0.1),         1.0, Vec3(1.5, 1, -1.5)) +
        PrimBall(Dielectric(WHITE, 1.4),    0.7, Vec3(2, 0.7, 0.8)) +
        PrimBall(Lambertian(PURPLE),        0.2, Vec3(2.5, 0.2, -1.8)) +
        PrimBall(Dielectric(PINK, 1.3),     0.3, Vec3(2, 0.3, -0.5)) +
        PrimBall(Metal(LIGHT_GREEN),        0.4, Vec3(-3, 0.4, 3)) +
        PrimBall(Metal(ORANGE),             0.4, Vec3(0.8, 0.4, -2.7)) +
        RandomBalls(50, 0.2, ground);

    CheckerTexture checker(ConstantTexture(PURPLE), ConstantTexture(YELLOW), 0.5);
    PerlinNoise noise(1.0);
    auto checkerGround = Square(Lambertian(noise), 6);
    GeometryContainer geos2 = GeometryContainer() + checkerGround;
    
    cameraTest.randerLoop(geos2.prims);
    //camera.randerLoop(geos2.prims);

    clock_t globalTimeEnd = clock();
    clock_t seconds{ globalTimeEnd - globalTimeStart };
    std::cout << "\nTotal time spending: "
        << seconds / 60 / 1000 << "m "
        << seconds / 1000 % 60 + (seconds % 1000 / 1000.0) << "s.\n\n";

    // Output
    outputPic("image", PIC_FORMAT::QOI, cameraTest.pixels);
    return 0;
}