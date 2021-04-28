#include "g_gui.hpp"

void gui_image_init(GuiImage* image)
{
	image->texture = NULL;
	image->x = 0;
	image->y = 0;
	image->width = 100;
	image->height = 100;
}

void gui_image_update(GuiImage* image, float delta, Rect parent_rect)
{
	Rect dest = { image->x + parent_rect.x, image->y + parent_rect.y, image->width, image->height };
	sb_render_sprite(dest, image->source, image->texture->id);
}
