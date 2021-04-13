#include "g_gui.h"

#include <string.h>

void gui_text_init(GuiText* text)
{
	text->text[0] = '\0';
	text->x = 0;
	text->y = 0;
}

void gui_text_update(GuiText* text, float delta, Rect parent_rect)
{
	GuiColors* colors = gui_get_colors();
	BitmapFont* font = gui_get_font();

	float pos_x = text->x + parent_rect.x;
	float pos_y = text->y + parent_rect.y;
	sb_render_text(text->text, font, pos_x, pos_y, colors->text, 0);
}

void gui_text_set_text(GuiText* text, const char* txt)
{
	//TODO: check for overflow
	strcpy(text->text, txt);
}