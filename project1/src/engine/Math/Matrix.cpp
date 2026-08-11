
#include "Math.h"

Matrix Matrix::operator * (const Matrix& m)
{
    Matrix result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] =
                this->data[i][0] * m.data[0][j] +
                this->data[i][1] * m.data[1][j] +
                this->data[i][2] * m.data[2][j] +
                this->data[i][3] * m.data[3][j];
        }
    }

    return result;
}

void Matrix::operator *= (const Matrix& m)
{
    *this = *this * m;
}

Matrix Matrix::operator * (const float& scale)
{
    Matrix result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = this->data[i][j] * scale;
        }
    }

    return result;
}

void Matrix::operator *= (const float& scale)
{
    *this = *this * scale;
}

Matrix Matrix::Transpose()
{
    Matrix result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = this->data[j][i];
        }
    }

    return result;
}

float Det2x2(float _11, float _12,
    float _21, float _22)
{
    return _11 * _22 - _12 * _21;
}

float Det3x3(float _11, float _12, float _13,
    float _21, float _22, float _23,
    float _31, float _32, float _33)
{
    return _11 * Det2x2(_22, _23, _32, _33) -
        _12 * Det2x2(_21, _23, _31, _33) +
        _13 * Det2x2(_21, _22, _31, _32);
}

float Det(Matrix m)
{
    return m._11 * Det3x3(m._22, m._23, m._24, m._32, m._33, m._34, m._42, m._43, m._44) -
        m._12 * Det3x3(m._21, m._23, m._24, m._31, m._33, m._34, m._41, m._43, m._44) +
        m._13 * Det3x3(m._21, m._22, m._24, m._31, m._32, m._34, m._41, m._42, m._44) -
        m._14 * Det3x3(m._21, m._22, m._23, m._31, m._32, m._33, m._41, m._42, m._43);
}

Matrix Matrix::Inverse()
{
    Matrix result;

    float det = Det(*this);
    if (std::fabs(det) < Math::epsilon)
    {
        return Matrix(1.0f);
    }

    float invDet = 1.0f / det;

    result._11 = Det3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44) * invDet;
    result._12 = -Det3x3(_12, _13, _14, _32, _33, _34, _42, _43, _44) * invDet;
    result._13 = Det3x3(_12, _13, _14, _22, _23, _24, _42, _43, _44) * invDet;
    result._14 = -Det3x3(_12, _13, _14, _22, _23, _24, _32, _33, _34) * invDet;

    result._21 = -Det3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44) * invDet;
    result._22 = Det3x3(_11, _13, _14, _31, _33, _34, _41, _43, _44) * invDet;
    result._23 = -Det3x3(_11, _13, _14, _21, _23, _24, _41, _43, _44) * invDet;
    result._24 = Det3x3(_11, _13, _14, _21, _23, _24, _31, _33, _34) * invDet;

    result._31 = Det3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44) * invDet;
    result._32 = -Det3x3(_11, _12, _14, _31, _32, _34, _41, _42, _44) * invDet;
    result._33 = Det3x3(_11, _12, _14, _21, _22, _24, _41, _42, _44) * invDet;
    result._34 = -Det3x3(_11, _12, _14, _21, _22, _24, _31, _32, _34) * invDet;

    result._41 = -Det3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43) * invDet;
    result._42 = Det3x3(_11, _12, _13, _31, _32, _33, _41, _42, _43) * invDet;
    result._43 = -Det3x3(_11, _12, _13, _21, _22, _23, _41, _42, _43) * invDet;
    result._44 = Det3x3(_11, _12, _13, _21, _22, _23, _31, _32, _33) * invDet;

    return result;
}

Vector Matrix::operator * (const Vector& v2) const
{
    Vector result;

    for (size_t i = 0; i < 4; i++)
    {
        result.data[i] = this->data[i][0] * v2.x +
            this->data[i][1] * v2.y +
            this->data[i][2] * v2.z +
            this->data[i][3] * v2.w;
    }

    return result;
}

Vector operator * (const Vector& vec, const Matrix& m)
{
    Vector result;

    for (size_t i = 0; i < 4; i++)
    {
        result.data[i] = m.data[0][i] * vec.x +
            m.data[1][i] * vec.y +
            m.data[2][i] * vec.z +
            m.data[3][i] * vec.w;
    }

    return result;
}