#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

struct Vector3f 
{
	float x;
	float y;
	float z;	
};

bool vector_equal(Vector3f&, Vector3f&);
Vector3f vector_add(Vector3f&, float)
Vector3f vector_add(Vector3f&, Vector3f&);
Vector3f vector_sub(Vector3f&, float);
Vector3f vector_sub(Vector3f&, Vector3f&);
Vector3f vector_scalar(Vector3f&, float);
float vector_magnitude(Vector3f&);
float vector_dot(Vector3f&, Vector3f&);
Vector3f vector_cross(Vector3f&, Vector3f&);
Vector3f vector_normalize(Vector3f&);

#endif
