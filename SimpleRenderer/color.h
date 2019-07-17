#pragma once

class Color
{
public:
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;

	Color() = default;

	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;

	Color(unsigned int r, unsigned int g, unsigned int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color operator+(const Color& c) const;
	Color operator*(float k) const;
	Color operator*(const Color& c) const;
};