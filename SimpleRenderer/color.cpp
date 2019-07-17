#include "color.h"

const Color Color::white = Color(1.0f, 1.0f, 1.0f);
const Color Color::black = Color(0.0f, 0.0f, 0.0f);
const Color Color::red = Color(1.0f, 0.0f, 0.0f);
const Color Color::green = Color(0.0f, 1.0f, 0.0f);
const Color Color::blue = Color(0.0f, 0.0f, 1.0f);

Color Color::operator+(const Color& c) const
{
	return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator-(const Color& c) const
{
	return Color(r - c.r, g - c.g, b - c.b);
}

Color Color::operator*(float k) const
{
	return Color(r * k, g * k, b * k);
}

Color Color::operator*(const Color& c) const
{
	return Color(r * c.r, g * c.g, b * c.b);
}

Color Color::operator/(float k) const
{
	return Color(r / k, g / k, b / k);
}