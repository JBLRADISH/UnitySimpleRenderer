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
	Matrix4x4 operator*(const Matrix4x4& m);

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

	static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);

	Vector3 Euler();

	float Determinant();
	bool Inverse(Matrix4x4& m);

	friend std::ostream& operator<< (std::ostream&, Matrix4x4&);
};

