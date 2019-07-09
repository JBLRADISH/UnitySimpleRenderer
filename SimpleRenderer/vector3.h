#pragma once

#include <math.h>
#include <string>
#include <iostream>

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
	};

	static Vector3 zero;
	static Vector3 one;
	static Vector3 forward;
	static Vector3 back;
	static Vector3 right;
	static Vector3 left;
	static Vector3 up;
	static Vector3 down;

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
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	//赋值运算符重载
	Vector3& operator=(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	float& operator[](int idx);
	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);
	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v);
	Vector3 operator*(float f);
	Vector3 operator/(float f);
	static Vector3 operator*(float f, const Vector3& v);

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

	//friend ostream& operator<< (ostream&, const Vector3&);
};

//零向量
Vector3 Vector3::zero = Vector3(0, 0, 0, 0);

//这样的不是常量是不是可以在外部修改？
Vector3 Vector3::one = Vector3(1, 1, 1);
Vector3 Vector3::forward = Vector3(0, 0, 1);
Vector3 Vector3::back = Vector3(0, 0, -1);
Vector3 Vector3::right = Vector3(1, 0, 0);
Vector3 Vector3::left = Vector3(-1, 0, 0);
Vector3 Vector3::up = Vector3(0, 1, 0);
Vector3 Vector3::down = Vector3(0, -1, 0);

//重载下标运算符
inline float& Vector3::operator[](int idx)
{
	return v[idx];
}

//重载比较运算符
inline bool Vector3::operator==(const Vector3& v)
{
	return (double)Vector3::SqrMagnitude(*this - v) < 9.99999943962493E-11;
}

//重载比较运算符
inline bool Vector3::operator!=(const Vector3& v)
{
	return !(*this == v);
}

//向量加法
inline Vector3 Vector3::operator+(const Vector3& v)
{
	return Vector3(x + v.x, y + v.y, z + v.z, w + v.w);
}

//向量减法
inline Vector3 Vector3::operator-(const Vector3& v)
{
	return Vector3(x - v.x, y - v.y, z - v.z, w - v.w);
}

//向量乘法
inline Vector3 Vector3::operator*(float f)
{
	return Vector3(x * f, y * f, z * f, w * f);
}

inline Vector3 Vector3::operator*(float f, const Vector3& v) 
{
	return Vector3(v.x * f, v.y * f, v.z * f, v.w * f);
}

//向量除法
inline Vector3 Vector3::operator/(float f)
{
	return Vector3(x / f, y / f, z / f, w / f);
}

//各分量相乘
inline Vector3& Vector3::Scale(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

inline Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

//向量长度
inline float Vector3::Magnitude()
{
	return sqrtf(Vector3::Dot(*this, *this));
}

inline float Vector3::Magnitude(const Vector3& v)
{
	return sqrtf(Vector3::Dot(v, v));
}

//向量长度平方
inline float Vector3::SqrMagnitude()
{
	return Vector3::Dot(*this, *this);
}

inline float Vector3::SqrMagnitude(const Vector3& v)
{
	return Vector3::Dot(v, v);
}

//向量正则化
inline void Vector3::Normalize()
{
	float num = Vector3::Magnitude(*this);
	if ((double)num > 9.99999974737875E-06)
		*this = *this / num;
	else
		*this = Vector3::zero;
}

inline Vector3 Vector3::Normalize(const Vector3& v)
{
	float num = Vector3::Magnitude(v);
	if ((double)num > 9.99999974737875E-06)
		return Vector3(v) / num;
	else
		return Vector3::zero;
}

inline Vector3 Vector3::normalized()
{
	return Vector3::Normalize(*this);
}

//Lerp插值
inline Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return Vector3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t, v1.w + (v2.w - v1.w) * t);
}

//点积
inline float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return (float)((double)v1.x * (double)v2.x + (double)v1.y * (double)v2.y + (double)v1.z * (double)v2.z + (double)v1.w * (double)v2.w);
}

//叉积
inline Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.x * v2.z - v1.z * v2.x, v1.x * v2.y - v1.y * v2.x);
}

//向量距离
inline float Vector3::Distance(Vector3& v1, Vector3& v2)
{
	return Vector3::Magnitude(v1 - v2);
}

//投影
inline Vector3 Vector3::Project(Vector3& v1, Vector3& v2)
{
	return v2 * Vector3::Dot(v1, v2) / Vector3::Dot(v2, v2);
}

//反射 为什么有的参数是常量，有的不是啊
inline Vector3 Vector3::Reflect(Vector3& inDir, Vector3& normal) {
	return normal * Vector3::Dot(inDir, normal) * -2 + inDir;
}

//向量间夹角
inline float Vector3::Angle(Vector3& v1, Vector3& v2) {
	//反余弦 不会写
}

//在平面上投影
inline Vector3 Vector3::ProjectOnPlane(Vector3& v, Vector3& normal)
{
	return v - Vector3::Project(v, normal);
}
