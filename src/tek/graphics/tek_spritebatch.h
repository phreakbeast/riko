#ifndef _TEK_SPRITEBATCH_H
#define _TEK_SPRITEBATCH_H

#include "tek_shader.h"
#include "tek_font.h"
#include "tek_rect.h"
#include "tek_color.h"

bool sb_init(u32 width, u32 height, u64 max_sprites);

void sb_destroy();

void sb_begin();

void sb_end();

void sb_flush();

void sb_resize(u32 width, u32 height);

void sb_reset_stats();

void sb_render_rect(Rect rect, Color color);

void sb_render_sprite(Rect dest, Rect src, u32 texture_id);

void sb_render_polygon(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color color);

void sb_render_text(const char *text, BitmapFont *font, int x, int y, Color color, float max_width);

void sb_render_circle(Vec2 pos, float radius, float start_angle, float end_angle, u32 num_segments, Color color);

void sb_render_line(Vec2 p0, Vec2 p1, float width, Color color);

#endif
