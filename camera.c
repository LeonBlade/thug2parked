#include "camera.h"
#include <stdio.h>

#define ANG2RAD 3.14159265358979323846 / 180.0

float dX = 0, dY = 0, dZ = 0, dAX = 0, dAY = 0, dAZ = 0, lookX = 0, lookY = 0, lookZ = 0;
bool gotta_go_fast = false;

void camera_new(Camera &camera, float x, float y, float z, float roll, float pitch, float yaw)
{
	camera.x = x;
	camera.y = y;
	camera.z = z;
	camera.yaw = yaw;
	camera.pitch = pitch;
	camera.roll = roll;

	lookX = sin(camera.yaw); 
	lookY = camera.pitch;
	lookZ = -cos(camera.yaw); 
}

void camera_draw(Camera &camera, int delta)
{
	float moveSpeed = 10.0f * (delta / 1000.0f);
	float cameraSpeed = 1.0f * (delta / 1000.0f);

	// reset values
	if (camera.yaw > 360) camera.yaw -= 360;
	if (camera.pitch > 360) camera.pitch -= 360;

	// moving forward/backwards
	if (dZ)	 
	{
		camera.x += (-dZ * lookX) * moveSpeed; 
		camera.z += (-dZ * lookZ) * moveSpeed; 
	}
	// strafing
	if (dX)	
	{ 
		float _lx = -cos(camera.yaw); 
		float _lz = -sin(camera.yaw); 
		camera.x += (-dX * _lx) * moveSpeed; 
		camera.z += (-dX * _lz) * moveSpeed; 
	}
	// moving up and down
	if (dY)
	{
		camera.y += dY * moveSpeed;
	}

	// looking around
	if (dAX) 
	{ 
		camera.yaw += dAX * cameraSpeed; 
		lookX = sin(camera.yaw); 
		lookZ = -cos(camera.yaw); 
	}
	// looking up/down
	if (dAY) 
	{
		lookY += dAY * cameraSpeed;
	}

	// set camera position
	gluLookAt(
		camera.x, camera.y, camera.z,
		camera.x + lookX, camera.y + lookY, camera.z + lookZ,
		0.0f, 1.0f, 0.0f
	);		
}

void camera_input(Camera &camera, int key, bool state)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:	 dAX = (state)?-1:0; break;
		case GLUT_KEY_RIGHT: dAX = (state)? 1:0; break;
		case GLUT_KEY_UP:	 dAY = (state)? 1:0; break;
		case GLUT_KEY_DOWN:	 dAY = (state)?-1:0; break;
	}
}

void camera_input(Camera &camera, unsigned char key, bool state)
{
	switch (key)
	{
		case 'w': dZ = (state)?-1:0; break;
		case 'a': dX = (state)?-1:0; break;
		case 's': dZ = (state)? 1:0; break;
		case 'd': dX = (state)? 1:0; break;
		case 'q': dY = (state)?-1:0; break;
		case 'e': dY = (state)? 1:0; break;
	}
}
