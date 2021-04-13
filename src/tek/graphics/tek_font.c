#include "tek_font.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void bitmapfont_destroy(BitmapFont* font)
{
    texture_destroy(font->texture);
}

const FontLetter* bitmapfont_get_letter(BitmapFont* font, char character)
{
    //TODO: replace with something faster
    for(int i = 0; i < font->num_letters; ++i)
    {
        if(font->letters[i].character == character)
        {
            return &font->letters[i];
        }
    }
    return NULL;
}

const u32 bitmapfont_text_length(BitmapFont* font, const char* text)
{
    u32 res = 0;
    u32 len = (u32)strlen(text);
    for(u32 i = 0; i < len; ++i)
    {
        char c = text[i];
        const FontLetter* letter = bitmapfont_get_letter(font, c);
        if(letter == NULL)
        {
            letter = bitmapfont_get_letter(font, '?');
        }
        res += letter->width;
    }
    return res;
}

const u32 bitmapfont_text_height(BitmapFont* font, const char* text)
{
    //TODO: implement
    return font->height;
}

BitmapFont* bitmapfont_load(const char* filename)
{
    printf("loading font %s\n", filename);
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }
    
    BitmapFont* font = malloc(sizeof(BitmapFont));
    
    font->num_letters = 0;

    char line[1024];
    while(fgets(line, 1024, fp))
    {
        if(line[0] == '\n')
        { continue; }
        if(line[0] == '\r' && line[1] == '\n')
        { continue; }
        if(line[0] == '\0')
        { continue; }
        
        if(line[0] == '#')
        { continue; }
        
        if(line[0] == 'h' && line[1] == 'd' && line[2] == 'r')
        {
            char tex_name[256];
            int width;
            int height;
            
            int res = sscanf(line, "hdr %s %d %d", tex_name, &width, &height);
            if(res < 3)
            {
                printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
            }
            
            font->texture = texture_load(tex_name, TEXTURE_FILTER_LINEAR);
            if(!font->texture)
            {
                fclose(fp);
                bitmapfont_destroy(font);
                free(font);
                return NULL;
            }
            
            font->width = width;
            font->height = height;
        }
        else if(line[0] == 'l' && line[1] == 't' && line[2] == 'r')
        {
            char character;
            int uv_x;
            int uv_y;
            int uv_w;
            int uv_h;
            int x_offset;
            int y_offset;
            
            int res = sscanf(line, "ltr %c %d %d %d %d %d %d", &character, &uv_x, &uv_y, &uv_w, &uv_h, &x_offset,
                             &y_offset);
            if(res < 7)
            {
                printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
            }
            
            FontLetter letter;
            letter.character = character;
            letter.width = uv_w;
            letter.x_offset = x_offset;
            letter.y_offset = y_offset;
            
            int tex_w = font->texture->width;
            int tex_h = font->texture->height;
            assert(tex_w != 0 && tex_h != 0);
            
            letter.uv_l = uv_x / (float)tex_w;
            letter.uv_r = letter.uv_l + uv_w / (float)tex_w;
            letter.uv_t = (tex_h - uv_y) / (float)tex_h;
            letter.uv_b = letter.uv_t - uv_h / (float)tex_h;
            
            
            font->letters[font->num_letters++] = letter;
        }
        else if(line[0] == 'k' && line[1] == 'r' && line[2] == 'n')
        {
            char letter1;
            char letter2;
            int val;
            
            int res = sscanf(line, "krn %c %c %d", &letter1, &letter2, &val);
            if(res < 3)
            {
                printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
            }
            
            font->kernings.val[(int)letter1][(int)letter2] = val;
        }
        else
        {
            printf("WARNING: cannot parse values in line %s. File: %s\n", line, filename);
        }
    }
    
    fclose(fp);
    
    //insert space
    FontLetter letter;
    letter.character = ' ';
    letter.width = font->width;
    letter.x_offset = 0;
    letter.y_offset = 0;
    
    letter.uv_l = 0;
    letter.uv_r = 0;
    letter.uv_t = 0;
    letter.uv_b = 0;
    assert(font->num_letters < 512);
    font->letters[font->num_letters++] = letter;
    
    return font;
}
