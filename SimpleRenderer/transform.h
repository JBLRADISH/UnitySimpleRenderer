#pragma once

#include "vector3.h"
#include "quaternion.h"
#include "matrix4x4.h"

class Transform
{
public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	
	Transform()
	{
		position = Vector3::zero;
		rotation = Quaternion::identity;
		scale = Vector3::one;
	}

	Matrix4x4 localToWorldMatrix();
	Matrix4x4 worldToLocalMatrix();

	void LookAt(Vector3 target);
};
