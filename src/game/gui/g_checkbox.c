#include "g_gui.h"

#include "../../tek/platform/tek_input.h"

void gui_checkbox_init(GuiCheckbox* checkbox)
{
	checkbox->x = 0;
	checkbox->y = 0;
	checkbox->width = 10;
	checkbox->height = 10;
	checkbox->checked = false;
}

void gui_checkbox_update(GuiCheckbox* checkbox, float delta, Rect parent_rect)
{
	GuiColors* colors = gui_get_colors();
	Rect dest = { checkbox->x + parent_rect.x, checkbox->y + parent_rect.y, checkbox->width, checkbox->height };
	MouseState* mstate = input_get_mouse_state();

	if (rect_is_inside(dest, mstate->mx, mstate->my))
	{
		if (mstate->buttons_down[MOUSE_BUTTON_LEFT])
		{
			if (mstate->buttons_pressed[MOUSE_BUTTON_LEFT])
			{
				//click
				printf("check\n");
				checkbox->checked = !checkbox->checked;
			}
		}
		else
		{
			//state = State::Hover;
		}
	}
	else
	{
		//state = State::Normal;
	}



	sb_render_rect(dest, colors->widget);

	if (checkbox->checked)
	{
		Rect check_rect = { dest.x + 2, dest.y + 2, dest.w - 4, dest.h - 4 };
		sb_render_rect(check_rect, colors->pressed);
	}
}
