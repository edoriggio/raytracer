#include "../primitives/Object.h"

#ifndef Sphere_h
#define Sphere_h

/**
 * @brief Sphere class
 * 
 * This class represents a sphere.
 */
class Sphere : public Object {
private:
	float radius; ///< Radius of the sphere
	glm::vec3 center; ///< Center of the sphere

public:
	/**
	 * @brief Construct a new Sphere object
	 * 
	 * @param material The material of the sphere
	 */
	Sphere(Material material) {
		this->material = material;
		this->radius = 1.0;
		this->center = glm::vec3(0.0);
	}

	/**
	 * @brief Implement the intersection of the sphere with a ray
	 * 
	 * @param ray Ray intersecting the sphere
	 * @return Hit struct representing the intersection
	 */
	Hit intersect(Ray ray) {
		Hit hit;
		hit.hit = false;

		glm::vec3 d = inverseTransformationMatrix * glm::vec4(ray.direction, 0.0);
		glm::vec3 o = inverseTransformationMatrix * glm::vec4(ray.origin, 1.0);
		d = glm::normalize(d);

		glm::vec3 c = - o;
		float delta = glm::dot(c, c) - glm::dot(c, d) * glm::dot(c, d);

		if (delta < 0) return hit;

		float D = sqrt(delta);

		if (D > radius) return hit;

		float t;
		float t1 = glm::dot(c, d) + sqrt(1.0f - D * D);
		float t2 = glm::dot(c, d) - sqrt(1.0f - D * D);

		t = t1 < t2 ? t1 : t2;
		if (t < 0) return hit;

		glm::vec3 intersection = o + t * d;
		
		glm::vec3 normal = intersection;
		normal = glm::normalize(normal);

		float theta = asin(normal.y);
		float phi = atan2(normal.z, normal.x);
		
		hit.hit = true;
		hit.intersection = transformationMatrix * glm::vec4(intersection, 1.0);
		hit.distance = glm::distance(ray.origin, hit.intersection);
		hit.normal = normalMatrix * glm::vec4(normal, 0.0);
		hit.normal = glm::normalize(hit.normal);
		hit.object = this;

		hit.uv.s = (theta + M_PI / 2) / M_PI;
		hit.uv.t = (phi + M_PI) / (2 * M_PI);

		return hit;
	}
};

#endif /* Sphere_h */
