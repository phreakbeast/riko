#ifndef _TEK_SHAPEBUFFER_H
#define _TEK_SHAPEBUFFER_H

#include "../utils/tek_core.h"
#include "../math/tek_math.h"
#include "tek_color.h"
#include "tek_camera.h"

bool sbuffer_init(u64 max_shapes);

void sbuffer_destroy();

void sbuffer_set_cam(Camera *cam);

void sbuffer_begin();

void sbuffer_end();

void sbuffer_flush();

void sbuffer_render_line(Vec3 p0, Vec3 p1, Color color);


#endif
