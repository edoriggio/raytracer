#include <iostream>
#include <fstream>
#include "../../lib/glm.hpp"

#ifndef Image_h
#define Image_h

using namespace std;

/**
 * @brief Image class
 * 
 * This class represents an image.
 */
class Image {
private:
  int width, height; ///< width and height of the image
  int *data; ///< a pointer to the data representing the images
  
public:
  /**
   * @brief Construct a new Image object
   * 
   * @param width The width of the image
   * @param height The height of the image
   */
  Image(int width, int height): width(width), height(height) {
    data = new int[3 * width * height];
  }

  /**
   * @brief Write an image to a ppm file
   * 
   * @param path The path where to save the image
   */
  void writeImage(const char *path) {
    ofstream file;
    file.open(path);
    file << "P3" << endl;
    file << width << " " << height << endl;
    file << 255 << endl;

    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        file << data[3 * (h * width + w)] << " ";
        file << data[3 * (h * width + w) + 1] << " ";
        file << data[3 * (h * width + w) + 2] << "  ";
      }

      file << endl;
    }

    file.close();
  }

  /**
   * @brief Set the Pixel value
   * 
   * @param x The x coordinate of the pixel
   * @param y The y coordinate of the pixel
   * @param r The red chanel value
   * @param g The green chanel value
   * @param b The blue chanel value
   */
  void setPixel(int x, int y, int r, int g, int b) {
    data[3 * (y * width + x)] = r;
    data[3 * (y * width + x) + 1] = g;
    data[3 * (y * width + x) + 2] = b;
  }

  /**
   * @brief Set the Pixel value
   * 
   * @param x The x coordinate of the pixel
   * @param y The y coordinate of the pixel
   * @param r The red chanel value
   * @param g The green chanel value
   * @param b The blue chanel value
   */
  void setPixel(int x, int y, float r, float g, float b) {
    data[3 * (y * width + x)] = (float)(255 * r);
    data[3 * (y * width + x) + 1] = (float)(255 * g);
    data[3 * (y * width + x) + 2] = (float)(255 * b);
  }

  /**
   * @brief Set the Pixel value
   * 
   * @param x The x coordinate of the pixel
   * @param y The y coordinate of the pixel
   * @param color The vector representing the color of the pixel
   */
  void setPixel(int x, int y, glm::vec3 color) {
    data[3 * (y * width + x)] = (float)(255 * color.r);
    data[3 * (y * width + x) + 1] = (float)(255 * color.g);
    data[3 * (y * width + x) + 2] = (float)(255 * color.b);
  }
};

#endif /* Image_h */
