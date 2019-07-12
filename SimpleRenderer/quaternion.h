#pragma once

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
	static Quaternion Euler(Vector3& euler);
	Vector3 Euler();

	float Norm();
	Quaternion Conjugate();
	Quaternion UnitInverse();
	Quaternion Inverse();
};

const Quaternion Quaternion::identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//重载下标运算符
inline float& Quaternion::operator[](int idx)
{
	return q[idx];
}

//重载加法
Quaternion Quaternion::operator+(const Quaternion& q)
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

//重载乘法
Quaternion Quaternion::operator*(float f)
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

Quaternion Quaternion::operator*(const Quaternion& q)
{
	return Quaternion((float)((double)w * (double)q.x + (double)x * (double)q.w + (double)y * (double)q.z - (double)z * (double)q.y), (float)((double)w * (double)q.y + (double)y * (double)q.w + (double)z * (double)q.x - (double)x * (double)q.z), (float)((double)w * (double)q.z + (double)z * (double)q.w + (double)x * (double)q.y - (double)y * (double)q.x), (float)((double)w * (double)q.w - (double)x * (double)q.x - (double)y * (double)q.y - (double)z * (double)q.z));
}

//对向量进行旋转 q * v * q^-1
Vector3 Quaternion::operator*(const Vector3& v)
{
	float num1 = x * 2.0f;
	float num2 = y * 2.0f;
	float num3 = z * 2.0f;
	float num4 = x * num1;
	float num5 = y * num2;
	float num6 = z * num3;
	float num7 = x * num2;
	float num8 = x * num3;
	float num9 = y * num3;
	float num10 = w * num1;
	float num11 = w * num2;
	float num12 = w * num3;
	Vector3 res;
	res.x = (float)((1.0 - ((double)num5 + (double)num6)) * (double)v.x + ((double)num7 - (double)num12) * (double)v.y + ((double)num8 + (double)num11) * (double)v.z);
	res.y = (float)(((double)num7 + (double)num12) * (double)v.x + (1.0 - ((double)num4 + (double)num6)) * (double)v.y + ((double)num9 - (double)num10) * (double)v.z);
	res.z = (float)(((double)num8 - (double)num11) * (double)v.x + ((double)num9 + (double)num10) * (double)v.y + (1.0 - ((double)num4 + (double)num5)) * (double)v.z);
	return res;
}

//点积
inline float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
#ifdef SIMD_ASM
	__declspec(align(16)) Quaternion res;
	_asm
	{
		mov esi, q1;
		mov edi, q2;
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
	return (float)((double)q1.x * (double)q2.x + (double)q1.y * (double)q2.y + (double)q1.z * (double)q2.z + (double)q1.w * (double)q2.w);
#endif
}

//角度
inline float Quaternion::Angle(const Quaternion& q1, const Quaternion& q2)
{
	float num = Quaternion::Dot(q1, q2);
	return (double)num <= 0.999998986721039 ? (float)((double)acosf(fminf(fabsf(num), 1.0f)) * 2.0 * 57.2957801818848) : 0.0f;
}

//单位四元数球面插值
inline Quaternion Quaternion::Slerp(Quaternion& q1, Quaternion& q2, float t)
{
	float costheta = Quaternion::Dot(q1, q2);
	Quaternion q = q1;
	if (costheta < 0.0f)
	{
		q = q * (-1.0f);
		costheta = -costheta;
	}
	float t0, t1;
	if (costheta > 0.9999f)
	{
		t0 = 1.0f - t;
		t1 = t;
	}
	else 
	{
		float sintheta = sqrtf(1.0f - costheta * costheta);
		float theta = atan2(sintheta, costheta);
		float invsintheta = 1.0f / sintheta;
		t0 = sinf((1.0f - t) * theta) * invsintheta;
		t1 = sinf(t * theta) * invsintheta;
	}

	return q * t0 + q2 * t1;
}

