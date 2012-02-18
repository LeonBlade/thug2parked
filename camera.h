#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gl.h"
#include <math.h>

struct Camera
{
	float x;
	float y;
	float z;
	float yaw;
	float pitch;
	float roll;
};

void camera_new(Camera &camera, float x, float y, float z, float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f);
void camera_draw(Camera &camera, int delta);
void camera_input(Camera &camera, int key, bool state);
void camera_input(Camera &camera, unsigned char key, bool state);

#endif
