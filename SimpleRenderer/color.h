#pragma once

class Color
{
public:
	int r;
	int g;
	int b;

	Color() = default;

	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;

	Color(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color operator+(const Color& c) const;
	Color operator*(float k) const;
	Color operator*(const Color& c) const;
};