//创建给定一个特定前向量的四元数 利用矩阵转换到四元数
inline Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 vector = Vector3::Normalize(forward);
	Vector3 vector2 = Vector3::Normalize(Vector3::Cross(up, vector));
	Vector3 vector3 = Vector3::Cross(vector, vector2);
	float m00 = vector2.x;
	float m01 = vector2.y;
	float m02 = vector2.z;
	float m10 = vector3.x;
	float m11 = vector3.y;
	float m12 = vector3.z;
	float m20 = vector.x;
	float m21 = vector.y;
	float m22 = vector.z;
	float num8 = (m00 + m11) + m22;
	Quaternion res;
	if (num8 > 0.0f)
	{
		float num = sqrtf(num8 + 1.0f);
		res.w = num * 0.5f;
		num = 0.5f / num;
		res.x = (m12 - m21) * num;
		res.y = (m20 - m02) * num;
		res.z = (m01 - m10) * num;
		return res;
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = sqrtf(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		res.x = 0.5f * num7;
		res.y = (m01 + m10) * num4;
		res.z = (m02 + m20) * num4;
		res.w = (m12 - m21) * num4;
		return res;
	}
	if (m11 > m22)
	{
		float num6 = sqrtf(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		res.x = (m10 + m01) * num3;
		res.y = 0.5f * num6;
		res.z = (m21 + m12) * num3;
		res.w = (m20 - m02) * num3;
		return res;
	}
	float num5 = sqrtf(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	res.x = (m20 + m02) * num2;
	res.y = (m21 + m12) * num2;
	res.z = 0.5f * num5;
	res.w = (m01 - m10) * num2;
	return res;
}

//四元数正则化
inline void Quaternion::Normalize()
{
	*this = Quaternion::Normalize(*this);
}

inline Quaternion Quaternion::Normalize(Quaternion& q)
{
	float num = sqrtf(Quaternion::Dot(q, q));
	if ((double)num < (double)Epsilon)
		return Quaternion::identity;
	return q * (1.0f / num);
}

inline Quaternion Quaternion::normalized()
{
	return Quaternion::Normalize(*this);
}

//根据旋转轴和旋转角度创建一个四元数
inline Quaternion Quaternion::AngleAxis(float angle, Vector3& axis)
{
	Vector3 newAxis = axis.normalized();
	float halftheta = 0.5f * angle * Deg2Rad;
	float sinhalftheta = sinf(halftheta);
	return Quaternion(sinhalftheta * newAxis.x, sinhalftheta * newAxis.y, sinhalftheta * newAxis.z, cosf(halftheta));
}

//将四元数转换为旋转轴和旋转角度
inline void Quaternion::ToAngleAxis(float& angle, Vector3& axis)
{
	angle = acosf(w);
	float invsinangle = 1.0f / sinf(angle);
	axis.x = x * invsinangle;
	axis.y = y * invsinangle;
	axis.z = z * invsinangle;
	angle *= 2.0f * Rad2Deg;
}

//根据欧拉角创建一个四元数 yxz
inline Quaternion Quaternion::Euler(Vector3& euler)
{
	float halfx = 0.5f * euler.x * Deg2Rad;
	float halfy = 0.5f * euler.y * Deg2Rad;
	float halfz = 0.5f * euler.z * Deg2Rad;
	float sinhalfx = sinf(halfx);
	float coshalfx = cosf(halfx);
	float sinhalfy = sinf(halfy);
	float coshalfy = cosf(halfy);
	float sinhalfz = sinf(halfz);
	float coshalfz = cosf(halfz);
	float coshalfxcoshalfy = coshalfx * coshalfy;
	float sinhalfxsinhalfy = sinhalfx * sinhalfy;
	float coshalfxsinhalfy = coshalfx * sinhalfy;
	float sinhalfxcoshalfy = sinhalfx * coshalfy;
	Quaternion q;
	q.x = coshalfxsinhalfy * sinhalfz + sinhalfxcoshalfy * coshalfz;
	q.y = coshalfxsinhalfy * coshalfz - sinhalfxcoshalfy * sinhalfz;
	q.z = coshalfxcoshalfy * sinhalfz - sinhalfxsinhalfy * coshalfz;
	q.w = coshalfxcoshalfy * coshalfz + sinhalfxsinhalfy * sinhalfz;
	return q;
}

//四元数转换为欧拉角
inline Vector3 Quaternion::Euler()
{
	Vector3 res;
	float xsqr = x * x;
	res.x = asinf(2.0f * (w * x - y * z)) * Rad2Deg;
	//atan2可以返回-π到π atan只能返回-π/2到π/2
	res.y = atan2f(2.0f * (w * y + x * z), 1.0f - 2.0f * (xsqr + y * y)) * Rad2Deg;
	res.z = atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (xsqr + z * z)) * Rad2Deg;
	return res;
}

//四元数的模
inline float Quaternion::Norm()
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

//四元数共轭
inline Quaternion Quaternion::Conjugate()
{
	return Quaternion(-x, -y, -z, w);
}

//单位四元数的逆等于共轭
inline Quaternion Quaternion::UnitInverse()
{
	return Conjugate();
}

//四元数的逆
inline Quaternion Quaternion::Inverse()
{
	float invNorm = 1.0f / Norm();
	return *this * invNorm;
}