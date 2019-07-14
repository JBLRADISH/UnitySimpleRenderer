#pragma once

#include "vector3.h"

class Quaternion
{
public:
	union
	{
		float q[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		__m128 m128;
	};

	static const Quaternion identity;

	Quaternion() = default;

	Quaternion(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	//拷贝构造
	Quaternion(const Quaternion& q)
	{
		memcpy(this, (void*)& q, sizeof(Quaternion));
	}

	//赋值运算符重载
	Quaternion& operator=(const Quaternion& q)
	{
		memcpy(this, (void*)& q, sizeof(Quaternion));
		return *this;
	}

	float& operator[](int idx);
	Quaternion operator+(const Quaternion& q);
	Quaternion operator*(float f);
	Quaternion operator*(const Quaternion& q);
	Vector3 operator*(const Vector3& q);

	static float Dot(const Quaternion& q1, const Quaternion& q2);
	static float Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion Slerp(Quaternion& q1, Quaternion& q2, float t);
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

	void Normalize();
	static Quaternion Normalize(Quaternion& q);
	Quaternion normalized();

	static Quaternion AngleAxis(float angle, Vector3& axis);
	void ToAngleAxis(float& angle, Vector3& axis);
	static Quaternion Euler(const Vector3& euler);
	Vector3 Euler();

	float Norm();
	Quaternion Conjugate();
	Quaternion UnitInverse();
	Quaternion Inverse();

	friend std::ostream& operator<< (std::ostream&, const Quaternion&);
};