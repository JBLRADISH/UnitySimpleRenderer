#pragma once

#include "vector3.h"
#include "quaternion.h"
#include "matrix4x4.h"

class Transform
{
public:
	Transform()
	{
		position = Vector3::zero;
		rotation = Quaternion::identity;
		scale = Vector3::one;
		dirty = true;
	}

	Matrix4x4 localToWorldMatrix();
	Matrix4x4 worldToLocalMatrix();

	const Vector3& GetPosition() { return position; }
	const Quaternion& GetRotation() { return rotation; }
	const Vector3& GetScale() { return scale; }
	void SetPosition(const Vector3& position) { this->position = position; dirty = true; }
	void SetRotation(const Quaternion& rotation) { this->rotation = rotation; dirty = true; }
	void SetScale(const Vector3& scale) { this->scale = scale; dirty = true; }

	void LookAt(Vector3 target);

private:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Matrix4x4 localToWorld;
	Matrix4x4 worldToLocal;
	bool dirty;
};
