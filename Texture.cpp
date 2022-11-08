#include "Texture.h"

Color PerlinNoise::v(double u, double v, const Vec3 &p) const {
    Vec3 P{ (p + offset) * scale };

    // Fractal, or turbulence
    double color{ plainPerlin(P) };
    for (int i = 1; i <= octaves; ++i) {
        color += pow(roughness, i) * plainPerlin(P * lacunarity * i);
    }

    // Fit to [-1, 1]
    // 2.5: ¾­ÑéÖµ
    color *= normalizeFactor * 2.5;

    // Clamp
    color = color < -1.0 ? -1.0 : (color <= 1.0 ? color : 1.0);
    
    if (fold) {
        color = color < 0.0 ? -color : color;
        return Color(color);
    } else return Color(color * 0.5 + 0.5);
}

double PerlinNoise::plainPerlin(const Vec3 &p) const {
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
    Vec3 A{ p.vecFloor() };
    Vec3 B{ A + Vec3(1.0, 0.0, 0.0) };
    Vec3 C{ A + Vec3(1.0, 0.0, 1.0) };
    Vec3 D{ A + Vec3(0.0, 0.0, 1.0) };
    Vec3 E{ A + Vec3(0.0, 1.0, 0.0) };
    Vec3 F{ A + Vec3(1.0, 1.0, 0.0) };
    Vec3 G{ A + Vec3(1.0, 1.0, 1.0) };
    Vec3 H{ A + Vec3(0.0, 1.0, 1.0) };

    // dotX = XP * gradiant
    double dotA{ (p - A) * randomGradiant(A) };
    double dotB{ (p - B) * randomGradiant(B) };
    double dotC{ (p - C) * randomGradiant(C) };
    double dotD{ (p - D) * randomGradiant(D) };
    double dotE{ (p - E) * randomGradiant(E) };
    double dotF{ (p - F) * randomGradiant(F) };
    double dotG{ (p - G) * randomGradiant(G) };
    double dotH{ (p - H) * randomGradiant(H) };

    // lattice uvw
    double ul{ p.x - A.x }, vl{ p.y - A.y }, wl{ p.z - A.z };

    double interpolatedYA{ smoothstep(dotA, dotE, vl) };
    double interpolatedYB{ smoothstep(dotB, dotF, vl) };
    double interpolatedYC{ smoothstep(dotC, dotG, vl) };
    double interpolatedYD{ smoothstep(dotD, dotH, vl) };
    double interpolatedZAD{ smoothstep(interpolatedYA, interpolatedYD, wl) };
    double interpolatedZBC{ smoothstep(interpolatedYB, interpolatedYC, wl) };
    double final{ smoothstep(interpolatedZAD, interpolatedZBC, ul) };

    // final : [-sqrt(2)/2, -sqrt(2)/2], fit to [-1, 1]
    return final * 1.414;
}