#include <vector>
#include "Shadows.h"
#include "Fresnel.h"
#include "ToneMapping.h"
#include "../../lib/glm.hpp"
#include "../primitives/Ray.h"
#include "../primitives/Light.h"
#include "../primitives/Object.h"

#ifndef Phong_h
#define Phong_h

glm::vec3 trace_ray(Ray ray, bool is_inside=false);

/**
 * @brief Function that computes the color of a point based on the Phong Model
 * 
 * @param point A point belonging to the object for which the color is computed
 * @param normal A normal vector at the point
 * @param uv Texture coordinates
 * @param view_direction A normalized direction from the point to the viewer/camera
 * @param material A material structure representing the material of the object
 * @param is_inside	Flag to check if the ray is inside or outside the object
 * @return The color of the point
 */
glm::vec3 PhongModel(glm::vec3 point, glm::vec3 normal, glm::vec2 uv, glm::vec3 view_direction, Material material, bool is_inside) {
	glm::vec3 color = glm::vec3(0.0);
	float epsilon = 0.001;

	if (material.is_reflective) {
		glm::vec3 reflected_vec = glm::reflect(-view_direction, normal);
		reflected_vec = glm::normalize(reflected_vec);

		Ray reflected_ray = Ray(point + epsilon * reflected_vec, reflected_vec);

		return trace_ray(reflected_ray) * material.reflectiveness;
	} else if (material.is_refractive) {
		float beta = is_inside ? material.delta : 1.0f / material.delta;
		float beta_2 = is_inside ? 1.0f / material.delta: material.delta;

		glm::vec3 direction_to_refract = is_inside ? view_direction : -view_direction; 
		glm::vec3 normal_to_refract = is_inside ? -normal : normal;

		glm::vec3 refracted_vec = glm::refract(direction_to_refract, normal_to_refract, beta);
		refracted_vec = glm::normalize(refracted_vec);

		glm::vec3 reflected_vec = glm::reflect(-view_direction, normal);
		reflected_vec = glm::normalize(reflected_vec);

		Ray refracted_ray = Ray(point - epsilon * normal_to_refract, -normal_to_refract);
		Ray reflected_ray = Ray(point + epsilon * reflected_vec, reflected_vec);

		float cos_1 = glm::dot(-direction_to_refract, normal_to_refract);
		float cos_2 = glm::dot(refracted_vec, -normal_to_refract);
		float fresnel = compute_fresnel(beta, beta_2, cos_1, cos_2);

		glm::vec3 refracted_color = fresnel < 1.0f ? trace_ray(refracted_ray, !is_inside) : glm::vec3(0.0);
		glm::vec3 reflected_color = trace_ray(reflected_ray);

		return reflected_color * fresnel + refracted_color * (1 - fresnel);
	} else {
		color += material.ambient * ambient_light;

		for (Light * source : lights) {
			glm::vec3 diffuse;

			float att_a = 1.0;
			float att_b = 0.001;
			float att_c = 0.001;

			glm::vec3 normal_source = glm::normalize(source->position - point);
			glm::vec3 reflected = glm::normalize(2.0f * normal * glm::dot(normal, normal_source) - normal_source);

			float is_occluded = glm::dot(normal_source, normal) < 0 ? 0.0 : 1.0;

			Ray shadow_ray(point + epsilon * normal_source, normal_source);
			if (is_occluded == 1.0) is_occluded = compute_shadow(shadow_ray, source, point);

			float cos_alpha = glm::dot(reflected, view_direction) >= 0.0f ? glm::dot(reflected, view_direction) : 0.0;
			float cos_phi = glm::dot(normal, normal_source) >= 0.0f ? glm::dot(normal, normal_source) : 0.0;
			float distance = glm::distance(source->position, point);

			if (material.texture != NULL) {
				diffuse = material.texture(uv) * cos_phi;
			} else {
				diffuse = material.diffuse * cos_phi;
			}

			glm::vec3 specular = material.specular * pow(cos_alpha, material.shininess);
			float attenuation = 1 / (att_a + (att_b * distance) + (att_c * pow(distance, 2)));

			color += ((diffuse + specular) * source->color * attenuation) * is_occluded;
		}

		return toneMapping(color);
	}
}

/**
 * @brief Function that traces all the rays and computes the colors of the pixels
 * 
 * @param ray A ray to be traced
 * @param is_inside Flag to check if the ray is inside or outside the object
 * @return The color of the pixel
 */
glm::vec3 trace_ray(Ray ray, bool is_inside) {
	Hit closest_hit;

	closest_hit.hit = false;
	closest_hit.distance = INFINITY;
	
	for (int k = 0; k < objects.size(); k++) {
		Hit hit = objects[k]->intersect(ray);

		if (hit.hit == true && hit.distance < closest_hit.distance) {
			closest_hit = hit;
		}
	}
	
	glm::vec3 color(0.0);

	if (closest_hit.hit)
		color = PhongModel(closest_hit.intersection, closest_hit.normal, closest_hit.uv, glm::normalize(-ray.direction), closest_hit.object->getMaterial(), is_inside);

	return color;
}

#endif /* Phong_h */
