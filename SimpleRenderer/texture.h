#pragma once

#include "color.h"
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
};