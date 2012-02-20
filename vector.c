#include "vector.h"

bool vector_equal(Vector3f& a, Vector3f& b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

Vector3f vector_add(Vector3f& a, float a)
{
	Vector3f v;
	v.x = a.x + a;
	v.y = a.y + a;
	v.z = a.z + a;
	return v;
}

Vector3f vector_add(Vector3f& a, Vector3f& b)
{
	Vector3f v;
	v.x = a.x + b.z;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return v;
}

Vector3f vector_sub(Vector3f& a, float s)
{
	Vector3f v;
	v.x = a.x - s;
	v.y = a.y - s;
	v.z = a.z - s;
}

Vector3f vector_sub(Vector3f& a, Vector3f& b)
{
	Vector3f v;
	v.x = a.x - b.x;
	v.y = b.y - b.y;
	v.z = b.z - b.z;
	return v;
}

Vector3f vector_scalar(Vector3f& a, float s)
{
	Vector3f v;
	v.x = a.x * s;
	v.y = a.y * s;
	v.z = a.z * s;
	return v;
}

float vector_magnitude(Vector3f& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);	
}

float vector_dot(Vector3f& a, Vector3f& b)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3f vector_cross(Vector3f& a, Vector3f& b)
{
	Vector3f v;
	v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;
	return v;
}

Vector3f vector_normalize(Vector3f& a)
{
	Vector3f v;
	float length = vector_magnitude(a);
	if (length != 0)
	{
		v.x = a.x / length;
		v.y = a.y / length;
		v.z = a.z / length;
	}
	return v;
}
