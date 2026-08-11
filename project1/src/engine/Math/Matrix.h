#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

typedef struct Matrix
{
    union
    {
        float data[4][4];

        struct
        {
            float _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44;
        };

    };

    Matrix() :
        _11(0.0f), _12(0.0f), _13(0.0f), _14(0.0f),
        _21(0.0f), _22(0.0f), _23(0.0f), _24(0.0f),
        _31(0.0f), _32(0.0f), _33(0.0f), _34(0.0f),
        _41(0.0f), _42(0.0f), _43(0.0f), _44(0.0f) {
    };

    Matrix(float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44) :
        _11(_11), _12(_12), _13(_13), _14(_14),
        _21(_21), _22(_22), _23(_23), _24(_24),
        _31(_31), _32(_32), _33(_33), _34(_34),
        _41(_41), _42(_42), _43(_43), _44(_44) {
    };

    Matrix(float scale) :
        _11(scale), _12(0.0f), _13(0.0f), _14(0.0f),
        _21(0.0f), _22(scale), _23(0.0f), _24(0.0f),
        _31(0.0f), _32(0.0f), _33(scale), _34(0.0f),
        _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f) {
    };

    Matrix operator * (const Matrix& m);
    void operator *= (const Matrix& m);

    Matrix operator * (const float& scale);
    void operator *= (const float& scale);

    Matrix Transpose();
    Matrix Inverse();

    Vector operator * (const Vector& v2) const;

} Matrix;

inline Matrix operator * (float s, const Matrix& m)
{
    return const_cast<Matrix&>(m) * s;
}

Vector operator * (const Vector& vec, const Matrix& m);

#endif