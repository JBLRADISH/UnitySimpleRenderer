#pragma once

#include <math.h>

class Vector4
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
	};

	static Vector4 zero;

	Vector4(float x, float y, float z, float w = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	//拷贝构造
	Vector4(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	//赋值运算符重载
	Vector4& operator=(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	float& operator[](int idx);
	bool operator==(const Vector4& v);
	bool operator!=(const Vector4& v);
	Vector4 operator+(const Vector4& v);
	Vector4 operator-(const Vector4& v);
	Vector4 operator*(float f);
	Vector4 operator/(float f);

	Vector4& Scale(const Vector4& v);
	static Vector4 Scale(const Vector4& v1, const Vector4& v2);

	float Magnitude();
	static float Magnitude(const Vector4& v);
	float SqrMagnitude();
	static float SqrMagnitude(const Vector4& v);

	void Normalize();
	static Vector4 Normalize(const Vector4& v);
	Vector4 normalized();

	static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);
	static float Dot(const Vector4& v1, const Vector4& v2);
	static float Distance(Vector4& v1, Vector4& v2);
	static Vector4 Project(Vector4& v1, Vector4& v2);
};

//零向量
Vector4 Vector4::zero = Vector4(0, 0, 0, 0);

//重载下标运算符
inline float& Vector4::operator[](int idx)
{
	return v[idx];
}

//重载比较运算符
inline bool Vector4::operator==(const Vector4& v)
{
	return (double)Vector4::SqrMagnitude(*this - v) < 9.99999943962493E-11;
}

//重载比较运算符
inline bool Vector4::operator!=(const Vector4& v)
{
	return !(*this == v);
}

//向量加法
inline Vector4 Vector4::operator+(const Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

//向量减法
inline Vector4 Vector4::operator-(const Vector4& v)
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

//向量乘法
inline Vector4 Vector4::operator*(float f)
{
	return Vector4(x * f, y * f, z * f, w * f);
}

//向量除法
inline Vector4 Vector4::operator/(float f)
{
	return Vector4(x / f, y / f, z / f, w / f);
}

//各分量相乘
inline Vector4& Vector4::Scale(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

inline Vector4 Vector4::Scale(const Vector4& v1, const Vector4& v2)
{
	return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

//向量长度
inline float Vector4::Magnitude()
{
	return sqrtf(Vector4::Dot(*this, *this));
}

inline float Vector4::Magnitude(const Vector4& v)
{
	return sqrtf(Vector4::Dot(v, v));
}

//向量长度平方
inline float Vector4::SqrMagnitude()
{
	return Vector4::Dot(*this, *this);
}

inline float Vector4::SqrMagnitude(const Vector4& v)
{
	return Vector4::Dot(v, v);
}

//向量正则化
inline void Vector4::Normalize()
{
	float num = Vector4::Magnitude(*this);
	if ((double)num > 9.99999974737875E-06)
		*this = *this / num;
	else
		*this = Vector4::zero;
}

inline Vector4 Vector4::Normalize(const Vector4& v)
{
	float num = Vector4::Magnitude(v);
	if ((double)num > 9.99999974737875E-06)
		return Vector4(v) / num;
	else
		return Vector4::zero;
}

inline Vector4 Vector4::normalized()
{
	return Vector4::Normalize(*this);
}

//Lerp插值
inline Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t)
{
	return Vector4(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t, v1.w + (v2.w - v1.w) * t);
}

//点积
inline float Vector4::Dot(const Vector4& v1, const Vector4& v2)
{
	return (float)((double)v1.x * (double)v2.x + (double)v1.y * (double)v2.y + (double)v1.z * (double)v2.z + (double)v1.w * (double)v2.w);
}

//向量距离
inline float Vector4::Distance(Vector4& v1, Vector4& v2)
{
	return Vector4::Magnitude(v1 - v2);
}

//投影
inline Vector4 Vector4::Project(Vector4& v1, Vector4& v2)
{
	return v2 * Vector4::Dot(v1, v2) / Vector4::Dot(v2, v2);
}