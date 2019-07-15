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
};
