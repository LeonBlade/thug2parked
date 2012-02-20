#ifndef __FRUSTRUM_H__
#define __FRUSTRUM_H__

struct Plane
{
	Vector3f normal;
	Vector3f point;
	float distance;
};

struct Box
{
	Vector3f vertex[8];
};

static enum { OUTSIDE, INTERSECT, INSIDE };

Plane plane[6];
Vector3f ntl, ntr, nbl, nbr;
Vector3f ftl, ftr, fbl, fbr;
float near_distance;
float far_distance;
float ratio;
float angle;
float tang;
float near_width;
float near_height;
float far_width;
float far_height;

void set_cam_internals(float, float, float, float);
void set_cam_def(Vector3f&, Vector3f&, Vector3f&);
int point_in_frustrum(Vector3f&);
int sphere_in_frustrum(Vector3f&, float);
int box_in_frustrum(Box&);
Plane set_points(Vector3f&, Vector3f&, Vector3f&);

#endif
