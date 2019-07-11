#pragma once

#include "vector3.h"
#include "quaternion.h"

class Matrix4x4
{
public:
	union
	{
		Vector3 v[4];
		struct
		{
			float m00;
			float m10;
			float m20;
			float m30;
			float m01;
			float m11;
			float m21;
			float m31;
			float m02;
			float m12;
			float m22;
			float m32;
			float m03;
			float m13;
			float m23;
			float m33;
		};
		float m[16];
		__m128 m128[4];
	};

	static const Matrix4x4 zero;
	static const Matrix4x4 identity;

	Matrix4x4() = default;

	Matrix4x4(Vector3 column0, Vector3 column1, Vector3 column2, Vector3 column3)
	{
		v[0] = column0;
		v[1] = column1;
		v[2] = column2;
		v[3] = column3;
	}

	//拷贝构造
	Matrix4x4(const Matrix4x4& m)
	{
		memcpy(this, (void*)& m, sizeof(Matrix4x4));
	}

	//赋值运算符重载
	Matrix4x4& operator=(const Matrix4x4& m)
	{
		memcpy(this, (void*)& m, sizeof(Matrix4x4));
		return *this;
	}

	Vector3& operator[](int idx);
	Vector3 operator*(const Vector3& v);
	Matrix4x4 operator*(float f);
	Matrix4x4 operator*(Matrix4x4& m);

	Vector3 GetRow(int idx);

	Matrix4x4 Transpose();
	static Matrix4x4 Transpose(Matrix4x4& m);

	static Matrix4x4 Translate(const Vector3& v);
	static Matrix4x4 Rotate(const Quaternion& q);
	static Matrix4x4 Rotate(const Vector3& v);
	static Matrix4x4 Scale(const Vector3& v);
	static Matrix4x4 TRS(const Vector3& translation, const Quaternion& rotate, const Vector3& scale);
	static Matrix4x4 TRS(const Vector3& translation, const Vector3& rotate, const Vector3& scale);

	static Vector3 ExtractTranslation(Matrix4x4& m);
	static Quaternion ExtractRotate(Matrix4x4& m);
	static Vector3 ExtractScale(Matrix4x4& m);

	float Determinant();
	bool Inverse(Matrix4x4& m);

	friend std::ostream& operator<< (std::ostream&, Matrix4x4&);
};

const Matrix4x4 Matrix4x4::zero = Matrix4x4(Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f));
const Matrix4x4 Matrix4x4::identity = Matrix4x4(Vector3(1.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 1.0f));

//重载下标运算符 得到矩阵第n列
inline Vector3& Matrix4x4::operator[](int idx)
{
	return v[idx];
}

//重载矩阵向量乘法
inline Vector3 Matrix4x4::operator*(const Vector3& v)
{
	Vector3 res;
#ifdef SIMD_ASM
	__m128 col1 = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 col2 = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 col3 = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 col4 = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(3, 3, 3, 3));

	res.m128 = _mm_add_ps(
		_mm_add_ps(_mm_mul_ps(m128[0], col1), _mm_mul_ps(m128[1], col2)),
		_mm_add_ps(_mm_mul_ps(m128[2], col3), _mm_mul_ps(m128[3], col4))
	);
#else
	res.x = (float)((double)m00 * (double)v.x + (double)m01 * (double)v.y + (double)m02 * (double)v.z + (double)m03 * (double)v.w);
	res.y = (float)((double)m10 * (double)v.x + (double)m11 * (double)v.y + (double)m12 * (double)v.z + (double)m13 * (double)v.w);
	res.z = (float)((double)m20 * (double)v.x + (double)m21 * (double)v.y + (double)m22 * (double)v.z + (double)m23 * (double)v.w);
	res.w = (float)((double)m30 * (double)v.x + (double)m31 * (double)v.y + (double)m32 * (double)v.z + (double)m33 * (double)v.w);
#endif
	return res;
}

inline Matrix4x4 Matrix4x4::operator*(float f)
{
	Matrix4x4 res;
#ifdef SIMD_ASM
	__m128 s = _mm_set1_ps(f);
	res.m128[0] = _mm_mul_ps(m128[0], s);
	res.m128[1] = _mm_mul_ps(m128[1], s);
	res.m128[2] = _mm_mul_ps(m128[2], s);
	res.m128[3] = _mm_mul_ps(m128[3], s);
#else
	for (int i = 0; i < 4; i++)
	{
		res[i] = v[i] * f;
	}
#endif
	return res;
}

