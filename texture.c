#include "texture.h"

bool load_texture(Texture &texture, const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("Failed to load \"%s\"\n.", filename);
		return false;
	}
	char *buffer = (char*)malloc(32 * 32 * 3);
	fread(buffer, 32 * 32 * 3, 1, f);
	fclose(f);

	//Now generate the OpenGL texture object
	glGenTextures(1, &texture.texture);
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 32, 32, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	printf("Loaded texture '%s' @ %i\n", filename, texture.texture);

	return true;
}


