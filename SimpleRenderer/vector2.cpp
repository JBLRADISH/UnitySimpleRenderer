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