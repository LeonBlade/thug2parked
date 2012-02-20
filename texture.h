#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#define TEXTURE_LOAD_ERROR false

#include "gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpng/png.h>

struct Texture
{
	GLuint texture;
	int width;
	int height;
};

bool load_texture(Texture &_texture, const char *filename, bool alpha = true, bool clamp = false);

#endif
