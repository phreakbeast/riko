#ifndef _TEK_IMAGE_HPP
#define _TEK_IMAGE_HPP

#include "../utils/tek_core.hpp"
#include "../math/tek_noise.hpp"
#include "tek_color.hpp"
#include <stdio.h>

typedef enum image_format_e
{
    IMAGE_FORMAT_GRAY,
    IMAGE_FORMAT_RGB,
    IMAGE_FORMAT_RGBA
}ImageFormat;

typedef struct image_s
{
    u32 width;
    u32 height;
    ImageFormat format;
    int internal_format;
    u8 *pixels;
}Image;

void image_destroy(Image* img);

void image_set_pixel(Image* img, int x, int y, Color color);
const Color image_get_pixel(Image* img, int x, int y);
void image_mirror_vertical(Image* img);

Image* image_load(const char* filename);
Image* image_create_blank(int width, int height, Color color);
Image* image_create_checker(Color color1, Color color2, int x_div, int y_div, int width, int height);

//TODO: handle top and bottom origins properly
bool image_load_tga(Image* img, const char* filename);

//TODO: handle 32 bit images
bool image_load_bmp(Image* img, const char* filename);

bool image_save_bmp(Image* img, const char* filename);

#endif
