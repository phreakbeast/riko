#include "g_gui.hpp"

#include "../../tek/platform/tek_input.hpp"

#include <stdio.h>
#include <string.h>

void gui_button_init(GuiButton* btn)
{
	btn->x = 0;
	btn->y = 0;
	btn->width = 100;
	btn->height = 100;
	strcpy(btn->text, "Button");
	btn->state = GUI_STATE_NORMAL;
	btn->on_click = NULL;
}

void gui_button_update(GuiButton* btn, float delta, Rect parent_rect)
{
	GuiColors* colors = gui_get_colors();
	BitmapFont* font = gui_get_font();

	MouseState* mstate = input_get_mouse_state();
	Rect dest = { btn->x + parent_rect.x, btn->y + parent_rect.y, btn->width, btn->height };
	if (rect_is_inside(dest, mstate->mx, mstate->my))
	{
		if (mstate->buttons_down[MOUSE_BUTTON_LEFT])
		{
			if (mstate->buttons_pressed[MOUSE_BUTTON_LEFT])
			{
				//click
				printf("click\n");
			}
			btn->state = GUI_STATE_PRESSED;
		}
		else
		{
			btn->state = GUI_STATE_HOVER;
		}
	}
	else
	{
		btn->state = GUI_STATE_NORMAL;
	}

	switch (btn->state)
	{
	    case GUI_STATE_NORMAL:
		{
			sb_render_rect(dest, colors->widget);
		}break;
	    case GUI_STATE_HOVER:
		{
			sb_render_rect(dest, colors->hover);
		}break;
	    case GUI_STATE_PRESSED:
		{
			sb_render_rect(dest, colors->pressed);
		}break;
	    case GUI_STATE_DISABLED:
		{
			sb_render_rect(dest, colors->disabled);
		}break;
	}
	sb_render_text(btn->text, font, dest.x, dest.y, colors->text, 0);
}
