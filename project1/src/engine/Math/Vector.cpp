#include "Math.h"

Vector Vector::operator + (const Vector& v2) const
{
	Vector result;

	for (size_t i = 0; i < 4; i++)
	{
		result.data[i] = this->data[i] + v2.data[i];
	}

	return result;
}

void Vector::operator += (const Vector& v2)
{
	for (size_t i = 0; i < 4; i++)
	{
		this->data[i] += v2.data[i];
	}
}

Vector Vector::operator - (const Vector& v2) const
{
	Vector result;

	for (size_t i = 0; i < 4; i++)
	{
		result.data[i] = this->data[i] - v2.data[i];
	}

	return result;
}

void Vector::operator -= (const Vector& v2)
{
	for (size_t i = 0; i < 4; i++)
	{
		this->data[i] -= v2.data[i];
	}
}

Vector Vector::operator * (const Vector& v2) const
{
	Vector result;

	for (size_t i = 0; i < 4; i++)
	{
		result.data[i] = this->data[i] * v2.data[i];
	}

	return result;
}

void Vector::operator *= (const Vector& v2)
{
	for (size_t i = 0; i < 4; i++)
	{
		this->data[i] *= v2.data[i];
	}
}

Vector Vector::operator * (const float& s) const
{
	Vector result;

	for (size_t i = 0; i < 4; i++)
	{
		result.data[i] = this->data[i] * s;
	}

	return result;
}

void Vector::operator *= (const float& s)
{
	for (size_t i = 0; i < 4; i++)
	{
		this->data[i] *= s;
	}
}

float Dot(Vector v1, Vector v2)
{
	float result = 0;
	for (size_t i = 0; i < 4; i++)
	{
		result += v1.data[i] * v2.data[i];
	}
	return result;
}

Vector Cross(Vector v1, Vector v2)
{
	Vector result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector Lerp(Vector v1, Vector v2, float a)
{
	Vector result;

	result = v1 * (1.0f - a) + v2 * a;

	return result;
}

float Vector::Length()
{
	return std::sqrt(Dot(*this, *this));
}

Vector Vector::Normalize()
{
	float len = this->Length();

	if (len < Math::epsilon)
	{
		return Vector();
	}

	float Rlength = 1.0f / len;
	return *this * Rlength;
}