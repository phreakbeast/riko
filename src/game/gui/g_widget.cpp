#include "g_gui.hpp"

#include <stdlib.h>
#include <assert.h>

static void widget_init(GuiWidget* widget)
{
    widget->type = GUI_WIDGET_NOTHING;
    widget->button = NULL;
    widget->image = NULL;
    widget->text = NULL;
    widget->checkbox = NULL;
    widget->slider = NULL;
}

void gui_widget_destroy(GuiWidget* widget)
{
    switch(widget->type)
    {
        case GUI_WIDGET_BUTTON:{
            free(widget->button);
        }break;
        case GUI_WIDGET_IMAGE:{
            free(widget->image);
        }break;
        case GUI_WIDGET_TEXT:{
            free(widget->text);
        }break;
        case GUI_WIDGET_CHECKBOX:{
            free(widget->checkbox);
        }break;
        case GUI_WIDGET_SLIDER:{
            free(widget->slider);
        }break;
        default:
            break;
    }
}

GuiWidget* gui_widget_create_button()
{
	GuiWidget* widget = (GuiWidget*)malloc(sizeof(GuiWidget));
    assert(widget);
    widget_init(widget);

	widget->type = GUI_WIDGET_BUTTON;
	widget->button = (GuiButton*)malloc(sizeof(GuiButton));
	gui_button_init(widget->button);

	return widget;
}

GuiWidget* gui_widget_create_image()
{
    GuiWidget* widget = (GuiWidget*)malloc(sizeof(GuiWidget));
    assert(widget);
    widget_init(widget);

	widget->type = GUI_WIDGET_IMAGE;
	widget->image = (GuiImage*)malloc(sizeof(GuiImage));
	gui_image_init(widget->image);

	return widget;
}

GuiWidget* gui_widget_create_text()
{
    GuiWidget* widget = (GuiWidget*)malloc(sizeof(GuiWidget));
    assert(widget);
    widget_init(widget);

	widget->type = GUI_WIDGET_TEXT;
	widget->text = (GuiText*)malloc(sizeof(GuiText));
	gui_text_init(widget->text);

	return widget;
}

GuiWidget* gui_widget_create_checkbox()
{
    GuiWidget* widget = (GuiWidget*)malloc(sizeof(GuiWidget));
    assert(widget);
    widget_init(widget);

	widget->type = GUI_WIDGET_CHECKBOX;
	widget->checkbox = (GuiCheckbox*)malloc(sizeof(GuiCheckbox));
	gui_checkbox_init(widget->checkbox);

	return widget;
}

GuiWidget* gui_widget_create_slider()
{
    GuiWidget* widget = (GuiWidget*)malloc(sizeof(GuiWidget));
    assert(widget);
    widget_init(widget);

	widget->type = GUI_WIDGET_SLIDER;
	widget->slider = (GuiSlider*)malloc(sizeof(GuiSlider));
	gui_slider_init(widget->slider);

	return widget;
}

void gui_widget_update(GuiWidget* widget, float delta, Rect parent_rect)
{
	switch (widget->type)
	{
	    case GUI_WIDGET_BUTTON:
	{
		gui_button_update(widget->button, delta, parent_rect);
	}break;
	    case GUI_WIDGET_IMAGE:
	{
		gui_image_update(widget->image, delta, parent_rect);
	}break;
	    case GUI_WIDGET_TEXT:
	{
		gui_text_update(widget->text, delta, parent_rect);
	}break;
	    case GUI_WIDGET_CHECKBOX:
	{
		gui_checkbox_update(widget->checkbox, delta, parent_rect);
	}break;
	    case GUI_WIDGET_SLIDER:
	{
		gui_slider_update(widget->slider, delta, parent_rect);
	}break;
	default:
		break;
	}
}