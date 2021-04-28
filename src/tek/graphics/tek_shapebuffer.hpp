#ifndef _TEK_SHAPEBUFFER_HPP
#define _TEK_SHAPEBUFFER_HPP

#include "../utils/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "tek_color.hpp"
#include "tek_camera.hpp"

bool sbuffer_init(u64 max_shapes);

void sbuffer_destroy();

void sbuffer_set_cam(Camera *cam);

void sbuffer_begin();

void sbuffer_end();

void sbuffer_flush();

void sbuffer_render_line(Vec3 p0, Vec3 p1, Color color);


#endif
