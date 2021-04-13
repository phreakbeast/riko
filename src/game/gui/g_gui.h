#ifndef _G_GUI_H
#define _G_GUI_H

#include "../../tek/utils/tek_core.h"
#include "../../tek/graphics/tek_color.h"
#include "../../tek/graphics/tek_font.h"
#include "../../tek/graphics/tek_spritebatch.h"

typedef struct gui_colors_s
{
	Color background;
	Color text;
	Color widget;
	Color hover;
	Color pressed;
	Color disabled;
}GuiColors;

typedef enum gui_state_s
{
	GUI_STATE_NORMAL,
	GUI_STATE_HOVER,
	GUI_STATE_PRESSED,
	GUI_STATE_DISABLED	
}GuiState;

typedef struct gui_button_s
{
	float x;
	float y;
	float width;
	float height;
	char text[64];
	GuiState state;

	void (*on_click)(void);	
}GuiButton;

void gui_button_init(GuiButton* btn);
void gui_button_update(GuiButton* btn, float delta, Rect parent_rect);

typedef struct gui_image_s
{
	Texture* texture;
	Rect source;
	float x;
	float y;
	float width;
	float height;	
}GuiImage;

void gui_image_init(GuiImage* image);
void gui_image_update(GuiImage* image, float delta, Rect parent_rect);

typedef struct gui_text_s
{
	float x;
	float y;
	char text[256];	
}GuiText;

void gui_text_init(GuiText* text);
void gui_text_set_text(GuiText* text, const char* txt);
void gui_text_update(GuiText* text, float delta, Rect parent_rect);

typedef struct gui_checkbox_s
{
	float x;
	float y;
	float width;
	float height;
	bool checked;	
}GuiCheckbox;

void gui_checkbox_init(GuiCheckbox* checkbox);
void gui_checkbox_update(GuiCheckbox* checkbox, float delta, Rect parent_rect);

typedef struct gui_slider_s
{
	float x;
	float y;
	float width;
	float height;

	int min_value;
	int max_value;
	int cur_value;
}GuiSlider;

void gui_slider_init(GuiSlider* slider);
void gui_slider_update(GuiSlider* slider, float delta, Rect parent_rect);

typedef enum gui_widget_type_e
{
	GUI_WIDGET_BUTTON,
	GUI_WIDGET_IMAGE,
	GUI_WIDGET_TEXT,
	GUI_WIDGET_CHECKBOX,
	GUI_WIDGET_SLIDER,
}GuiWidgetType;

typedef struct widget_s
{
	GuiWidgetType type;
	GuiButton* button;
	GuiImage* image;
	GuiText* text;
	GuiCheckbox* checkbox;
	GuiSlider* slider;
}GuiWidget;

GuiWidget* gui_widget_create_button();
GuiWidget* gui_widget_create_image();
GuiWidget* gui_widget_create_text();
GuiWidget* gui_widget_create_checkbox();
GuiWidget* gui_widget_create_slider();
void gui_widget_destroy(GuiWidget* widget);
void gui_widget_update(GuiWidget* widget, float delta, Rect parent_rect);

typedef struct gui_frame_s
{
	float x;
	float y;
	float width;
	float height;

	GuiWidget** widgets;
	u32 num_widgets;	
}GuiFrame;

GuiFrame* gui_frame_create();
void gui_frame_destroy(GuiFrame* frame);
void gui_frame_update(GuiFrame* frame, float delta);
void gui_frame_add_widget(GuiFrame* frame, GuiWidget* widget);

bool gui_init();
void gui_destroy();
void gui_update(float delta);
BitmapFont* gui_get_font();
Texture* gui_get_texture();
GuiColors* gui_get_colors();
void gui_add_frame(GuiFrame* frame);

#endif