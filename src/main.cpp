#include <ctime>
#include <cmath>
#include <iostream>
#include "./Scene.h"
#include "../lib/glm.hpp"
#include "./shader/Phong.h"
#include "./primitives/Ray.h"
#include "./primitives/Image.h"

using namespace std;

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
	
	Image image(width, height);

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

	image.writeImage("./out/result.ppm");

	return 0;
}
