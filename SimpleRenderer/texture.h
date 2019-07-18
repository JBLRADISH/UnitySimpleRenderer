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

	Uint32 GetPixel(Vector2 uv) const
	{
		int x = uv.x * (width - 1);
		int y = (1 - uv.y) * (height - 1);
		int idx = (y * width + x) * 4;
		//BGRA
		return RGB2Uint32(buffer[idx + 2], buffer[idx + 1], buffer[idx]);
	}
};