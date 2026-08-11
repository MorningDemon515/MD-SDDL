#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/Math.h"

typedef struct Camera
{
	float near, far, fov, aspect;
	Vector right, up, front;
	Vector _1font;

	Vector pos;

	Matrix projection, view;
} Camera;

#endif