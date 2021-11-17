#include "../../lib/glm.hpp"

#ifndef Light_h
#define Light_h

/**
 * @brief Light class
 * 
 * This class represents a light source.
 */
class Light {
public:
	glm::vec3 position; ///< Position of the light source
	glm::vec3 color; ///< Color/intensity of the light source

  /**
   * @brief Construct a new Light object
   * 
   * @param position The position of the light source
   */
	Light(glm::vec3 position): position(position) {
		color = glm::vec3(1.0);
	}

  /**
   * @brief Construct a new Light object
   * 
   * @param position The position of the light source
   * @param color The color/intensity of the light source
   */
	Light(glm::vec3 position, glm::vec3 color): position(position), color(color) {}
};

#endif /* Light_h */
