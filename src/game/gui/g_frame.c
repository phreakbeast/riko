#include "g_gui.h"

#include <assert.h>
#include <stdlib.h>

GuiFrame* gui_frame_create()
{
	GuiFrame* frame = malloc(sizeof(GuiFrame));
	assert(frame);

	frame->x = 0;
	frame->y = 0;
	frame->width = 100;
	frame->height = 100;

	frame->num_widgets = 0;
	frame->widgets = NULL;

	return frame;
}

void gui_frame_destroy(GuiFrame* frame)
{
	for (u32 i = 0; i < frame->num_widgets; ++i)
	{		
		gui_widget_destroy(frame->widgets[i]);
	}
	free(frame->widgets);
}

void gui_frame_update(GuiFrame* frame, float delta)
{
	GuiColors* colors = gui_get_colors();
	Rect dest = { frame->x, frame->y, frame->width, frame->height };
	sb_render_rect(dest, colors->background);

	for (u32 i=0; i<frame->num_widgets;++i)
	{
		gui_widget_update(frame->widgets[i], delta, dest);
	}
}


void gui_frame_add_widget(GuiFrame* frame, GuiWidget* widget)
{
	if (frame->num_widgets == 0)
	{
		frame->num_widgets = 1;
		frame->widgets = malloc(sizeof(GuiWidget*) * frame->num_widgets);
		assert(frame->widgets);
		frame->widgets[0] = widget;
	}
	else
	{
		frame->num_widgets++;
		frame->widgets = realloc(frame->widgets, sizeof(GuiWidget*) * frame->num_widgets);
		assert(frame->widgets);
		frame->widgets[frame->num_widgets-1] = widget;
	}
}