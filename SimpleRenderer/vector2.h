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

	float& operator[](int idx);
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(float k) const;
	Vector2 operator/(float k) const;

	static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);
};