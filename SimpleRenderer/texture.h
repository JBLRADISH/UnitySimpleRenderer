#pragma once

#include "color.h"
#include "tool.h"
#include <stdlib.h>

enum TextureFilter 
{
	Filter_Point,
	Filter_Bilinear,
	Filter_Trilinear
};

class Texture
{
public:
	int width;
	int height;
	unsigned char** buffer;
	int mipmapLevel;

	TextureFilter texFilter;

	Texture()
	{
		width = 0;
		height = 0;
		texFilter = TextureFilter::Filter_Bilinear;
	}

	void SetMipMapLevel(int mipmapLevel)
	{
		this->mipmapLevel = mipmapLevel;
		buffer = (unsigned char**)malloc(sizeof(unsigned char*) * (mipmapLevel + 1));
	}

	void UnLoad()
	{
		for (int i = 0; i <= mipmapLevel; i++)
		{
			free(buffer[i]);
			buffer[i] = 0;
		}
		free(buffer);
		buffer = 0;
	}

	Color GetColor(int idx, int level) const
	{
		//BGRA
		return Color(buffer[level][idx + 2], buffer[level][idx + 1], buffer[level][idx]);
	}

	void GetRGB(Vector2 uv, Color& c) const
	{
		if (width == 0)
		{
			c = Color::white;
			return;
		}
		if (texFilter == TextureFilter::Filter_Point)
		{
			int x = uv.x * (width - 1);
			//竖直方向要取反
			int y = (1 - uv.y) * (height - 1);
			int idx = (y * width + x) << 2;
			c = GetColor(idx, 0);
		}
		else if (texFilter == TextureFilter::Filter_Bilinear)
		{
			c = Bilinear(uv, width, height, 0);
		}
		else if (texFilter == TextureFilter::Filter_Trilinear)
		{
			float d = 0.0f;
			int floord = Floor(d);
			if (Equal(d, floord))
			{
				c = Bilinear(uv, width, height, 0);
			}
			else
			{
				int ceild = floord + 1;
				Color ceilColor = Bilinear(uv, width >> ceild, height >> ceild, ceild);
				Color floorColor = Bilinear(uv, width >> floord, height >> floord, floord);
				c = Color::Lerp(floorColor, ceilColor, d - floord);
			}
		}
	}

	Color Bilinear(Vector2 uv, int width, int height, int mipmapLevel) const
	{
		Color res;
		float x = uv.x * (width - 1);
		float y = (1 - uv.y) * (height - 1);
		int floorx = Floor(x);
		int floory = Floor(y);
		int ceilx = Ceil(x);
		int ceily = Ceil(y);
		int lefttopidx = (floory * width + floorx) << 2;
		int leftbottomidx = (ceily * width + floorx) << 2;
		int righttopidx = (floory * width + ceilx) << 2;
		int rightbottomidx = (ceily * width + ceilx) << 2;
		Color topColor = Color::Lerp(GetColor(lefttopidx, mipmapLevel), GetColor(righttopidx, mipmapLevel), x - floorx);
		Color bottomColor = Color::Lerp(GetColor(leftbottomidx, mipmapLevel), GetColor(rightbottomidx, mipmapLevel), x - floorx);
		res = Color::Lerp(topColor, bottomColor, y - floory);
		return res;
	}

	void GenerateMipMap()
	{
		int curWidth = width;
		int curHeight = height;
		for (int i = 1; i <= mipmapLevel; i++)
		{
			int originWidth = curWidth;
			curWidth = curWidth >> 1;
			curHeight = curHeight >> 1;
			buffer[i] = (unsigned char*)malloc(curWidth * curHeight * 4);
			for (int x = 0; x < curWidth; x++)
			{
				for (int y = 0; y < curHeight; y++)
				{
					int lefttopidx = (y * 2 * originWidth + x * 2) * 4;
					int leftbottomidx = ((y * 2 + 1) * originWidth + x * 2) * 4;
					int righttopidx = (y * 2 * originWidth + x * 2 + 1) * 4;
					int rightbottomidx = ((y * 2 + 1) * originWidth + x * 2 + 1) * 4;
					Color average = GetColor(lefttopidx, i - 1) + GetColor(leftbottomidx, i - 1) + GetColor(righttopidx, i - 1) + GetColor(rightbottomidx, i - 1);
					average = average / 4;
					int idx = (y * curWidth + x) * 4;
					buffer[i][idx] = Round(average.b);
					buffer[i][idx + 1] = Round(average.g);
					buffer[i][idx + 2] = Round(average.r);
				}
			}
		}
	}
};