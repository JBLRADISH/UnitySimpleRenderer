#pragma once

#include "vector3.h"

class Plane
{
public:
	Vector3 n;
	float d;

	Plane() = default;

	Plane(Vector3 n, float d)
	{
		this->n = n;
		this->d = d;
	}

	bool Inside(const Vector3& v)
	{
		return Vector3::Dot(v, n) <= d;
	}
};