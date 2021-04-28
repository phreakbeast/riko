#include "tek_image.hpp"

#include "../utils/tek_core.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void image_destroy(Image* img)
{
    free(img->pixels);
    img->pixels = NULL;
}

void image_set_pixel(Image* img, int x, int y, Color color)
{
    img->pixels[(x + y * img->width) * img->internal_format + 0] = color.r;
    img->pixels[(x + y * img->width) * img->internal_format + 1] = color.g;
    img->pixels[(x + y * img->width) * img->internal_format + 2] = color.b;
    if (img->internal_format == 4)
        img->pixels[(x + y * img->width) * img->internal_format + 3] = color.a;
}

const Color image_get_pixel(Image* img, int x, int y)
{
    u8 r, g, b;
    u8 a = 255;
    r = img->pixels[(x + y * img->width) * img->internal_format + 0];
    g = img->pixels[(x + y * img->width) * img->internal_format + 1];
    b = img->pixels[(x + y * img->width) * img->internal_format + 2];
    if (img->internal_format == 4)
        a = img->pixels[(x + y * img->width) * img->internal_format + 3];

    return color_create_alpha(r, g, b, a);
}

Image* image_load(const char* filename)
{
    Image *img = (Image*)malloc(sizeof(Image));

    bool success = false;
    char *ext = get_file_extension(filename);

    if (strcmp(ext, "tga") == 0) 
    {
        success = image_load_tga(img, filename);
    } else if (strcmp(ext, "bmp") == 0) 
    {
        success = image_load_bmp(img, filename);
    } 
    else 
    {
        free(ext);
        free(img);
        return NULL;
    }

    free(ext);

    if (!success)
    {
        free(img);
        return NULL;
    }

    return img;
}

Image* image_create_blank(int width, int height, Color color)
{
    Image *img = (Image*)malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->format = IMAGE_FORMAT_RGB;
    img->internal_format = 3;
    img->pixels = (u8*)malloc(sizeof(u8) * width * height * img->internal_format);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            image_set_pixel(img, x, y, color);
        }
    }

    return img;
}

void draw_checker(Image *img, Color col, int xmin, int xmax, int ymin, int ymax)
{
    int mid_x = xmin + ((xmax - xmin) / 2);
    int mid_y = ymin + ((ymax - ymin) / 2);

    for (int y = ymin; y < mid_y; y++)
    {
        for (int x = xmin; x < mid_x; x++)
        {
            image_set_pixel(img, x, y, col);
        }
    }

    for (int y = mid_y; y < ymax; y++)
    {
        for (int x = mid_x; x < xmax; x++)
        {
            image_set_pixel(img, x, y, col);
        }
    }
}

Image* image_create_checker(Color color1, Color color2, int x_div, int y_div, int width, int height)
{
    Image *img = image_create_blank(width, height, color2);

    int part_width = width / (x_div);
    int part_height = height / (y_div);

    for (int dy = 0; dy < y_div; dy++)
    {
        for (int dx = 0; dx < x_div; dx++)
        {
            int xmin = dx * part_width;
            int xmax = xmin + part_width;
            int ymin = dy * part_height;
            int ymax = ymin + part_height;

            draw_checker(img, color1, xmin, xmax, ymin, ymax);
        }
    }

    return img;
}

void image_mirror_vertical(Image* img)
{
    u8 *tmp_pixels = (u8*)malloc(sizeof(u8) * img->width * img->height * img->internal_format);
    //memcpy(tmp_pixels,pixels,sizeof(u8) * width * height * internal_format);

    for (u32 y = 0; y < img->height; ++y) 
    {
        for (u32 x = 0; x < img->width; ++x) 
        {
            u32 y_pos = img->height - y;
            Color pix = image_get_pixel(img, x, y_pos);

            tmp_pixels[(x + y * img->width) * img->internal_format + 0] = pix.r;
            tmp_pixels[(x + y * img->width) * img->internal_format + 1] = pix.g;
            tmp_pixels[(x + y * img->width) * img->internal_format + 2] = pix.b;
            if (img->internal_format == 4)
                tmp_pixels[(x + y * img->width) * img->internal_format + 3] = pix.a;
        }
    }
    memcpy(img->pixels, tmp_pixels, sizeof(u8) * img->width * img->height * img->internal_format);
    free(tmp_pixels);
}
