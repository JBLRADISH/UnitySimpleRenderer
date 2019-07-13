#pragma once

#include "vector3.h"

class Plane
{
public:
	Vector3 n;
	float d;

	Plane() = default;

	Plane(Vector3 n, Vector3 p)
	{
		this->n = Vector3::Normalize(n);
		d = -Vector3::Dot(this->n, p);
	}
};