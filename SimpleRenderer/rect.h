#pragma once

class Rect {
public:
	int x;
	int y;
	int width;
	int height;

	Rect() = default;

	Rect(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	int xmax()
	{
		return x + width - 1;
	}

	int ymax()
	{
		return y + height - 1;
	}
};