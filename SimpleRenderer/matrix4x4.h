#pragma once

#include "vector3.h"

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

	Matrix4x4 Transpose();
	static Matrix4x4 Transpose(Matrix4x4& m);
};

const Matrix4x4 Matrix4x4::zero = Matrix4x4(Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 0.0f));
const Matrix4x4 Matrix4x4::identity = Matrix4x4(Vector3(1.0f, 0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f, 1.0f));

//重载下标运算符
inline Vector3& Matrix4x4::operator[](int idx)
{
	return v[idx];
}

//矩阵转置
inline Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4::Transpose(*this);
}

inline Matrix4x4 Matrix4x4::Transpose(Matrix4x4& m)
{
	Matrix4x4 res;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i][j] = m[j][i];
		}
	}
	return res;
}

