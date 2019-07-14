#include "matrix4x4.h"

const Matrix4x4 Matrix4x4::zero = Matrix4x4(Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f));
const Matrix4x4 Matrix4x4::identity = Matrix4x4(Vector3(1.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 1.0f));

//重载下标运算符 得到矩阵第n列
Vector3& Matrix4x4::operator[](int idx)
{
	return v[idx];
}

//重载矩阵向量乘法
Vector3 Matrix4x4::operator*(const Vector3& v)
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

Matrix4x4 Matrix4x4::operator*(float f)
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
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m)
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
Vector3 Matrix4x4::GetRow(int idx)
{
	return Vector3(v[0][idx], v[1][idx], v[2][idx], v[3][idx]);
}

//矩阵转置
Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4::Transpose(*this);
}

Matrix4x4 Matrix4x4::Transpose(Matrix4x4& m)
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
Matrix4x4 Matrix4x4::Translate(const Vector3& v)
{
	Matrix4x4 res = Matrix4x4::identity;
	res[3] = v;
	res[3][3] = 1.0f;
	return res;
}

//TODO 根据四元数创建旋转矩阵
Matrix4x4 Matrix4x4::Rotate(const Quaternion& q)
{
	float num1 = q.x * 2.0f;
	float num2 = q.y * 2.0f;
	float num3 = q.z * 2.0f;
	float num4 = q.x * num1;
	float num5 = q.y * num2;
	float num6 = q.z * num3;
	float num7 = q.x * num2;
	float num8 = q.x * num3;
	float num9 = q.y * num3;
	float num10 = q.w * num1;
	float num11 = q.w * num2;
	float num12 = q.w * num3;
	Matrix4x4 res;
	res.m00 = (float)(1.0 - ((double)num5 + (double)num6));
	res.m10 = num7 + num12;
	res.m20 = num8 - num11;
	res.m30 = 0.0f;
	res.m01 = num7 - num12;
	res.m11 = (float)(1.0 - ((double)num4 + (double)num6));
	res.m21 = num9 + num10;
	res.m31 = 0.0f;
	res.m02 = num8 + num11;
	res.m12 = num9 - num10;
	res.m22 = (float)(1.0 - ((double)num4 + (double)num5));
	res.m32 = 0.0f;
	res.m03 = 0.0f;
	res.m13 = 0.0f;
	res.m23 = 0.0f;
	res.m33 = 1.0f;
	return res;
}

//根据欧拉角创建旋转矩阵 local2world yxz
Matrix4x4 Matrix4x4::Rotate(const Vector3& v)
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
Matrix4x4 Matrix4x4::Scale(const Vector3& v)
{
	Matrix4x4 res = Matrix4x4::identity;
	res[0][0] = v.x;
	res[1][1] = v.y;
	res[2][2] = v.z;
	return res;
}

//TODO TRS矩阵
Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Quaternion& rotate, const Vector3& scale)
{
	float num1 = rotate.x * 2.0f;
	float num2 = rotate.y * 2.0f;
	float num3 = rotate.z * 2.0f;
	float num4 = rotate.x * num1;
	float num5 = rotate.y * num2;
	float num6 = rotate.z * num3;
	float num7 = rotate.x * num2;
	float num8 = rotate.x * num3;
	float num9 = rotate.y * num3;
	float num10 = rotate.w * num1;
	float num11 = rotate.w * num2;
	float num12 = rotate.w * num3;
	Matrix4x4 res;
	res.m00 = scale.x * (float)(1.0 - ((double)num5 + (double)num6));
	res.m10 = scale.x * (num7 + num12);
	res.m20 = scale.x * (num8 - num11);
	res.m30 = 0.0f;
	res.m01 = scale.y * (num7 - num12);
	res.m11 = scale.y * (float)(1.0 - ((double)num4 + (double)num6));
	res.m21 = scale.y * (num9 + num10);
	res.m31 = 0.0f;
	res.m02 = scale.z * (num8 + num11);
	res.m12 = scale.z * (num9 - num10);
	res.m22 = scale.z * (float)(1.0 - ((double)num4 + (double)num5));
	res.m32 = 0.0f;
	res.m03 = translation.x;
	res.m13 = translation.y;
	res.m23 = translation.z;
	res.m33 = 1.0f;
	return res;
}

Matrix4x4 Matrix4x4::TRS(const Vector3& translation, const Vector3& rotate, const Vector3& scale)
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
Vector3 Matrix4x4::ExtractTranslation(Matrix4x4& m)
{
	Vector3 res = m[3];
	res[3] = 0.0f;
	return res;
}

//TODO 从矩阵中提取旋转
Quaternion Matrix4x4::ExtractRotate(Matrix4x4& m)
{
	return Quaternion::LookRotation(m[2], m[1]);
}

//从矩阵中提取缩放
Vector3 Matrix4x4::ExtractScale(Matrix4x4& m)
{
	return Vector3(m.Determinant() < 0 ? -m[0].Magnitude() : m[0].Magnitude(), m[1].Magnitude(), m[2].Magnitude());
}

//创建透视投影矩阵
Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar)
{
	Matrix4x4 res = Matrix4x4::zero;
	float f_n = 1.0f / (zFar - zNear);
	float theta = Deg2Rad * fov * 0.5f;
	float divisor = tanf(theta);
	float factor = 1.0f / divisor;
	res[0][0] = (1.0f / aspect) * factor;
	res[1][1] = factor;
	res[2][2] = (-(zFar + zNear)) * f_n;
	res[2][3] = -1.0f;
	res[3][2] = -2.0f * zFar * zNear * f_n;
	return res;
}

//矩阵转换为欧拉角
Vector3 Matrix4x4::Euler()
{
	Vector3 res;
	res.x = atan2f(-v[2][1], sqrtf(v[0][1] * v[0][1] + v[1][1] * v[1][1])) * Rad2Deg;
	res.y = atan2f(v[2][0], v[2][2]) * Rad2Deg;
	res.z = atan2f(v[0][1], v[1][1]) * Rad2Deg;
	return res;
}

//矩阵行列式 (代数余子式法)
float Matrix4x4::Determinant()
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
bool Matrix4x4::Inverse(Matrix4x4& inverse)
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