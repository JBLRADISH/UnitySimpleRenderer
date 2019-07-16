#pragma once

class Vector2
{
public:
	union
	{
		float v[2];
		struct
		{
			float x;
			float y;
		};
	};

	Vector2() = default;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};