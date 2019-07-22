#pragma once

#include "vector3.h"
#include "color.h"

class Texture {
public:
	virtual Color GetColor(const Vector3& uv) const = 0;
	virtual void UnLoad() = 0;
};