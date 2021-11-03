/**
@file main.cpp
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "Image.h"
#include "Material.h"

using namespace std;

/**
 Class representing a single ray.
*/
class Ray {
public:
	glm::vec3 origin; ///< Origin of the ray
	glm::vec3 direction; ///< Direction of the ray
	
	/**
	 Contructor of the ray
	 @param origin Origin of the ray
	 @param direction Direction of the ray
	*/
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {
	}
};

class Object;

/**
 Structure representing the event of hitting an object
*/
struct Hit {
	bool hit; ///< Boolean indicating whether there was or there was no intersection with an object
	glm::vec3 normal; ///< Normal vector of the intersected object at the intersection point
	glm::vec3 intersection; ///< Point of Intersection
	float distance; ///< Distance from the origin of the ray to the intersection point
	Object *object; ///< A pointer to the intersected object
	glm::vec2 uv; ///< Coordinates for computing the texture
};

/**
 General class for objects
*/
class Object {
protected:
	glm::mat4 transformationMatrix; ///< Matrix representing the transformation from the local to the global coordinate system
	glm::mat4 inverseTransformationMatrix; ///< Matrix representing the transformation from the global to the local coordinate system
	glm::mat4 normalMatrix; ///< Matrix for transforming normal vectors from the local to the global coordinate system
	
public:
	glm::vec3 color; ///< Color of the object
	Material material; ///< Structure describing the material of the object

	/** A function computing an intersection, which returns the structure Hit */
	virtual Hit intersect(Ray ray) = 0;

	/** Function that returns the material struct of the object */
	Material getMaterial() {
		return material;
	}

	/**
	 Function that set the material
	 @param material A structure desribing the material of the object
	*/
	void setMaterial(Material material) {
		this->material = material;
	}

	/**
	 Functions for setting up all the transformation matrices
	 @param matrix The matrix representing the transformation of the object in the global coordinates
	*/
	void setTransformation(glm::mat4 matrix) {
		transformationMatrix = matrix;
		inverseTransformationMatrix = glm::inverse(matrix);
		normalMatrix = glm::transpose(inverseTransformationMatrix);
	}
};

/**
 Implementation of the class Object for spheres
*/
class Sphere : public Object {
private:
	float radius; ///< Radius of the sphere
	glm::vec3 center; ///< Center of the sphere

public:
	/**
	 The constructor of the sphere
	 @param radius Radius of the sphere
	 @param center Center of the sphere
	 @param color Color of the sphere
	*/
	// Sphere(float radius, glm::vec3 center, glm::vec3 color) : radius(radius), center(center) {
	// 	this->color = color;
	// }

	// Sphere(float radius, glm::vec3 center, Material material) : radius(radius), center(center) {
	// 	this->material = material;
	// }

	Sphere(Material material) {
		this->material = material;

		this->radius = 1.0;
		this->center = glm::vec3(0.0);
	}

	/** Implementation of the intersection function */
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

/**
 Implementation of the class Object for planes
*/
class Plane : public Object {
private:
	glm::vec3 normal;
	glm::vec3 point;

public:
	/**
	 The constructor of the plane
	 @param point Center of the plane
	 @param normal Normal of the plane
	*/
	Plane(glm::vec3 point, glm::vec3 normal) : point(point), normal(normal) {
	}

	Plane(glm::vec3 point, glm::vec3 normal, Material material) : point(point), normal(normal) {
		this->material = material;
	}

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

/**
 Implementation of the class Object for cones
*/
class Cone : public Object {
private:
	Plane * plane;
public:
	/**
	 The constructor of the cone
	 @param material Material of the cone
	*/
	Cone (Material material) {
		this->material = material;
		plane = new Plane(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0));
	}

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

/**
 Light class
*/
class Light {
public:
	glm::vec3 position; ///< Position of the light source
	glm::vec3 color; ///< Color/intensity of the light source

	Light(glm::vec3 position): position(position) {
		color = glm::vec3(1.0);
	}

