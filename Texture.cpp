#include "Texture.h"

Color PerlinNoise::v(double u, double v, const Vec3 &p) const {
    Vec3 P{ (p + offset) * scale };
    /*
        Corner notation inside specific int lattice

                          ¡ü y
                         |
                        |
                       E--------F
                     /|       /|
                   / |      / |
                 H--------G  |
                |  A-----|--B--------¡ú x
               | /      | /
              |/       |/
             D--------C
           /
         ¡ý z

    */
    
    // 8 lattice corners
    Vec3 A{ P.vecFloor() };
    Vec3 B{ A + Vec3(1.0, 0.0, 0.0) };
    Vec3 C{ A + Vec3(1.0, 0.0, 1.0) };
    Vec3 D{ A + Vec3(0.0, 0.0, 1.0) };
    Vec3 E{ A + Vec3(0.0, 1.0, 0.0) };
    Vec3 F{ A + Vec3(1.0, 1.0, 0.0) };
    Vec3 G{ A + Vec3(1.0, 1.0, 1.0) };
    Vec3 H{ A + Vec3(0.0, 1.0, 1.0) };

    // dotX = XP * gradiant
    double dotA{ (P - A) * randomGradiant(A) };
    double dotB{ (P - B) * randomGradiant(B) };
    double dotC{ (P - C) * randomGradiant(C) };
    double dotD{ (P - D) * randomGradiant(D) };
    double dotE{ (P - E) * randomGradiant(E) };
    double dotF{ (P - F) * randomGradiant(F) };
    double dotG{ (P - G) * randomGradiant(G) };
    double dotH{ (P - H) * randomGradiant(H) };

    // lattice uvw
    double ul{ P.x - A.x }, vl{ P.y - A.y }, wl{ P.z - A.z };
    
    double interpolatedYA{ smoothstep(dotA, dotE, vl) };
    double interpolatedYB{ smoothstep(dotB, dotF, vl) };
    double interpolatedYC{ smoothstep(dotC, dotG, vl) };
    double interpolatedYD{ smoothstep(dotD, dotH, vl) };
    double interpolatedZAD{ smoothstep(interpolatedYA, interpolatedYD, wl) };
    double interpolatedZBC{ smoothstep(interpolatedYB, interpolatedYC, wl) };
    double final{ smoothstep(interpolatedZAD, interpolatedZBC, ul) };
    // final : [-sqrt(2)/2, -sqrt(2)/2], fit to [0, 1]
    return Color(final * 0.70711 + 0.5);
}
