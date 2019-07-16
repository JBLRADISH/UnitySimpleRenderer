#include "color.h"

const Color Color::white = Color(255, 255, 255);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::blue = Color(0, 0, 255);

Color Color::operator+(const Color& c) const
{
	return Color(r + c.r, g + c.g, b + c.b);
}

Color Color::operator*(float k) const
{
	return Color(r * k, g * k, b * k);
}

Color Color::operator*(const Color& c) const
{
	float inv = 1.0f / 255.0f;
	return Color(r * c.r * inv, g * c.g * inv, b * c.b * inv);
}