	Light(glm::vec3 position, glm::vec3 color): position(position), color(color) {
	}
};

vector<Light *> lights; ///< A list of lights in the scene
vector<Object *> objects; ///< A list of all objects in the scene
glm::vec3 ambient_light(1.0, 1.0, 1.0);

glm::vec3 trace_ray(Ray ray, bool is_inside=false);
glm::vec3 toneMapping(glm::vec3 intensity);
float compute_shadow(Ray ray, Light * source, glm::vec3 intersection);
float compute_fresnel(float beta_1, float beta_2, float cos_1, float cos_2);

/** Function for computing color of an object according to the Phong Model
 @param point A point belonging to the object for which the color is computed
 @param normal A normal vector at the point
 @param uv Texture coordinates
 @param view_direction A normalized direction from the point to the viewer/camera
 @param material A material structure representing the material of the object
 @param inside_object Flag to check if the ray is inside or outside the object
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
 Function performing tone mapping of the intensities computed using the raytracer
 @param intensity Input intensity
 @return Tone mapped intensity in range (0,1)
*/
glm::vec3 toneMapping(glm::vec3 intensity) {
	glm::vec3 alpha(10.0);
	glm::vec3 beta(3.0);
	glm::vec3 gamma(3.0);

	glm::vec3 tone_mapped = glm::pow(alpha * glm::pow(intensity, beta), glm::vec3(1.0) / gamma);
	
	return glm::clamp(tone_mapped, glm::vec3(0.0), glm::vec3(1.0));
}

/**
 Function that finds if the point is occluded from light
 @param ray Ray that should be traced through the scene
 @param source The light source
 @param intersection The intersection point of the object
 @return 1 if light is not occluded, 0 if it is
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

/**
 Function that computes the Fresnel Effect
 @param beta_1 The delta of the first material
 @param beta_2 The delta of the second material
 @param cos_1 The cosine of the angle of the original direction
 @param cos_2 The cosine of the angle of the refracted ray
 @return The Fresnel Effect
*/
float compute_fresnel(float beta_1, float beta_2, float cos_1, float cos_2) {
	float part_1 = pow((beta_1 * cos_1 - beta_2 * cos_2) / (beta_1 * cos_1 + beta_2 * cos_2), 2);
	float part_2 = pow((beta_1 * cos_2 - beta_2 * cos_1) / (beta_1 * cos_2 + beta_2 * cos_1), 2);

	return 0.5 * (part_1 + part_2);
}

/**
 Function that computes a color along the ray
 @param ray Ray that should be traced through the scene
 @return Color at the intersection point
*/
glm::vec3 trace_ray(Ray ray, bool is_inside) {
	Hit closest_hit;

	bool in_obj = false;
	
	closest_hit.hit = false;
	closest_hit.distance = INFINITY;
	
	// Loop over all objects to find the closest intersection
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

/**
 Function defining the scene
*/
void sceneDefinition(float x=0, float y=12) {
	// Materials
	Material blue;
	blue.ambient = glm::vec3(0.06f, 0.06f, 0.09f);
	blue.diffuse = glm::vec3(0.7f, 0.7f, 1.0f);
	blue.specular = glm::vec3(0.6);
	blue.shininess = 100.0;
	
	Material blue_matte;
	blue_matte.ambient = glm::vec3(0.06f, 0.06f, 0.09f);
	blue_matte.diffuse = glm::vec3(0.7f, 0.7f, 1.0f);
	blue_matte.specular = glm::vec3(0.0);
	blue_matte.shininess = 0.0;

	Material red;
	red.ambient = glm::vec3(0.09f, 0.06f, 0.06f);
	red.diffuse = glm::vec3(1.0f, 0.3f, 0.3f);
	red.specular = glm::vec3(0.5);
	red.shininess = 10.0;

	Material red_matte;
	red_matte.ambient = glm::vec3(0.09f, 0.06f, 0.06f);
	red_matte.diffuse = glm::vec3(1.0f, 0.3f, 0.3f);
	red_matte.specular = glm::vec3(0.0);
	red_matte.shininess = 0.0;
	
	Material green;
	green.ambient = glm::vec3(0.06f, 0.09f, 0.06f);
	green.diffuse = glm::vec3(0.7f, 0.9f, 0.7f);
	green.specular = glm::vec3(0.0);
	green.shininess = 0.0;

	Material green_matte;
	green_matte.ambient = glm::vec3(0.06f, 0.09f, 0.06f);
	green_matte.diffuse = glm::vec3(1.0f, 0.3f, 0.3f);
	green_matte.specular = glm::vec3(0.0);
	green_matte.shininess = 0.0;

	Material yellow;
	yellow.ambient = glm::vec3(0.09f, 0.09f, 0.06f);
	yellow.diffuse = glm::vec3(0.9f, 0.9f, 0.2f);
	yellow.specular = glm::vec3(0.6);
	yellow.shininess = 80.0;

	Material white;
	white.ambient = glm::vec3(0.06f, 0.06f, 0.06f);
	white.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	white.specular = glm::vec3(0.1);
	white.shininess = 0.0;

	Material mirror;
	mirror.specular = glm::vec3(0.3);
	mirror.is_reflective = true;
	mirror.reflectiveness = 1.0;

	Material glass;
	glass.is_refractive = true;
	glass.refractiveness = 1.0;
	glass.delta = 2.0;

	// Textures
	Material checkerBoard;
	checkerBoard.texture = &checkerboardTexture;

	Material rainbow;
	rainbow.texture = &rainbowTexture;

	// Transformation Matrices
	glm::mat4 ST1 = glm::translate(glm::vec3(-1.0, -2.5, 6.0));
	glm::mat4 SS1 = glm::scale(glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 SM1 = ST1 * SS1;

	glm::mat4 SM2 = glm::translate(glm::vec3(1.0, -2.0, 8.0));

	glm::mat4 ST3 = glm::translate(glm::vec3(-3.0, -1.0, 8.0));
	glm::mat4 SS3 = glm::scale(glm::vec3(2.0, 2.0, 2.0));
	glm::mat4 SM3 = ST3 * SS3;

	glm::mat4 ST4 = glm::translate(glm::vec3(-6.0, 4.0, 23.0));
	glm::mat4 SS4 = glm::scale(glm::vec3(7.0, 7.0, 7.0));
	glm::mat4 SM4 = ST4 * SS4;

	glm::mat4 CT1 = glm::translate(glm::vec3(5.0, 9.0, 14.0));
	glm::mat4 CS1 = glm::scale(glm::vec3(3.0, 12.0, 3.0));
	glm::mat4 CR1 = glm::rotate(glm::radians(180.0f) , glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 CM1 = CT1 * CS1 * CR1;

	glm::mat4 CT2 = glm::translate(glm::vec3(6.0, -3.0, 7.0));
	glm::mat4 CS2 = glm::scale(glm::vec3(1.0, 3.0, 1.0));
	glm::mat4 CR2 = glm::rotate(glm::atan(3.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 CM2 = CT2 * CR2 * CS2;

	// Normal Spheres
	Sphere * sphere1 = new Sphere(red);
	sphere1->setTransformation(SM1);
	objects.push_back(sphere1);

	// Special Spheres
	Sphere * sphere2 = new Sphere(mirror);
	sphere2->setTransformation(SM2);
	objects.push_back(sphere2);

	Sphere * sphere3 = new Sphere(glass);
	sphere3->setTransformation(SM3);
	objects.push_back(sphere3);

	// Textured spheres
	Sphere * sphere4 = new Sphere(rainbow);
	sphere4->setTransformation(SM4);
	objects.push_back(sphere4);

	// Planes
	objects.push_back(new Plane(glm::vec3(0, 0, 30.0), glm::normalize(glm::vec3(0, 0, 30.0)), green));
	objects.push_back(new Plane(glm::vec3(0, 0, -0.01), glm::normalize(glm::vec3(0, 0, -0.01)), green));
	objects.push_back(new Plane(glm::vec3(15.0, 0, 0), glm::normalize(glm::vec3(15.0, 0, 0)), blue_matte));
	objects.push_back(new Plane(glm::vec3(-15.0, 0, 0), glm::normalize(glm::vec3(-15.0, 0, 0)), red_matte));
	objects.push_back(new Plane(glm::vec3(0, 27.0, 0), glm::normalize(glm::vec3(0, 27.0, 0)), white));
	objects.push_back(new Plane(glm::vec3(0, -3.0, 0), glm::normalize(glm::vec3(0, -3.0, 0)), white));
	
	// Cones
	Cone * cone1 = new Cone(yellow);
	cone1->setTransformation(CM1);
	objects.push_back(cone1);

	Cone * cone2 = new Cone(green);
	cone2->setTransformation(CM2);
	objects.push_back(cone2);

	// Light sources
	lights.push_back(new Light(glm::vec3(0, 26, 5), glm::vec3(0.2)));
	lights.push_back(new Light(glm::vec3(x, 1, y), glm::vec3(0.2)));
	lights.push_back(new Light(glm::vec3(0, 5, 1), glm::vec3(0.2)));
}

int main(int argc, const char * argv[]) {
	clock_t t = clock(); // variable for keeping the time of the rendering
	
	int width = 1024; // width of the image
	int height = 768; // height of the image
	float fov = 90; // field of view

	float pixel_size = (2 * tan((fov / 2) * (M_PI / 180))) / width; // size of the pixels
	
	if (argc > 2) {
		sceneDefinition(atof(argv[2]), atof(argv[3]));
	} else {
		sceneDefinition();
	}
	
	Image image(width, height); // Create an image where we will store the result

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float dx = (((- width) * pixel_size) / 2) + (i * pixel_size) + (0.5 * pixel_size);
			float dy = ((height * pixel_size) / 2) - (j * pixel_size) - (0.5 * pixel_size);
			
			glm::vec3 origin = glm::vec3(0.0);
			glm::vec3 direction = glm::vec3(dx, dy, 1);

			direction = glm::normalize(direction);

			Ray ray = Ray(origin, direction);
			image.setPixel(i, j, trace_ray(ray));
		}
	}
  
	for (int i = 0; i < argc ; i++) {
		if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v")) {
			t = clock() - t;
			cout << "It took " << ((float)t)/CLOCKS_PER_SEC << " seconds to render the image." << endl;
			cout << "I could render at " << (float)CLOCKS_PER_SEC/((float)t) << " frames per second." << endl;
		}
	}
    
	// Writing the final results of the rendering
	if (argc >= 2) {
		image.writeImage(argv[1]);
	} else {
		image.writeImage("./result.ppm");
	}

	return 0;
}
