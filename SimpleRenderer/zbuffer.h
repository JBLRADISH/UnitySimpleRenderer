#pragma once

#include "Rect.h"
#include "tool.h"

class ZBuffer
{
public:
	void CreateZBuffer(Rect& rect)
	{
		width = rect.width;
		height = rect.height;
		buffer = (float*)malloc(width * height * sizeof(float));
	}

	void ClearZBuffer(float data)
	{
		ASM_MEMSET_DWORD(buffer, data, width * height);
	}

	void DeleteZBuffer()
	{
		free(buffer);
		buffer = NULL;
	}

	bool DepthTest(int i, int j, float z) const
	{
		float tmp = buffer[j * width + i];
		if (z <= tmp)
		{
			buffer[j * width + i] = z;
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	int width;
	int height;
	float* buffer;
};