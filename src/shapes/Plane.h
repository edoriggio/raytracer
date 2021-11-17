#include "../primitives/Object.h"

#ifndef Plane_h
#define Plane_h

/**
 * @brief Plane class
 * 
 * This class represents a plane.
 */
class Plane : public Object {
private:
	glm::vec3 normal;
	glm::vec3 point;

public:
	/**
	 * @brief Construct a new Plane object
	 * 
	 * @param point Point of the plane
	 * @param normal Normal of the plane
	 */
	Plane(glm::vec3 point, glm::vec3 normal) : point(point), normal(normal) {}

  /**
   * @brief Construct a new Plane object
   * 
   * @param point Point of the plane
   * @param normal Normal of the plane
   * @param material Material of the plane
   */
	Plane(glm::vec3 point, glm::vec3 normal, Material material) : point(point), normal(normal) {
		this->material = material;
	}

	/**
	 * @brief Implement the intersection of the sphere with a plane
	 * 
	 * @param ray Ray intersecting the plane
	 * @return Hit struct representing the intersection
	 */
	Hit intersect(Ray ray) {
		Hit hit;
		hit.hit = false;
		
		float num = glm::dot(point - ray.origin, normal);
		float denom = glm::dot(ray.direction, normal);

		if (denom == 0) return hit;

		float t = num / denom;

		if (t < 0) return hit;

		glm::vec3 intersection = ray.origin + ray.direction * t;

		hit.hit = true;
		hit.distance = glm::distance(ray.origin, intersection);
		hit.intersection = intersection;
		hit.normal = -normal;
		hit.object = this;
		
		return hit;
	}
};

#endif /* Plane_h */
