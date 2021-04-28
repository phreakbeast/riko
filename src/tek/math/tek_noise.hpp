#ifndef _TEK_NOISE_HPP
#define _TEK_NOISE_HPP

#include "../utils/tek_core.hpp"

typedef struct noise_t
{
    u32 width;
    u32 height;
    float *values;
    u32 num_values;        
}Noise;


void noise_init(Noise* noise, u32 width, u32 height);
void noise_destroy(Noise* noise);
float noise_get_value(Noise* noise, u32 x, u32 y);

#endif
