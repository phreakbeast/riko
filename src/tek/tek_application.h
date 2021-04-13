#ifndef _TEK_APPLICATION_H
#define _TEK_APPLICATION_H

#include "utils/tek_core.h"
#include "utils/tek_stopwatch.h"

void application_destroy();

bool application_init(u32 width, u32 height, const char *title, bool fullscreen);

void application_start_mainloop();

void application_set_init_callback(bool(*f)(u32, u32));
void application_set_update_callback(void(*f)(float));
void application_set_resize_callback(void(*f)(u32, u32));

#endif
