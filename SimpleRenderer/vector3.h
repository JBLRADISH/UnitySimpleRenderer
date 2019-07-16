#pragma once

#include <math.h>
#include <xmmintrin.h>
#include <ostream>
#include "mathf.h"

#define SIMD_ASM 1
#define SIMD_SHUFFLE(srch, srcl, desth, destl) (srch << 6) | (srcl << 4) | (desth << 2) | (destl)

class Vector3
{
public:
	union
	{
		float v[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		__m128 m128;
	};

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 up;
	static const Vector3 down;

	Vector3() = default;

	Vector3(float x, float y, float z, float w = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	//拷贝构造
	Vector3(const Vector3& v)
	{
		memcpy(this, (void*)& v, sizeof(Vector3));
	}

	//赋值运算符重载
	Vector3& operator=(const Vector3& v)
	{
		memcpy(this, (void*)& v, sizeof(Vector3));
		return *this;
	}

	float& operator[](int idx);
	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);
	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(float f) const;
	friend Vector3 operator*(float f, const Vector3& v);
	Vector3 operator/(float f);

	Vector3& Scale(const Vector3& v);
	static Vector3 Scale(const Vector3& v1, const Vector3& v2);

	float Magnitude();
	static float Magnitude(const Vector3& v);
	float SqrMagnitude();
	static float SqrMagnitude(const Vector3& v);

	void Normalize();
	static Vector3 Normalize(const Vector3& v);
	Vector3 normalized();

	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	static Vector3 Reflect(Vector3& inDir, Vector3& normal);
	static float Distance(Vector3& v1, Vector3& v2);
	static Vector3 Project(Vector3& v1, Vector3& v2);
	static float Angle(Vector3& v1, Vector3& v2);
	static Vector3 ProjectOnPlane(Vector3& v, Vector3 &normal);

	friend std::ostream& operator<< (std::ostream&, const Vector3&);
};