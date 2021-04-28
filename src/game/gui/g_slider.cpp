#include "g_gui.hpp"

#include "../../tek/platform/tek_input.hpp"

void gui_slider_init(GuiSlider* slider)
{
	slider->x = 0;
	slider->y = 0;
	slider->width = 100;
	slider->height = 10;

	slider->min_value = 0;
	slider->max_value = 100;
	slider->cur_value = 50;
}

void gui_slider_update(GuiSlider* slider, float delta, Rect parent_rect)
{
	GuiColors* colors = gui_get_colors();
	Rect dest = { slider->x + parent_rect.x, slider->y + parent_rect.y, slider->width, slider->height };
	MouseState* mstate = input_get_mouse_state();

	sb_render_rect(dest, colors->widget);

	Rect line_dest = { dest.x + 2,dest.y + dest.h / 2 - 2, dest.w - 4, 4 };

	sb_render_rect(line_dest, colors->hover);

	Rect grab_dest = { dest.x, dest.y, 5, dest.h };

	sb_render_rect(grab_dest, colors->pressed);
}
