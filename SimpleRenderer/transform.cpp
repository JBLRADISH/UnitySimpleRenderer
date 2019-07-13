#include "transform.h"

Matrix4x4 Transform::localToWorldMatrix()
{
	return Matrix4x4::TRS(position, rotation, scale);
}

Matrix4x4 Transform::worldToLocalMatrix()
{
	Matrix4x4 inverseM;
	localToWorldMatrix().Inverse(inverseM);
	return inverseM;
}

void Transform::LookAt(Vector3 target)
{
	rotation = Quaternion::LookRotation(target - position, Vector3::up);
}