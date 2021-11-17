#include "../../lib/glm.hpp"

#ifndef Ray_h
#define Ray_h

/**
 * @brief Ray class
 * 
 * This class represents a ray.
 */
class Ray {
public:
	glm::vec3 origin; ///< Origin of the ray
	glm::vec3 direction; ///< Direction of the ray
	
	/**
	 * @brief Construct a new Ray object
	 * 
	 * @param origin The origin of the ray
	 * @param direction The direction of the ray
	 */
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}
};

#endif /* Ray_h */
