#include "font.h"
#include <string.h>

Quad chars[256];

Quad* get_chars()
{
	return chars;
}

void init_font()
{
	FILE *f = fopen("font", "rb");
	if (f != NULL)
	{
		fread(chars, sizeof(Quad), 256, f);
		fclose(f);
		printf("Using font file.\n");
		//return;
	}

	int x = 0;
	int y = -1;

	for (int i = 0; i < 256; ++i)
	{
		if (i % 16) { y++; x = 0; }

		Vector3f tl = {  0.0f,  0.0f, 0.0f };
		Vector3f tr = { 32.0f,  0.0f, 0.0f };
		Vector3f br = { 32.0f, 32.0f, 0.0f };
		Vector3f bl = {  0.0f, 32.0f, 0.0f };

		Vector2f tlt = {  x * 32, 		  y * 32,		   };
		Vector2f trt = { (x * 32) + 32,   y * 32, 		   };
		Vector2f brt = { (x * 32) + 32,  (y * 32) + 32 };
		Vector2f blt = {  x * 32, 		 (y * 32) + 32 };

		Vector4f c = { 1.0f, 1.0f, 1.0f, 1.0f };

		Quad quad =
		{
			tl, tlt, c,
			tr, trt, c,
			br, brt, c,
			bl, blt, c
		};

		chars[i] = quad;
		x++;
	}

	f = fopen("font", "wb");
	fwrite(chars, 256, sizeof(Quad), f);
	fclose(f);

	printf("Saved font file.\n");
}

void draw_string(const char *string, float x, float y, float z)
{
	float dx = x;
	float dy = y;
	for (uint i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\n') { dy++; dx = 0; } else
		if (string[i] == ' ') { dx++; } else 
			draw_quad(chars[(int)string[i]], dx, dy, 0.0f);
		dx++;
	}
}
