#include "vector3.h"

const Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::back = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);

//重载下标运算符
float& Vector3::operator[](int idx)
{
	return v[idx];
}

//重载比较运算符
bool Vector3::operator==(const Vector3& v)
{
	return (double)Vector3::SqrMagnitude(*this - v) < 9.99999943962493E-11;
}

//重载比较运算符
bool Vector3::operator!=(const Vector3& v)
{
	return !(*this == v);
}

//向量加法
Vector3 Vector3::operator+(const Vector3& v)
{
	return Vector3(x + v.x, y + v.y, z + v.z, w + v.w);
}

//向量减法
Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z, w - v.w);
}

//向量乘法
Vector3 Vector3::operator*(float f) const
{
	return Vector3(x * f, y * f, z * f, w * f);
}

Vector3 operator*(float f, const Vector3& v)
{
	return Vector3(v.x * f, v.y * f, v.z * f, v.w * f);
}

//向量除法
Vector3 Vector3::operator/(float f)
{
	return Vector3(x / f, y / f, z / f, w / f);
}

//各分量相乘
Vector3& Vector3::Scale(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

//向量长度
float Vector3::Magnitude()
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
		//sqrtss 只计算低32位的平方根 sqrtps会计算4个32位的平方根
		sqrtss xmm0, xmm0;
		movaps res, xmm0;
	}
	return res.x;
#else
	return sqrtf(Vector3::Dot(*this, *this));
#endif
}

float Vector3::Magnitude(const Vector3& v)
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
		sqrtss xmm0, xmm0;
		movaps res, xmm0;
	}
	return res.x;
#else
	return sqrtf(Vector3::Dot(v, v));
#endif
}

//向量长度平方
float Vector3::SqrMagnitude()
{
	return Vector3::Dot(*this, *this);
}

float Vector3::SqrMagnitude(const Vector3& v)
{
	return Vector3::Dot(v, v);
}

//向量正则化
void Vector3::Normalize()
{
	float num = Vector3::Magnitude(*this);
	if ((double)num > 9.99999974737875E-06)
		* this = *this / num;
	else
		*this = Vector3::zero;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	float num = Vector3::Magnitude(v);
	if ((double)num > 9.99999974737875E-06)
		return Vector3(v) / num;
	else
		return Vector3::zero;
}

Vector3 Vector3::normalized()
{
	return Vector3::Normalize(*this);
}

//Lerp插值
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return Vector3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t, v1.w + (v2.w - v1.w) * t);
}

//点积
float Vector3::Dot(const Vector3& v1, const Vector3& v2)
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
	return res.x;
#else
	return (float)((double)v1.x * (double)v2.x + (double)v1.y * (double)v2.y + (double)v1.z * (double)v2.z);
#endif
}

//叉积
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3((float)((double)v1.y * (double)v2.z - (double)v1.z * (double)v2.y), (float)((double)v1.z * (double)v2.x - (double)v1.x * (double)v2.z), (float)((double)v1.x * (double)v2.y - (double)v1.y * (double)v2.x));
}

//向量距离
float Vector3::Distance(Vector3& v1, Vector3& v2)
{
	return Vector3::Magnitude(v1 - v2);
}

//投影
Vector3 Vector3::Project(Vector3& v1, Vector3& v2)
{
	return v2 * Vector3::Dot(v1, v2) / Vector3::Dot(v2, v2);
}

//反射
Vector3 Vector3::Reflect(Vector3& inDir, Vector3& normal)
{
	return -2.0f * normal * Vector3::Dot(inDir, normal) + inDir;
}

//向量间夹角
float Vector3::Angle(Vector3& v1, Vector3& v2)
{
	float num = sqrtf(v1.SqrMagnitude() * v2.SqrMagnitude());
	if ((double)num < 1.00000000362749E-15)
		return 0.0f;
	return acosf(Clamp(Vector3::Dot(v1, v2) / num, -1.0f, 1.0f)) * 57.29578f;
}

//在平面上投影
Vector3 Vector3::ProjectOnPlane(Vector3& v, Vector3& normal)
{
	return v - Vector3::Project(v, normal);
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return os;
}