#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector
{
    union
    {
        float data[4];
        struct
        {
            float x, y, z, w;
        };

        struct
        {
            float r, g, b, a;
        };

        struct
        {
            float s, t, p, q;
        };

    };

    Vector() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
    Vector(float scale) : x(scale), y(scale), z(scale), w(scale) {};
    Vector(float x, float y) : x(x), y(y), z(0.0f), w(0.0f) {};
    Vector(float x, float y, float z) : x(x), y(y), z(z), w(0.0f) {};
    Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

    Vector operator + (const Vector& v2) const;
    void operator += (const Vector& v2);

    Vector operator - (const Vector& v2) const;
    void operator -= (const Vector& v2);

    Vector operator * (const Vector& v2) const;
    void operator *= (const Vector& v2);

    Vector operator * (const float& s) const;
    void operator *= (const float& s);

    float Length();
    Vector Normalize();

} Vector;

inline Vector operator * (float s, const Vector& v)
{
    return const_cast<Vector&>(v) * s;
}

float Dot(Vector v1, Vector v2);
Vector Cross(Vector v1, Vector v2);
Vector Lerp(Vector v1, Vector v2, float a);

#endif