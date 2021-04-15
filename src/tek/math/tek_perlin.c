#include "tek_perlin.h"
#include <stdlib.h>

void perlin1d_init(Perlin1D* perlin, int size, int octaves, float bias)
{
    perlin->output_size = size;
    perlin->seed = malloc(sizeof(float) * perlin->output_size);
    perlin->noise_values = malloc(sizeof(float) * perlin->output_size);
    
    for(int i=0; i<perlin->output_size; ++i)
    {
        perlin->seed[i] = math_get_random_float(0,1);
    }
    
    for(int i=0; i<perlin->output_size; ++i)
    {
        float noise = 0.0f;
        float scale = 1.0f;
        float scale_acc = 0.0f;
        
        for(int j=0; j<octaves; ++j)
        {
            int pitch = size >> j; //divide  by 2
            int sample1 = (i/pitch) * pitch;
            int sample2 = (sample1 + pitch) % size;
            
            float blend = (float)(i-sample1)/(float)pitch;
            float sample = (1.0f - blend) * perlin->seed[sample1] + blend * perlin->seed[sample2];
            
            noise += sample * scale;
            scale_acc += scale;
            scale /= bias;
        }
        
        perlin->noise_values[i] = noise/scale_acc;
    }
}

void perlin1d_destroy(Perlin1D* perlin)
{
    free(perlin->seed);
    free(perlin->noise_values);
}

void perlin2d_init(Perlin2D* perlin, int width, int height, int octaves, float bias)
{
    perlin->output_width = width;
    perlin->output_height = height;
    int output_size = width * height;
    perlin->seed = malloc(sizeof(float) * output_size);
    perlin->noise_values = malloc(sizeof(float) * output_size);
    
    for(int i=0; i<output_size; ++i)
    {
        perlin->seed[i] = math_get_random_float(0,1);
    }
    perlin->min_value = 2;
    perlin->max_value = 0;
    
    perlin2d_update(perlin, octaves, bias);
}

void perlin2d_update(Perlin2D* perlin, int octaves, float bias)
{
    for(int x=0; x<perlin->output_width; ++x)
    {
        for(int y=0; y<perlin->output_height; ++y)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scale_acc = 0.0f;
            
            for(int j = 0; j < octaves; ++j)
            {
                int pitch = perlin->output_width >> j; //divide  by 2
                int sample1x = (x / pitch) * pitch;
                int sample1y = (y / pitch) * pitch;
                
                int sample2x = (sample1x + pitch) % perlin->output_width;
                int sample2y = (sample1y + pitch) % perlin->output_width;
                
                float blendx = (float)(x - sample1x) / (float)pitch;
                float blendy = (float)(y - sample1y) / (float)pitch;
                
                float sample_t = (1.0f - blendx) * perlin->seed[sample1y * perlin->output_width + sample1x] + blendx * perlin->seed[sample1y * perlin->output_width + sample2x];
                float sample_b = (1.0f - blendx) * perlin->seed[sample2y * perlin->output_width + sample1x] + blendx * perlin->seed[sample2y * perlin->output_width + sample2x];
                
                noise += (blendy * (sample_b - sample_t) + sample_t) * scale;
                scale_acc += scale;
                scale /= bias;
            }
            
            float val = noise / scale_acc;
            perlin->noise_values[y * perlin->output_width + x] = val;
            if(val < perlin->min_value)
            {
                perlin->min_value = val;
            }
            if(val > perlin->max_value)
            {
                perlin->max_value = val;
            }
        }
    }
    
    perlin->center_value = perlin->min_value + (perlin->max_value - perlin->min_value)/2;
}

void perlin2d_destroy(Perlin2D* perlin)
{
    free(perlin->seed);
    free(perlin->noise_values);
}

float perlin2d_get_value(Perlin2D* perlin, int x, int y)
{
    return perlin->noise_values[y * perlin->output_width + x];
}
