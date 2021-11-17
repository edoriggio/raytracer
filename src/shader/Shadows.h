#include <vector>
#include "../../lib/glm.hpp"
#include "../primitives/Ray.h"
#include "../primitives/Light.h"
#include "../primitives/Object.h"

#ifndef Shadows_h
#define Shadows_h

using namespace std;

vector<Light *> lights; ///< A list of lights in the scene
vector<Object *> objects; ///< A list of all objects in the scene
glm::vec3 ambient_light(1.0, 1.0, 1.0);

/**
 * @brief Function that checks if an object occludes the light source
 * 
 * @param ray A ray from the object to the light source
 * @param source The light source
 * @param intersection The intersection point of the object
 * @return retuns the amount of light that is not blocked by the object 
 */
float compute_shadow(Ray ray, Light * source, glm::vec3 intersection) {
	vector<Object *> objs = objects;
	float light_distance = glm::distance(intersection, source->position);
	
	for (int k = 0; k < objs.size(); k++) {
		Hit hit = objs[k]->intersect(ray);

		if (hit.hit == true && hit.distance < light_distance) {
			Material material = objs[k]->getMaterial();
			if (material.is_refractive) return 0.4;

			return 0.0;
		}
	}

	return 1.0;
}

#endif /* Shadows_h */
