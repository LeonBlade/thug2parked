#include "font.h"
#include <string.h>

void draw_string(const char *string, float x, float y, float z)
{
	float dx = x;
	float dy = y;
	for (uint i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\n') { dy++; dx = 0; } else
		if (string[i] == ' ') { dx++; } else 
			//draw_quad(chars[(int)string[i]], dx, dy, 0.0f);
		dx++;
	}
}
