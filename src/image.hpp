//
// Created by Peter Lewis on 2016-10-23.
//

#ifndef LEARNINGCPP_IMAGELOADER_HPP
#define LEARNINGCPP_IMAGELOADER_HPP


#include <ntsid.h>
#include <vector>

class Image {
private:
    int image_width;
    int image_height;
    int comp;
    unsigned char* image;

public:
    Image(std::string filename, int image_type);

    int get_comp() {
        return comp;
    }

    int get_image_width() {
        return image_width;
    }

    int get_image_height() {
        return image_height;
    }

    unsigned char* get_image() {
        return image;
    }

    void releaseImage();

};


#endif //LEARNINGCPP_IMAGELOADER_HPP
