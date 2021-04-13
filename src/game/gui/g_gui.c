#include "g_gui.h"

#include <stdlib.h>
#include <assert.h>

static BitmapFont* g_font = NULL;
static GuiFrame** g_frames;
static u32 g_num_frames;
static GuiColors g_colors;
static Texture* g_texture;

bool gui_init()
{
	g_font = bitmapfont_load("data/fonts/test.font");
	if (!g_font)
		return false;

	g_texture = texture_load("data/textures/gui.tga", TEXTURE_FILTER_NEAREST);
	if (!g_texture)
		return false;

	g_colors.background = color_create(100, 100, 100);
	g_colors.text = color_create(255, 255, 255);
	g_colors.widget = color_create(130, 130, 130);
	g_colors.hover = color_create(150, 150, 100);
	g_colors.pressed = color_create(100, 130, 150);
	g_colors.disabled = color_create(100, 100, 100);

	g_num_frames = 0;
	g_frames = NULL;

	return true;
}

void gui_destroy()
{
	bitmapfont_destroy(g_font);
	free(g_font);
	for (u32 i = 0; i < g_num_frames; ++i)
	{
		gui_frame_destroy(g_frames[i]);
	}
	free(g_frames);
}

void gui_update(float delta)
{
	for (u32 i=0; i<g_num_frames;++i)
	{
		gui_frame_update(g_frames[i], delta);
	}
}

BitmapFont* gui_get_font()
{
	return g_font;
}

GuiColors* gui_get_colors()
{
	return &g_colors;
}

Texture* gui_get_texture()
{
	return g_texture;
}

void gui_add_frame(GuiFrame* frame)
{
	if (g_num_frames == 0)
	{
		g_num_frames = 1;
		g_frames = malloc(sizeof(GuiFrame*) * g_num_frames);
		assert(g_frames);
		g_frames[0] = frame;
	}
	else
	{
		g_num_frames++;
		g_frames = realloc(g_frames, sizeof(GuiFrame*) * g_num_frames);
		assert(g_frames);
		g_frames[g_num_frames-1] = frame;
	}
}

