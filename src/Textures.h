//
//  Textures.h
//  Raytracer
//

#ifndef Textures_h
#define Textures_h

#include "glm/glm.hpp"

glm::vec3 checkerboardTexture(glm::vec2 uv) {
    float u = uv.s;
    float v = uv.t;

    int bw = (int)(floor(30 * u) + floor(30 * v)) % 2;

    if (bw == 0) {
        return glm::vec3(0.0);
    }
    
    return glm::vec3(1.0);
}

glm::vec3 rainbowTexture(glm::vec2 uv) {
    float u = 0.5 + uv.s;
    float v = 0.5 - uv.t;

    int rainbow = (int)(-60 * (v * u)) % 3;

    if (rainbow == 0) {
        return glm::vec3(1.0, 0.0, 0.0);
    } else if (rainbow == 1) {
        return glm::vec3(0.0, 1.0, 0.0);
    }

    return glm::vec3(0.0, 0.0, 1.0);
}

#endif /* Textures_h */
