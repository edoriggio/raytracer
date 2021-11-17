#include "../../lib/glm.hpp"
#include "../primitives/Ray.h"
#include "../attributes/Material.h"

#ifndef Object_h
#define Object_h

struct Hit;

/**
 * @brief Object class
 * 
 * This class represents an object which can be extended.
 */
class Object {
protected:
	glm::mat4 transformationMatrix; ///< Matrix representing the transformation from the local to the global coordinate system
	glm::mat4 inverseTransformationMatrix; ///< Matrix representing the transformation from the global to the local coordinate system
	glm::mat4 normalMatrix; ///< Matrix for transforming normal vectors from the local to the global coordinate system
	
public:
	glm::vec3 color; ///< Color of the object
	Material material; ///< Structure describing the material of the object

	virtual Hit intersect(Ray ray) = 0;

	/**
	 * @brief Get the Material object
	 * 
	 * @return The material of the object
	 */
	Material getMaterial() {
		return material;
	}

  /**
   * @brief Set the Material object
   * 
   * @param material The material of the object
   */
	void setMaterial(Material material) {
		this->material = material;
	}

  /**
   * @brief Set the Transformation matices
   * 
   * @param matrix The matrix representing the transformation of the object in the global coordinates
   */
	void setTransformation(glm::mat4 matrix) {
		transformationMatrix = matrix;
		inverseTransformationMatrix = glm::inverse(matrix);
		normalMatrix = glm::transpose(inverseTransformationMatrix);
	}
};

/**
 * @brief Hit structure
 * 
 * This structure represents the hit of a ray on an object.
 */
struct Hit {
	bool hit; ///< Boolean indicating whether there was or there was no intersection with an object
	glm::vec3 normal; ///< Normal vector of the intersected object at the intersection point
	glm::vec3 intersection; ///< Point of Intersection
	float distance; ///< Distance from the origin of the ray to the intersection point
	Object *object; ///< A pointer to the intersected object
	glm::vec2 uv; ///< Coordinates for computing the texture
};

#endif /* Object_h */
