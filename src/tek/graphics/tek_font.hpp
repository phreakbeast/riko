#ifndef _TEK_FONT_HPP
#define _TEK_FONT_HPP

#include "../utils/tek_core.hpp"
#include "tek_texture.hpp"

typedef struct font_letter_s
{
    char character;
    float uv_l;
    float uv_r;
    float uv_t;
    float uv_b;
    int width;
    int x_offset;
    int y_offset;
}FontLetter;

typedef struct kerning_table_S
{
    int val[512][512];
}KerningTable;

typedef struct bitmap_font_S
{
    Texture* texture;
    KerningTable kernings;
    FontLetter letters[512];
    int num_letters;
    int width;
    int height;        
}BitmapFont;


void bitmapfont_destroy(BitmapFont* font);

const FontLetter* bitmapfont_get_letter(BitmapFont* font, char character);

const u32 bitmapfont_text_length(BitmapFont* font, const char* text);

const u32 bitmapfont_text_height(BitmapFont* font, const char* text);

BitmapFont* bitmapfont_load(const char* filename);

#endif
