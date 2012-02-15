#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gl.h"

struct Camera
{
	float x;
	float y;
	float z;
	float lookX;
	float lookY;
	float lookZ;
};

void camera(Camera &camera, float x, float y, float z, float lx, float ly, float lz);
void camera_draw(Camera &camera);
void camera_input(Camera &camera, int key);
void camera_input(Camera &camera, char key);

#endif
