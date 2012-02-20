#include "frustum.h"
#include <math.h>

#define PI_RAD 3.14159265358979323846 / 180.0

enum { TOP = 0, BOTTOM, LEFT, RIGHT, NEARP, FARP };

void set_cam_internals(float _angle, float _ratio, float _near_distance, float _far_distance)
{
	radio = _radio;
	angle = _angle;
	near_distance = _near_distance;
	far_distance = _far_distance;

	tang = (float)tan(PI_RAD * angle * 0.5);
	near_height = near_distance * tang;
	near_width = near_height * ratio;
	far_height = far_distance * tang;
	far_width = far_height * ratio;
}

void set_cam_def(Vector3f& position, Vector3f& look, Vector3f& up)
{
	Vector3f dir;
	Vector3f near_center;
	Vector3f far_center;
	Vector3f X;
	Vector3f Y;
	Vector3f Z;

	// compute Z axis of camera
	Z = vector_sub(position, look);
	Z = vector_normalize(Z);

	// X axis of camera with given up vector and Z axis
	X = vector_cross(up, Z);
	X = vector_normalize(X);

	// the real up vector is the cross product of Z and X
	Y = vector_cross(Z, X);

	// compute the centers of the near and far planes
	near_center = vector_scalar(vector_sub(position, Z), near_distance);
	far_center = vector_scalar(vector_sub(position, Z), far_distance);

	// compute the 4 corners of the frustum on the near plane
	ntl = vector_scalar(vector_cross(vector_add(near_center, Y), vector_sub(X, near_height), near_width);
	ntr = vector_scalar(vector_cross(vector_add(near_center, Y), vector_add(X, near_height), near_width);
	nbl = vector_scalar(vector_cross(vector_sub(near_center, Y), vector_sub(X, near_height), near_width);
	nbr = vector_scalar(vector_cross(vector_sub(near_center, Y), vector_add(X, near_height), near_width);

	// compute the 4 corners of the frustum on the far plane
	ftl = vector_scalar(vector_cross(vector_add(far_center, Y), vector_sub(X, far_height), far_width);
	ftr = vector_scalar(vector_cross(vector_add(far_center, Y), vector_add(X, far_height), far_width);
	fbl = vector_scalar(vector_cross(vector_sub(far_center, Y), vector_sub(X, far_height), far_width);
	fbr = vector_scalar(vector_cross(vector_sub(far_center, Y), vector_add(X, far_height), far_width);

	// compute the six planes
	// the functions set3Points assumes that the points are given in counter clockwise order
	plane[TOP] = set_points(ntr, ntl, ftl);
	plane[BOTTOM] = set_points(nbl, nbr, fbr);
	plane[LEFT] = set_points(ntl, nbl, fbl);
	plane[RIGHT] = set_points(nbr, ntr, fbr);
	plane[NEARP] = set_points(ntl, ntr, nbr);
	plane[FARP] = set_points(ftr, ftl, fbl);
}

int point_in_frustrum(Vector3f& a)
{
	for (int i = 0; i < 6; i++)
		if (plane_distance(plane[i], a) < 0)
			return OUTSIDE;
	return INSIDE;
}

int sphere_in_frustrum(Vector3f& a, float radius)
{
	float distance;
	int result = INSIDE;
	for (int i = 0; i < 6; i++)
	{
		distance = plane_distance(plane[i], a);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result = INTERSECT;
	}
	return result;
}

int box_in_frustrum(Box& a)
{
	int result = INSIDE;
	int out;
	int in;

	for (int i = 0; i < 6; i++)
	{
		// reset counters for corners in and out
		out = 0; in = 0;
		// for each corner of the box do...
		// get out of cycle as soon as a box has corners both inside and out
		for (int k = 0; k < 8 && (in == 0 || out == 0); k++)
		{
			if (plane_distance(plane[i], a.vertex[k])) < 0)
				out++;
			else
				in++;
		}
		if (!in) return OUTSIDE;
		else if (out) result = INTERSECT;
	}
	return result;
}

Plane set_points(Vector3f& a, Vector3f& b, Vector3f& c)
{
	Plane p;
	Vector3f d;
	Vector3f e;

	d =	vector_sub(a, b);
	e = vector_sub(b, c);

	p.normal = vector_cross(e, d);
	p.normal = vector_normalize(p.normal);
	p.point = b;
	p.distance = -vector_dot(p.point);
	return p;
}

float plane_distance(Plane &p, Vector3f &a)
{
	return p.distance + vector_dot(p.normal, a);
}
