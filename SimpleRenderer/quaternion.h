#pragma once

class Quaternion
{
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

	static float Dot(const Quaternion& q1, const Quaternion& q2);

	void Normalize();
	static Quaternion Normalize(const Quaternion& q);
	Quaternion normalized();
};

const Quaternion Quaternion::identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//重载下标运算符
inline float& Quaternion::operator[](int idx)
{
	return q[idx];
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

//四元数正则化
inline void Quaternion::Normalize()
{
	*this = Quaternion::Normalize(*this);
}

inline Quaternion Quaternion::Normalize(const Quaternion& q)
{
	float num = sqrtf(Quaternion::Dot(q, q));
	if ((double)num < (double)Epsilon)
		return Quaternion::identity;
	return Quaternion(q.x / num, q.y / num, q.z / num, q.w / num);
}

inline Quaternion Quaternion::normalized()
{
	return Quaternion::Normalize(*this);
}