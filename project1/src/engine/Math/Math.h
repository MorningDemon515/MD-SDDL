#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <random>

#include "Quaternion.h"

namespace Math
{
	extern float pi;
	extern float epsilon;
};

float ToRad(float angle);
float ToAngle(float rad);

bool Equal(float a, float b);
float Lerp(float x, float y, float a);

#endif