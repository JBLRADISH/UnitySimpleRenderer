#pragma once

#include "color.h"
#include "tool.h"
#include <stdlib.h>

class Texture
{
public:
	int width;
	int height;
	unsigned char* buffer;

	Texture()
	{
		width = 0;
		height = 0;
		buffer = 0;
	}

	void UnLoad()
	{
		free(buffer);
		buffer = 0;
	}

	void GetRGB(Vector2 uv, int& r, int& g, int& b) const
	{
		if (width == 0)
		{
			r = 255;
			g = 255;
			b = 255;
			return;
		}
		int x = uv.x * (width - 1);
		//竖直方向要取反
		int y = (1 - uv.y) * (height - 1);
		int idx = (y * width + x) * 4;
		//BGRA
		r = buffer[idx + 2];
		g = buffer[idx + 1];
		b = buffer[idx];
	}
};