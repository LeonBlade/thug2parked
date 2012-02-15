#include "draw.h"

void draw_quad(Quad quad, float x, float y, float z)
{
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		GLfloat color[4];
		glGetFloatv(GL_CURRENT_COLOR, color);
		glBegin(GL_QUADS);
		{
			glNormal3i(0, 0, 1);
			glColor4f(quad.tlc.x, quad.tlc.y, quad.tlc.z, quad.tlc.w);
			glVertex3f(quad.tl.x, quad.tl.y, quad.tl.z);
			glTexCoord2i(quad.tlt.x, quad.tlt.y);

			glColor4f(quad.trc.x, quad.trc.y, quad.trc.z, quad.trc.w);
			glVertex3f(quad.tr.x, quad.tr.y, quad.tr.z);
			glTexCoord2i(quad.trt.x, quad.trt.y);

			glColor4f(quad.brc.x, quad.brc.y, quad.brc.z, quad.brc.w);
			glVertex3f(quad.br.x, quad.br.y, quad.br.z);
			glTexCoord2i(quad.brt.x, quad.brt.y);

			glColor4f(quad.blc.x, quad.blc.y, quad.blc.z, quad.blc.w);
			glVertex3f(quad.bl.x, quad.bl.y, quad.bl.z);
			glTexCoord2i(quad.blt.x, quad.blt.y);
		}
		glEnd();
		glColor4fv(color);
	}
	glPopMatrix();
}

void draw_qube(Cube cube, float x, float y, float z)
{
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		GLfloat color[4];
		glGetFloatv(GL_CURRENT_COLOR, color);

		draw_quad(cube.front, 0, 0, 0);

		glColor4fv(color);
	}
	glPopMatrix();
}
