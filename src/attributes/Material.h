#include "../../lib/glm.hpp"
#include "Textures.h"

#ifndef Material_h
#define Material_h

/**
 * @brief Material class
 * 
 * This class represents the material of an object.
 */
struct Material {
  glm::vec3 ambient = glm::vec3(0.0); ///< Ambient coefficient
  glm::vec3 diffuse = glm::vec3(1.0); ///< Diffuse coefficient
  glm::vec3 specular = glm::vec3(0.0); ///< Specular coefficient

  bool is_reflective = false; ///< Indicate if the object is reflective
  bool is_refractive = false; ///< Indicate if the object is refractive

  float shininess = 0.0; ///< Exponent for Phong model
  float reflectiveness = 0.0; ///< Quantify the reflectiveness of the object
  float refractiveness = 0.0; ///< Quantity of refractiveness of the object
  float delta = 0.0; ///< Index representing refractiveness

  glm::vec3 (* texture)(glm::vec2 uv) = NULL; ///< Texture of the object
};

#endif /* Material_h */
