//
//  Material.h
//  Raytracer
//

#ifndef Material_h
#define Material_h

#include "glm/glm.hpp"
#include "Textures.h"

/**
 Structure describing a material of an object
*/
struct Material {
  glm::vec3 ambient = glm::vec3(0.0); ///< Ambient coefficient
  glm::vec3 diffuse = glm::vec3(1.0); ///< Diffuse coefficient
  glm::vec3 specular = glm::vec3(0.0); ///< Specular coefficient
  float shininess = 0.0; ///< Exponent for Phong model
  float reflectiveness = 0.0; ///< Index representing reflectiveness
  float refractiveness = 0.0; ///< Index representing refractiveness
  glm::vec3 (* texture)(glm::vec2 uv) = NULL; ///< Texture
};

#endif /* Material_h */
