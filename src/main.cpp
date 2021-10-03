/**
@file main.cpp
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include "glm/glm.hpp"

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
};

/**
 General class for the object
 */
class Object {
public:
	glm::vec3 color; ///< Color of the object
	Material material;

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
};

/**
 Implementation of the class Object for sphere shape.
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
	Sphere(float radius, glm::vec3 center, glm::vec3 color) : radius(radius), center(center) {
		this->color = color;
	}

	Sphere(float radius, glm::vec3 center, Material material) : radius(radius), center(center){
		this->material = material;
	}

	/** Implementation of the intersection function */
	Hit intersect(Ray ray) {
		Hit hit;
		hit.hit = false;

		glm::vec3 intersection;

		glm::vec3 d = ray.direction;
		glm::vec3 c = center;

		float D = sqrt(pow(glm::length(c), 2) - pow(glm::dot(c, d), 2));

		if (D > radius) {
			return hit;
		}

		float t;
		float t1 = glm::dot(c, d) + sqrt(pow(radius, 2) - pow(D, 2));
		float t2 = glm::dot(c, d) - sqrt(pow(radius, 2) - pow(D, 2));

		t = t1 < t2 ? t1 : t2;
		intersection = t * ray.direction;
		
		glm::vec3 normal = glm::normalize(intersection - c);
		
		hit.hit = true;
		hit.distance = glm::distance(ray.origin, intersection);
		hit.intersection = intersection;
		hit.normal = normal;
		hit.object = this;

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

/** Function for computing color of an object according to the Phong Model
 @param point A point belonging to the object for which the color is computed
 @param normal A normal vector at the point
 @param view_direction A normalized direction from the point to the viewer/camera
 @param material A material structure representing the material of the object
*/
glm::vec3 PhongModel(glm::vec3 point, glm::vec3 normal, glm::vec3 view_direction, Material material) {
	glm::vec3 color = glm::vec3(0.0);
	color += material.ambient * ambient_light;

	for (Light * source : lights) {
		glm::vec3 normal_source = glm::normalize(source->position - point);
		glm::vec3 reflected = glm::normalize(2.0f * normal * glm::dot(normal, normal_source) - normal_source);

		float cos_alpha = glm::dot(reflected, view_direction) >= 0.0f ? glm::dot(reflected, view_direction) : 0.0;
		float cos_phi = glm::dot(normal, normal_source) >= 0.0f ? glm::dot(normal, normal_source) : 0.0;

		glm::vec3 diffuse = material.diffuse * cos_phi;
		glm::vec3 specular = material.specular * pow(cos_alpha, material.shininess);

		color += (diffuse + specular) * source->color;
	}

	color = glm::clamp(color, glm::vec3(0.0), glm::vec3(1.0));

	return color;
}

/**
 Functions that computes a color along the ray
 @param ray Ray that should be traced through the scene
 @return Color at the intersection point
*/
glm::vec3 trace_ray(Ray ray) {
	// Hit structure representing the closest intersection
	Hit closest_hit;
	
	closest_hit.hit = false;
	closest_hit.distance = INFINITY;
	
	// Loop over all objects to find the closest intersection
	for (int k = 0; k<objects.size(); k++) {
		Hit hit = objects[k]->intersect(ray);

		if (hit.hit == true && hit.distance < closest_hit.distance)
			closest_hit = hit;
	}
	
	glm::vec3 color(0.0);

	if (closest_hit.hit)
		color = PhongModel(closest_hit.intersection, closest_hit.normal, glm::normalize(-ray.direction), closest_hit.object->getMaterial());

	return color;
}

/**
 Function defining the scene
*/
void sceneDefinition () {
	Material blue;
	blue.ambient = glm::vec3(0.07f, 0.07f, 0.1f);
	blue.diffuse = glm::vec3(0.7f, 0.7f, 1.0f);
	blue.specular = glm::vec3(0.6);
	blue.shininess = 100.0;
	
	Material red;
	red.ambient = glm::vec3(0.01f, 0.03f, 0.03f);
	red.diffuse = glm::vec3(1.0f, 0.3f, 0.3f);
	red.specular = glm::vec3(0.5);
	red.shininess = 10.0;

	Material green;
	green.ambient = glm::vec3(0.07f, 0.09f, 0.07f);
	green.diffuse = glm::vec3(0.7f, 0.9f, 0.7f);
	green.specular = glm::vec3(0.0);
	green.shininess = 0.0;

	objects.push_back(new Sphere(1.0, glm::vec3(1.0, -2.0, 8.0), blue));
	objects.push_back(new Sphere(0.5, glm::vec3(-1.0, -2.5, 6.0), red));
	objects.push_back(new Sphere(1.0, glm::vec3(3.0, -2.0, 6.0), green));

	lights.push_back(new Light(glm::vec3(0, 26, 5), glm::vec3(0.4)));
	lights.push_back(new Light(glm::vec3(0, 1, 12), glm::vec3(0.4)));
	lights.push_back(new Light(glm::vec3(0, 5, 1), glm::vec3(0.4)));
}

int main(int argc, const char * argv[]) {
	clock_t t = clock(); // variable for keeping the time of the rendering
	
	int width = 1024; // width of the image
	int height = 768; // height of the image
	float fov = 90; // field of view

	float pixel_size = (2 * tan((fov / 2) * (M_PI / 180))) / width; // size of the pixels
	
	sceneDefinition();
	
	Image image(width, height); // Create an image where we will store the result

	for(int i = 0; i < width ; i++)
		for(int j = 0; j < height ; j++) {
			float dx = (((- width) * pixel_size) / 2) + (i * pixel_size) + (0.5 * pixel_size);
			float dy = ((height * pixel_size) / 2) - (j * pixel_size) - (0.5 * pixel_size);
			
			glm::vec3 origin = glm::vec3(0,0,0);
			glm::vec3 direction = glm::vec3(dx,dy,1);

			direction = glm::normalize(direction);

			Ray ray = Ray(origin, direction);
			image.setPixel(i, j, trace_ray(ray));
		}
    
	t = clock() - t;
	cout << "It took " << ((float)t)/CLOCKS_PER_SEC << " seconds to render the image." << endl;
	cout << "I could render at " << (float)CLOCKS_PER_SEC/((float)t) << " frames per second." << endl;
    
	// Writing the final results of the rendering
	if (argc == 2) {
		image.writeImage(argv[2]);
	} else {
		image.writeImage("./result.ppm");
	}

	return 0;
}
