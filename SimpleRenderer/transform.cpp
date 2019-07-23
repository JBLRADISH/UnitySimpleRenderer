#include "transform.h"

Matrix4x4 Transform::localToWorldMatrix()
{
	if (dirty)
	{
		localToWorld = Matrix4x4::TRS(position, rotation, scale);
		localToWorld.Inverse(worldToLocal);
		dirty = false;
	}
	return localToWorld;
}

Matrix4x4 Transform::worldToLocalMatrix()
{
	if (dirty)
	{
		localToWorldMatrix();
	}
	return worldToLocal;
}

void Transform::LookAt(Vector3 target)
{
	rotation = Quaternion::LookRotation(target - position, Vector3::up);
}