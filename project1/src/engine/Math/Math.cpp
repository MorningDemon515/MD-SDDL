#include "Math.h"

namespace Math
{
	float pi = 3.14159265358979323846f;
	float epsilon = 1.192092896e-07f;
};

float ToRad(float angle)
{
	return angle * 0.01745329251994329577f;// PI / 180.0f
}

float ToAngle(float rad)
{
	return rad * 57.2957795130823208768f; // 180.0f / PI
}

bool Equal(float a, float b)
{
	return ( (std::fabs(a - b)) < Math::epsilon) ? true : false;
}

float Lerp(float x, float y, float a)
{
	return (1.0f - a) * x + a * y;
}