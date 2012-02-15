#ifndef __FONT_H__
#define __FONT_H__

#include "gl.h"
#include "texture.h"
#include "draw.h"

Quad* get_chars();
void init_font();
void draw_string(const char *string, float x = 0.0f, float y = 0.0f, float z = 0.0f);

#endif
