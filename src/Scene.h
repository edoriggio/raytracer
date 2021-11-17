#include "../lib/glm.hpp"
#include "./shapes/Cone.h"
#include "./shapes/Plane.h"
#include "./shapes/Sphere.h"
#include "./shader/Shadows.h"
#include "./primitives/Light.h"
#include "./primitives/Object.h"
#include "./attributes/Material.h"
#include "./attributes/Textures.h"
#include "../lib/gtx/transform.hpp"

#ifndef Scene_h
#define Scene_h

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

#endif /* Scene_h */
