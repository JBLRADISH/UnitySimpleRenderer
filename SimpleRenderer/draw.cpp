#include "draw.h"
#include "tool.h"

void DrawPoint(SDL_Surface* surface, int x, int y, Color* c)
{
	*GetPixelAddress(surface, x, y) = Color2Uint32(c);
}

void DrawHLine(SDL_Surface* surface, int x1, int x2, int y, Color* c)
{
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	ASM_MEMSET_DWORD(GetPixelAddress(surface, x1, y), Color2Uint32(c), x2 - x1 + 1);
}

void DrawVLine(SDL_Surface* surface, int x, int y1, int y2, Color* c)
{
	if (y1 > y2)
	{
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	Uint32* start = GetPixelAddress(surface, x, y1);
	Uint32 pixel = Color2Uint32(c);
	for (int i = 0; i <= y2 - y1; i++)
	{
		*start = pixel;
		start += surface->pitch >> 2;
	}
}

//对角线 k=1
void DrawDLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c)
{
	int xInc = x2 > x1 ? 1 : -1;
	int yInc = y2 > y1 ? (surface->pitch >> 2) : -(surface->pitch >> 2);
	Uint32* start = GetPixelAddress(surface, x1, y1);
	Uint32 pixel = Color2Uint32(c);
	for (int i = 0; i <= x2 - x1; i++)
	{
		*start = pixel;
		start += xInc + yInc;
	}
}

void DrawLine_Bresenham(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c)
{
	Uint32 pixel = Color2Uint32(c);
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx > dy)
	{
		if (x1 > x2)
		{
			int tmp = x1;
			x1 = x2;
			x2 = tmp;

			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		int yInc = y2 > y1 ? (surface->pitch >> 2) : -(surface->pitch >> 2);
		int dydouble = dy << 1;
		int dydoubleminusdxdouble = (dy - dx) << 1;
		int p = dydouble - dx;
		Uint32* start = GetPixelAddress(surface, x1, y1);
		*start = pixel;
		for (int i = 0; i < dx; i++)
		{
			start++;
			if (p < 0)
				p += dydouble;
			else
			{
				start += yInc;
				p += dydoubleminusdxdouble;
			}
			*start = pixel;
		}
	}
	else
	{
		if (y1 > y2)
		{
			int tmp = y1;
			y1 = y2;
			y2 = tmp;

			tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		int xInc = x2 > x1 ? 1 : -1;
		int dxdouble = dx << 1;
		int dxdoubleminusdydouble = (dx - dy) << 1;
		int p = dxdouble - dy;
		Uint32* start = GetPixelAddress(surface, x1, y1);
		*start = pixel;
		for (int i = 0; i < dy; i++)
		{
			start += surface->pitch >> 2;
			if (p < 0)
				p += dxdouble;
			else
			{
				start += xInc;
				p += dxdoubleminusdydouble;
			}
			*start = pixel;
		}
	}
}

void DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dy == 0)
	{
		DrawHLine(surface, x1, x2, y1, c);
	}
	else if (dx == 0)
	{
		DrawVLine(surface, x1, y1, y2, c);
	}
	else if (dx == dy)
	{
		DrawDLine(surface, x1, y1, x2, y2, c);
	}
	else
	{
		DrawLine_Bresenham(surface, x1, y1, x2, y2, c);
	}
}

void DrawClearColor(SDL_Surface* surface, Color* c)
{
	//内存对齐的考虑 有时会不相等
	if (surface->pitch == surface->w << 2)
	{
		ASM_MEMSET_DWORD(GetPixelAddress(surface, 0, 0), Color2Uint32(c), surface->w * surface->h);
	}
	else
	{
		Uint32* start = GetPixelAddress(surface, 0, 0);
		Uint32 pixel = Color2Uint32(c);
		for (int i = 0; i < surface->h; i++)
		{
			ASM_MEMSET_DWORD(start, pixel, surface->w);
			start += (surface->pitch >> 2);
		}
	}
}