//重载矩阵矩阵乘法
inline Matrix4x4 Matrix4x4::operator*(Matrix4x4& m)
{
	Matrix4x4 res;
#ifdef SIMD_ASM
	{
		__m128 e0 = _mm_shuffle_ps(m.m128[0], m.m128[0], _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(m.m128[0], m.m128[0], _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(m.m128[0], m.m128[0], _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(m.m128[0], m.m128[0], _MM_SHUFFLE(3, 3, 3, 3));

		__m128 v0 = _mm_mul_ps(m128[0], e0);
		__m128 v1 = _mm_mul_ps(m128[1], e1);
		__m128 v2 = _mm_mul_ps(m128[2], e2);
		__m128 v3 = _mm_mul_ps(m128[3], e3);

		__m128 a0 = _mm_add_ps(v0, v1);
		__m128 a1 = _mm_add_ps(v2, v3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res.m128[0] = a2;
	}

	{
		__m128 e0 = _mm_shuffle_ps(m.m128[1], m.m128[1], _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(m.m128[1], m.m128[1], _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(m.m128[1], m.m128[1], _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(m.m128[1], m.m128[1], _MM_SHUFFLE(3, 3, 3, 3));

		__m128 v0 = _mm_mul_ps(m128[0], e0);
		__m128 v1 = _mm_mul_ps(m128[1], e1);
		__m128 v2 = _mm_mul_ps(m128[2], e2);
		__m128 v3 = _mm_mul_ps(m128[3], e3);

		__m128 a0 = _mm_add_ps(v0, v1);
		__m128 a1 = _mm_add_ps(v2, v3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res.m128[1] = a2;
	}

	{
		__m128 e0 = _mm_shuffle_ps(m.m128[2], m.m128[2], _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(m.m128[2], m.m128[2], _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(m.m128[2], m.m128[2], _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(m.m128[2], m.m128[2], _MM_SHUFFLE(3, 3, 3, 3));

		__m128 v0 = _mm_mul_ps(m128[0], e0);
		__m128 v1 = _mm_mul_ps(m128[1], e1);
		__m128 v2 = _mm_mul_ps(m128[2], e2);
		__m128 v3 = _mm_mul_ps(m128[3], e3);

		__m128 a0 = _mm_add_ps(v0, v1);
		__m128 a1 = _mm_add_ps(v2, v3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res.m128[2] = a2;
	}

	{
		__m128 e0 = _mm_shuffle_ps(m.m128[3], m.m128[3], _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(m.m128[3], m.m128[3], _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(m.m128[3], m.m128[3], _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(m.m128[3], m.m128[3], _MM_SHUFFLE(3, 3, 3, 3));

		__m128 v0 = _mm_mul_ps(m128[0], e0);
		__m128 v1 = _mm_mul_ps(m128[1], e1);
		__m128 v2 = _mm_mul_ps(m128[2], e2);
		__m128 v3 = _mm_mul_ps(m128[3], e3);

		__m128 a0 = _mm_add_ps(v0, v1);
		__m128 a1 = _mm_add_ps(v2, v3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res.m128[3] = a2;
	}
#else
	res.m00 = (float)((double)m00 * (double)m.m00 + (double)m01 * (double)m.m10 + (double)m02 * (double)m.m20 + (double)m03 * (double)m.m30);
	res.m01 = (float)((double)m00 * (double)m.m01 + (double)m01 * (double)m.m11 + (double)m02 * (double)m.m21 + (double)m03 * (double)m.m31);
	res.m02 = (float)((double)m00 * (double)m.m02 + (double)m01 * (double)m.m12 + (double)m02 * (double)m.m22 + (double)m03 * (double)m.m32);
	res.m03 = (float)((double)m00 * (double)m.m03 + (double)m01 * (double)m.m13 + (double)m02 * (double)m.m23 + (double)m03 * (double)m.m33);
	res.m10 = (float)((double)m10 * (double)m.m00 + (double)m11 * (double)m.m10 + (double)m12 * (double)m.m20 + (double)m13 * (double)m.m30);
	res.m11 = (float)((double)m10 * (double)m.m01 + (double)m11 * (double)m.m11 + (double)m12 * (double)m.m21 + (double)m13 * (double)m.m31);
	res.m12 = (float)((double)m10 * (double)m.m02 + (double)m11 * (double)m.m12 + (double)m12 * (double)m.m22 + (double)m13 * (double)m.m32);
	res.m13 = (float)((double)m10 * (double)m.m03 + (double)m11 * (double)m.m13 + (double)m12 * (double)m.m23 + (double)m13 * (double)m.m33);
	res.m20 = (float)((double)m20 * (double)m.m00 + (double)m21 * (double)m.m10 + (double)m22 * (double)m.m20 + (double)m23 * (double)m.m30);
	res.m21 = (float)((double)m20 * (double)m.m01 + (double)m21 * (double)m.m11 + (double)m22 * (double)m.m21 + (double)m23 * (double)m.m31);
	res.m22 = (float)((double)m20 * (double)m.m02 + (double)m21 * (double)m.m12 + (double)m22 * (double)m.m22 + (double)m23 * (double)m.m32);
	res.m23 = (float)((double)m20 * (double)m.m03 + (double)m21 * (double)m.m13 + (double)m22 * (double)m.m23 + (double)m23 * (double)m.m33);
	res.m30 = (float)((double)m30 * (double)m.m00 + (double)m31 * (double)m.m10 + (double)m32 * (double)m.m20 + (double)m33 * (double)m.m30);
	res.m31 = (float)((double)m30 * (double)m.m01 + (double)m31 * (double)m.m11 + (double)m32 * (double)m.m21 + (double)m33 * (double)m.m31);
	res.m32 = (float)((double)m30 * (double)m.m02 + (double)m31 * (double)m.m12 + (double)m32 * (double)m.m22 + (double)m33 * (double)m.m32);
	res.m33 = (float)((double)m30 * (double)m.m03 + (double)m31 * (double)m.m13 + (double)m32 * (double)m.m23 + (double)m33 * (double)m.m33);
#endif
	return res;
}

//得到矩阵第n行
inline Vector3 Matrix4x4::GetRow(int idx)
{
	return Vector3(v[0][idx], v[1][idx], v[2][idx], v[3][idx]);
}

//矩阵转置
inline Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4::Transpose(*this);
}

inline Matrix4x4 Matrix4x4::Transpose(Matrix4x4& m)
{
	Matrix4x4 res;
#ifdef SIMD_ASM
	__m128 tmp0 = _mm_shuffle_ps(m.m128[0], m.m128[1], 0x44);
	__m128 tmp2 = _mm_shuffle_ps(m.m128[0], m.m128[1], 0xEE);
	__m128 tmp1 = _mm_shuffle_ps(m.m128[2], m.m128[3], 0x44);
	__m128 tmp3 = _mm_shuffle_ps(m.m128[2], m.m128[3], 0xEE);

	res.m128[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
	res.m128[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
	res.m128[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
	res.m128[3] = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
#else

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i][j] = m[j][i];
		}
	}
#endif
	return res;
}

//创建平移矩阵
inline Matrix4x4 Matrix4x4::Translate(const Vector3& v)
{
	Matrix4x4 res = Matrix4x4::identity;
	res[3] = v;
	res[3][3] = 1.0f;
	return res;
}

//TODO 根据四元数创建旋转矩阵
inline Matrix4x4 Matrix4x4::Rotate(const Quaternion& q)
{

}

//根据欧拉角创建旋转矩阵 local2world yxz
inline Matrix4x4 Matrix4x4::Rotate(const Vector3& v)
{
	float sinx = sinf(v.x * Deg2Rad);
	float cosx = cosf(v.x * Deg2Rad);
	float siny = sinf(v.y * Deg2Rad);
	float cosy = cosf(v.y * Deg2Rad);
	float sinz = sinf(v.z * Deg2Rad);
	float cosz = cosf(v.z * Deg2Rad);
	float cosycosz = cosy * cosz;
	float sinxsiny = sinx * siny;
	float cosysinz = cosy * sinz;

	Matrix4x4 res;
	res[0] = Vector3(cosycosz + sinxsiny * sinz, cosx * sinz, -siny * cosz + sinx * cosysinz);
	res[1] = Vector3(-cosysinz + sinxsiny * cosz, cosx * cosz, siny * sinz + sinx * cosycosz);
	res[2] = Vector3(cosx * siny, -sinx, cosx * cosy);
	res[3] = Vector3(0.0f, 0.0f, 0.0f, 1.0f);
	return res;
}

//创建缩放矩阵
inline Matrix4x4 Matrix4x4::Scale(const Vector3& v)
{
	Matrix4x4 res = Matrix4x4::identity;
	res[0][0] = v.x;
	res[1][1] = v.y;
	res[2][2] = v.z;
	return res;
}

//TODO TRS矩阵
inline Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Quaternion& rotate, const Vector3& scale)
{

}

inline Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Vector3& rotate, const Vector3& scale)
{
	float sinx = sinf(rotate.x * Deg2Rad);
	float cosx = cosf(rotate.x * Deg2Rad);
	float siny = sinf(rotate.y * Deg2Rad);
	float cosy = cosf(rotate.y * Deg2Rad);
	float sinz = sinf(rotate.z * Deg2Rad);
	float cosz = cosf(rotate.z * Deg2Rad);
	float cosycosz = cosy * cosz;
	float sinxsiny = sinx * siny;
	float cosysinz = cosy * sinz;

	Matrix4x4 res;
	res[0] = Vector3(scale.x * (cosycosz + sinxsiny * sinz), scale.x * cosx * sinz, scale.x * (-siny * cosz + sinx * cosysinz));
	res[1] = Vector3(scale.y * (-cosysinz + sinxsiny * cosz), scale.y * cosx * cosz, scale.y * (siny * sinz + sinx * cosycosz));
	res[2] = Vector3(scale.z * (cosx * siny), scale.z * -sinx, scale.z * cosx * cosy);
	res[3] = translation;
	res[3][3] = 1.0f;
	return res;
}

//从矩阵中提取平移
inline Vector3 Matrix4x4::ExtractTranslation(Matrix4x4& m)
{
	Vector3 res = m[3];
	res[3] = 0.0f;
	return res;
}

//TODO 从矩阵中提取旋转
inline Quaternion Matrix4x4::ExtractRotate(Matrix4x4& m)
{

}

//从矩阵中提取缩放
inline Vector3 Matrix4x4::ExtractScale(Matrix4x4& m)
{
	return Vector3(m.Determinant() < 0 ? -m[0].Magnitude() : m[0].Magnitude(), m[1].Magnitude(), m[2].Magnitude());
}

//矩阵行列式 (代数余子式法)
inline float Matrix4x4::Determinant()
{
	float a0 = m[0] * m[5] - m[1] * m[4];
	float a1 = m[0] * m[6] - m[2] * m[4];
	float a2 = m[0] * m[7] - m[3] * m[4];
	float a3 = m[1] * m[6] - m[2] * m[5];
	float a4 = m[1] * m[7] - m[3] * m[5];
	float a5 = m[2] * m[7] - m[3] * m[6];
	float b0 = m[8] * m[13] - m[9] * m[12];
	float b1 = m[8] * m[14] - m[10] * m[12];
	float b2 = m[8] * m[15] - m[11] * m[12];
	float b3 = m[9] * m[14] - m[10] * m[13];
	float b4 = m[9] * m[15] - m[11] * m[13];
	float b5 = m[10] * m[15] - m[11] * m[14];

	return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

//矩阵求逆 (伴随矩阵法)
inline bool Matrix4x4::Inverse(Matrix4x4& inverse)
{
	float a0 = m[0] * m[5] - m[1] * m[4];
	float a1 = m[0] * m[6] - m[2] * m[4];
	float a2 = m[0] * m[7] - m[3] * m[4];
	float a3 = m[1] * m[6] - m[2] * m[5];
	float a4 = m[1] * m[7] - m[3] * m[5];
	float a5 = m[2] * m[7] - m[3] * m[6];
	float b0 = m[8] * m[13] - m[9] * m[12];
	float b1 = m[8] * m[14] - m[10] * m[12];
	float b2 = m[8] * m[15] - m[11] * m[12];
	float b3 = m[9] * m[14] - m[10] * m[13];
	float b4 = m[9] * m[15] - m[11] * m[13];
	float b5 = m[10] * m[15] - m[11] * m[14];

	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	if (std::abs(det) <= Tolerance)
		return false;

	inverse.m[0] = m[5] * b5 - m[6] * b4 + m[7] * b3;
	inverse.m[1] = -m[1] * b5 + m[2] * b4 - m[3] * b3;
	inverse.m[2] = m[13] * a5 - m[14] * a4 + m[15] * a3;
	inverse.m[3] = -m[9] * a5 + m[10] * a4 - m[11] * a3;

	inverse.m[4] = -m[4] * b5 + m[6] * b2 - m[7] * b1;
	inverse.m[5] = m[0] * b5 - m[2] * b2 + m[3] * b1;
	inverse.m[6] = -m[12] * a5 + m[14] * a2 - m[15] * a1;
	inverse.m[7] = m[8] * a5 - m[10] * a2 + m[11] * a1;

	inverse.m[8] = m[4] * b4 - m[5] * b2 + m[7] * b0;
	inverse.m[9] = -m[0] * b4 + m[1] * b2 - m[3] * b0;
	inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
	inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

	inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
	inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
	inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
	inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

	inverse = inverse * (1.0f / det);

	return true;
}

std::ostream& operator<<(std::ostream& os, Matrix4x4& m)
{
	os << m.GetRow(0) << std::endl;
	os << m.GetRow(1) << std::endl;
	os << m.GetRow(2) << std::endl;
	os << m.GetRow(3) << std::endl;
	return os;
}