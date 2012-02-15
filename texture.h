#ifndef __SPRITES_H__
#define __SPRITES_H__

#include "gl.h"
#include <stdio.h>
#include <stdlib.h>

struct Texture
{
	GLuint texture;
	int width;
	int height;
};

bool load_texture(Texture &texture, const char *filename);

#endif
