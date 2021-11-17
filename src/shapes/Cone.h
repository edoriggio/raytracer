#include "../primitives/Object.h"
#include "Plane.h"

#ifndef Cone_h
#define Cone_h

/**
 * @brief Cone class
 * 
 * This class represents a cone.
 */
class Cone : public Object {
private:
	Plane * plane;
public:
	/**
	 * @brief Construct a new Cone object
	 * 
	 * @param material The material of the cone
	 */
	Cone (Material material) {
		this->material = material;
		plane = new Plane(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0));
	}

  /**
	 * @brief Implement the intersection of the cone with a ray
	 * 
	 * @param ray Ray intersecting the cone
	 * @return Hit struct representing the intersection
	 */
	Hit intersect(Ray ray) {
		Hit hit;
		hit.hit = false;
		
		glm::vec3 d = inverseTransformationMatrix * glm::vec4(ray.direction, 0.0);
		glm::vec3 o = inverseTransformationMatrix * glm::vec4(ray.origin, 1.0);
		d = glm::normalize(d);
		
		float a = d.x * d.x + d.z * d.z - d.y * d.y;
		float b = 2 * (d.x * o.x + d.z * o.z - d.y * o.y);
		float c = o.x * o.x + o.z * o.z - o.y * o.y;
		
		float delta = b * b - 4 * a * c;
		
		if (delta < 0) {
			return hit;
		}
		
		float t1 = (-b - sqrt(delta)) / (2 * a);
		float t2 = (-b + sqrt(delta)) / (2 * a);
		
		float t = t1;
		hit.intersection = o + t * d;

		if (t < 0 || hit.intersection.y > 1 || hit.intersection.y < 0) {
			t = t2;
			hit.intersection = o + t * d;

			if (t < 0 || hit.intersection.y > 1 || hit.intersection.y < 0) return hit;
		}
	
		hit.normal = glm::vec3(hit.intersection.x, -hit.intersection.y, hit.intersection.z);
		hit.normal = glm::normalize(hit.normal);
		
		Ray new_ray(o, d);
		Hit hit_plane = plane->intersect(new_ray);

		if (hit_plane.hit && hit_plane.distance < t && length(hit_plane.intersection - glm::vec3(0.0, 1.0, 0.0)) <= 1.0) {
			hit.intersection = hit_plane.intersection;
			hit.normal = -hit_plane.normal;
		}
		
		hit.hit = true;
		hit.object = this;
		hit.intersection = transformationMatrix * glm::vec4(hit.intersection, 1.0);
		hit.normal = normalMatrix * glm::vec4(hit.normal, 0.0);
		hit.normal = glm::normalize(hit.normal);
		hit.distance = glm::length(hit.intersection - ray.origin);
		
		return hit;
	}
};

#endif /* Cone_h */
