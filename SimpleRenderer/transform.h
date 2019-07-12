#pragma once

#include "vector3.h"
#include "quaternion.h"

class Transform
{
public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};
