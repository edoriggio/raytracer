#include "../../lib/glm.hpp"

#ifndef Textures_h
#define Textures_h

/**
 * @brief Checkerboard texture
 * 
 * This function returns the color for each point of a
 * checkerboard texture.
 * 
 * @param uv  The uv coordinates of the point
 * @return The color of the point in the texture
 */
glm::vec3 checkerboardTexture(glm::vec2 uv) {
  float n = 20;
  float value = int(floor(n * uv.s) + floor(2 * n * uv.t)) % 2;

  return glm::vec3(value);
}

/**
 * @brief Rainbow texture
 * 
 * This function returns the color for each point of a
 * rainbow texture.
 * 
 * @param uv The uv coordinates of the point
 * @return The color of the point in the texture
 */
glm::vec3 rainbowTexture(glm::vec2 uv) {
  float n = 40;
  int value = int(floor(n * uv.t + 0.5 * n * uv.s)) % 3;

  switch(value){
    case 0:
      return glm::vec3(1.0, 0.0, 0.0);
    case 1:
      return glm::vec3(0.0, 1.0, 0.0);
    default:
      return glm::vec3(0.0, 0.0, 1.0);
  }
}

#endif /* Textures_h */
