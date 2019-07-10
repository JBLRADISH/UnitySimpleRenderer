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

const Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::back = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);

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
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, this;
		mov edi, v;
		movaps xmm0, [esi];
		addps xmm0, [edi];
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(x + v.x, y + v.y, z + v.z, w + v.w);
#endif
}

//向量减法
inline Vector3 Vector3::operator-(const Vector3& v)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, this;
		mov edi, v;
		movaps xmm0, [esi];
		subps xmm0, [edi];
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(x - v.x, y - v.y, z - v.z, w - v.w);
#endif
}

//向量乘法
inline Vector3 Vector3::operator*(float f)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res(f, f, f, f);
	_asm
	{
		mov esi, this;
		movaps xmm0, [esi];
		mulps xmm0, res.m128;
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(x * f, y * f, z * f, w * f);
#endif
}

inline Vector3 operator*(float f, const Vector3& v)
{
	return Vector3(v.x * f, v.y * f, v.z * f, v.w * f);
}

//向量除法
inline Vector3 Vector3::operator/(float f)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res(f, f, f, f);
	_asm
	{
		mov esi, this;
		movaps xmm0, [esi];
		divps xmm0, res.m128;
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(x / f, y / f, z / f, w / f);
#endif
}

//各分量相乘
inline Vector3& Vector3::Scale(const Vector3& v)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, this;
		mov edi, v;
		movaps xmm0, [esi];
		mulps xmm0, [edi];
		movaps res, xmm0;
	}
	*this = res;
	return *this;
#else
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
#endif
}

inline Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, v1;
		mov edi, v2;
		movaps xmm0, [esi];
		mulps xmm0, [edi];
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
#endif
}

//向量长度
inline float Vector3::Magnitude()
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, this;
		movaps xmm0, [esi];
		mulps xmm0, xmm0;
		movaps xmm1, xmm0;
		shufps xmm1, xmm0, SIMD_SHUFFLE(0x01, 0x00, 0x03, 0x02);
		addps xmm1, xmm0;
		shufps xmm0, xmm1, SIMD_SHUFFLE(0x02, 0x03, 0x00, 0x01);
		addps xmm0, xmm1;
		movaps res, xmm0;
		//sqrtss 只计算低32位的平方根 sqrtps会计算4个32位的平方根
		sqrtss xmm0, res.z;
		movaps res, xmm0;
	}
	return res.x;
#else
	return sqrtf(Vector3::Dot(*this, *this));
#endif
}

inline float Vector3::Magnitude(const Vector3& v)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, v;
		movaps xmm0, [esi];
		mulps xmm0, xmm0;
		movaps xmm1, xmm0;
		shufps xmm1, xmm0, SIMD_SHUFFLE(0x01, 0x00, 0x03, 0x02);
		addps xmm1, xmm0;
		shufps xmm0, xmm1, SIMD_SHUFFLE(0x02, 0x03, 0x00, 0x01);
		addps xmm0, xmm1;
		movaps res, xmm0;
		sqrtss xmm0, res.z;
		movaps res, xmm0;
	}
	return res.x;
#else
	return sqrtf(Vector3::Dot(v, v));
#endif
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
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res(t, t, t, t);
	_asm
	{
		mov esi, v1;
		mov edi, v2;
		movaps xmm0, [edi];
		movaps xmm1, [esi];
		subps xmm0, xmm1;
		mulps xmm0, res.m128;
		addps xmm0, xmm1;
		movaps res, xmm0;
	}
	return res;
#else
	return Vector3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t, v1.w + (v2.w - v1.w) * t);
#endif
}

//点积
inline float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Vector3 res;
	_asm
	{
		mov esi, v1;
		mov edi, v2;
		movaps xmm0, [esi];
		mulps xmm0, [edi];
		movaps xmm1, xmm0;
		shufps xmm1, xmm0, SIMD_SHUFFLE(0x01, 0x00, 0x03, 0x02);
		addps xmm1, xmm0;
		shufps xmm0, xmm1, SIMD_SHUFFLE(0x02, 0x03, 0x00, 0x01);
		addps xmm0, xmm1;
		movaps res, xmm0;
	}
	return res.z;
#else
	return (float)((double)v1.x * (double)v2.x + (double)v1.y * (double)v2.y + (double)v1.z * (double)v2.z + (double)v1.w * (double)v2.w);
#endif
}

//叉积
inline Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3((float)((double)v1.y * (double)v2.z - (double)v1.z * (double)v2.y), (float)((double)v1.z * (double)v2.x - (double)v1.x * (double)v2.z), (float)((double)v1.x * (double)v2.y - (double)v1.y * (double)v2.x));
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

//反射
inline Vector3 Vector3::Reflect(Vector3& inDir, Vector3& normal) 
{
	return -2.0f * normal * Vector3::Dot(inDir, normal) + inDir;
}

//向量间夹角
inline float Vector3::Angle(Vector3& v1, Vector3& v2)
{
	float num = sqrtf(v1.SqrMagnitude() * v2.SqrMagnitude());
	if ((double)num < 1.00000000362749E-15)
		return 0.0f;
	return acosf(Clamp(Vector3::Dot(v1, v2) / num, -1.0f, 1.0f)) * 57.29578f;
}

//在平面上投影
inline Vector3 Vector3::ProjectOnPlane(Vector3& v, Vector3& normal)
{
	return v - Vector3::Project(v, normal);
}


std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return os;
}