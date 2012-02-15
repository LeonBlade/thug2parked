#ifndef __DRAW_H__
#define __DRAW_H__

#include "gl.h"

struct Vector2f 
{
	float x;
	float y;
};

struct Vector3f 
{
	float x;
	float y;
	float z;	
};

struct Vector4f 
{
	float x;
	float y;
	float z;
	float w;	
};

struct Quad
{
	Vector3f tl;
	Vector2f tlt;
	Vector4f tlc;

	Vector3f tr;
	Vector2f trt;
	Vector4f trc;

	Vector3f br;
	Vector2f brt;
	Vector4f brc;

	Vector3f bl;
	Vector2f blt;
	Vector4f blc;
};

struct Cube
{
	Quad above;
	Quad under;
	Quad front;
	Quad back;
	Quad left;
	Quad right;
};

void draw_quad(Quad quad, float x = 0, float y = 0, float z = 0);
void draw_qube(Cube cube, float x = 0, float y = 0, float z = 0);

#endif
