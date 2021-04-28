#ifndef _PERLIN_HPP
#define _PERLIN_HPP

#include "tek_math.hpp"

typedef struct perlin1d_t
{    
    float* seed;
    float* noise_values;
    int output_size;
}Perlin1D;

void perlin1d_init(Perlin1D* perlin, int size, int octaves, float bias);
void perlin1d_destroy(Perlin1D* perlin);

typedef struct perlin2d_t
{    
    float* seed;
    float* noise_values;
    int output_width;
    int output_height;
    float min_value;
    float max_value;
    float center_value;
}Perlin2D;

void perlin2d_init(Perlin2D* perlin, int width, int height, int octaves, float bias);
void perlin2d_destroy(Perlin2D* perlin);

void perlin2d_update(Perlin2D* perlin, int octaves, float bias);
float perlin2d_get_value(Perlin2D* perlin, int x, int y);

#endif