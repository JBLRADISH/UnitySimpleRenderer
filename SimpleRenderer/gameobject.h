#pragma once

#include "transform.h"
#include "mesh.h"
#include "material.h"

class GameObject
{
public:
	Transform transform;
	Mesh mesh;
	Material material;

	Bounds GetWorldBounds()
	{
		Matrix4x4 m = transform.localToWorldMatrix();
		return mesh.GetWorldBounds(m);
	}
};
