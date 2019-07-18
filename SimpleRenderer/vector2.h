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

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(float k) const;
	Vector2 operator/(float k) const;
};