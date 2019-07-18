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

//Lerp≤Â÷µ
Color Color::Lerp(const Color& c1, const Color& c2, float t)
{
	return Color(c1.r + (c2.r - c1.r) * t, c1.g + (c2.g - c1.g) * t, c1.b + (c2.b - c1.b) * t);
}