#pragma once

class Color
{
public:
	float r;
	float g;
	float b;
	float a;

	Color() = default;

	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;

	Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color& operator+=(const Color& c);
	Color operator+(const Color& c) const;
	Color operator-(const Color& c) const;
	Color operator*(float k) const;
	Color operator*(const Color& c) const;
	Color operator/(float k) const;

	static Color Lerp(const Color& c1, const Color& c2, float t);
};