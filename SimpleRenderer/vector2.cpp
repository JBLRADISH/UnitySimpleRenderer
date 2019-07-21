#include "vector2.h"

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(float k) const
{
	return Vector2(x * k, y * k);
}

Vector2 Vector2::operator/(float k) const
{
	return Vector2(x / k, y / k);
}

Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	return Vector2(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t);
}