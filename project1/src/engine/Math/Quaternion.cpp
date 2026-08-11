#include "Math.h"

Quat Quat::operator + (const Quat& q2)
{
    return Quat(this->a + q2.a, this->b + q2.b, this->c + q2.c, this->d + q2.d);
}

Quat Quat::operator * (const Quat& q2)
{
    return Quat(
        a * q2.a - (b * q2.b + c * q2.c + d * q2.d),
        a * q2.b + b * q2.a + c * q2.d - d * q2.c,
        a * q2.c + c * q2.a + d * q2.b - b * q2.d,
        a * q2.d + d * q2.a + b * q2.c - c * q2.b
    );
}

Quat Quat::operator * (const float& s)
{
    return Quat(
        a * s, b * s, c * s, d * s
    );
}

Matrix Quat::ToMat()
{
    Matrix result(1.0f);

    float aF = a * a, bF = b * b, cF = c * c, dF = d * d;
    float ab = a * b, ac = a * c, ad = a * d, bc = b * c, bd = b * d, cd = c * d;

    result._11 = 1.0f - 2.0f * (cF + dF);
    result._12 = 2.0f * bc + 2.0f * ad;
    result._13 = 2.0f * bd - 2.0f * ac;

    result._21 = 2.0f * bc - 2.0f * ad;
    result._22 = 1.0f - 2.0f * (bF + dF);
    result._23 = 2.0f * cd + 2.0f * ab;

    result._31 = 2.0f * bd + 2.0f * ac;
    result._32 = 2.0f * cd - 2.0f * ab;
    result._33 = 1.0f - 2.0f * (bF + cF);

    return result;
}

Quat Quat::operator - ()
{
    return Quat(-a, -b, -c, -d);
}

Quat::Quat(float angle, Vector axis)
{
    float halfAngle = angle * 0.5f;
    float sinAngle = std::sin(halfAngle);
    float cosAngle = std::cos(halfAngle);
    Vector vec = axis.Normalize() * sinAngle;

    this->a = cosAngle, this->b = vec.x, this->c = vec.y, this->d = vec.z;
}

Quat Quat::Normalize()
{
    float n = a * a + b * b + c * c + d * d;

    if (n < 1e-6f)
    {
        return Quat();
    }

    float fact = 1.0f / std::sqrt(n);

    return Quat(a * fact, b * fact, c * fact, d * fact);
}

Vector VecRotate(Quat q, Vector v)
{
    Vector result;

    float aF = q.a * q.a, bF = q.b * q.b, cF = q.c * q.c, dF = q.d * q.d;
    float ab = q.a * q.b, ac = q.a * q.c, ad = q.a * q.d, bc = q.b * q.c, bd = q.b * q.d, cd = q.c * q.d;

    result.x = (1.0f - 2.0f * (cF + dF)) * v.x +
        (2.0f * bc - 2.0f * ad) * v.y +
        (2.0f * bd + 2.0f * ac) * v.z;

    result.y = (2.0f * bc + 2.0f * ad) * v.x +
        (1.0f - 2.0f * (bF + dF)) * v.y +
        (2.0f * cd - 2.0f * ab) * v.z;

    result.z = (2.0f * bd - 2.0f * ac) * v.x +
        (2.0f * cd + 2.0f * ab) * v.y +
        (1.0f - 2.0f * (bF + cF)) * v.z;

    return result;
}

Quat NLerp(Quat q1, Quat q2, float t)
{
    Quat result;

    result = (1.0f - t) * q1 + t * q2;

    return result.Normalize();
}

Quat Slerp(Quat q1, Quat q2, float t)
{
    float cosT = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
    if (cosT < 0.0f)
    {
        q2 = -q2;
        cosT = -cosT;
    }

    if (cosT > 0.9995f)
    {
        return NLerp(q1, q2, t);
    }

    float T = std::acos(cosT);
    float sinT = std::sin(T);
    float sinQ1T = std::sin((1.0f - t) * T);
    float sinQ2T = std::sin(t * T);

    return (sinQ1T / sinT) * q1 + (sinQ2T / sinT) * q2;
}