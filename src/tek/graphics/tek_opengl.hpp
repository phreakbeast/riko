#ifndef _TEK_OPENGL_HPP
#define _TEK_OPENGL_HPP


#ifdef TEK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLext.h>
#define WGL_WGLEXT_PROTOTYPES
#include <GL/wglext.h>
#else

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glxext.h>

#endif


#ifdef TEK_DEBUG
#define GLCall(x) x; _check_gl_error(#x, __FILE__, __LINE__)
#else
#define GLCall(x) x
#endif


void _check_gl_error(const char *function, const char *file, int line);

void print_gl_string(GLenum name, const char *label);


#define GL_FUNCTION(x, y) extern PFN## x ##PROC y;

#include "gl_functions.inl"


void load_gl_extensions();

#endif
