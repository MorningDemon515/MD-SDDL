#ifndef QUATERNION_H
#define QUATERNION_H

#include "Matrix.h"

typedef struct Quat
{
    float a, b, c, d;
    //i^2 = j^2 = k^2 = i * j * k = -1
    //a + i * b + j * c + k *d;

    Quat() : a(1.0f), b(0.0f), c(0.0f), d(0.0f) {}
    Quat(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    Quat(Vector v) : a(0.0f), b(v.x), c(v.y), d(v.z) {}
    Quat(float angle, Vector axis);

    Quat operator + (const Quat& q2);

    Quat operator * (const Quat& q2);

    Quat operator * (const float& s);

    Matrix ToMat();

    Quat operator - ();

    Quat Normalize();

} Quat;

inline Quat operator * (float s, const Quat& v)
{
    return const_cast<Quat&>(v) * s;
}

Vector VecRotate(Quat q, Vector v);

Quat NLerp(Quat q1, Quat q2, float t);
Quat Slerp(Quat q1, Quat q2, float t);

#endif