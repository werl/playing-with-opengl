//
// Created by Peter Lewis on 2016-10-23.
//

#include <string>
#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Image::Image(std::string filename, int image_type) {
    image = stbi_load(filename.c_str(), &image_width, &image_height, &comp, image_type);
}

void Image::releaseImage() {
    stbi_image_free(image);
}

