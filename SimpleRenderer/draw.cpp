#include "draw.h"
#include "tool.h"

void DrawPoint(SDL_Surface* surface, int x, int y, const Color& c)
{
	*GetPixelAddress(surface, x, y) = Color2Uint32(c);
}

void DrawHLine(SDL_Surface* surface, int x1, int x2, int y, const Color& c)
{
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	ASM_MEMSET_DWORD(GetPixelAddress(surface, x1, y), Color2Uint32(c), x2 - x1 + 1);
}

void DrawVLine(SDL_Surface* surface, int x, int y1, int y2, const Color& c)
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
void DrawDLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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

void DrawLine_Bresenham(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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

void DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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

bool ClipTest(float p, float q, float& u1, float& u2)
{
	float r;
	if (p < 0.0f)
	{
		r = q / p;
		if (r > u2)
		{
			return false;
		}
		else if (r > u1)
		{
			u1 = r;
		}
	}
	else if (p > 0.0f)
	{
		r = q / p;
		if (r < u1)
		{
			return false;
		}
		else if (r < u2)
		{
			u2 = r;
		}
	}
	else if (q < 0.0f)
	{
		return false;
	}
	return true;
}

void DrawClipLine(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int y2, const Color& c)
{
	float u1 = 0.0f, u2 = 1.0f;
	float dx = x2 - x1, dy;
	int clipx1 = x1, clipy1 = y1, clipx2 = x2, clipy2 = y2;
	if (ClipTest(-dx, x1 - rect.x, u1, u2))
	{
		if (ClipTest(dx, rect.xmax() - x1, u1, u2))
		{
			dy = y2 - y1;
			if (ClipTest(-dy, y1 - rect.y, u1, u2))
			{
				if (ClipTest(dy, rect.ymax() - y1, u1, u2))
				{
					if (u2 < 1.0f)
					{
						clipx2 = x1 + u2 * dx;
						clipy2 = y1 + u2 * dy;
					}
					if (u1 > 0.0f)
					{
						clipx1 = x1 + u1 * dx;
						clipy1 = y1 + u1 * dy;
					}
					DrawLine(surface, clipx1, clipy1, clipx2, clipy2, c);
				}
			}
		}
	}
}

//平顶三角形
void DrawClipTopTriangle(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int x3, int y3, const Color& c)
{
	if (x2 < x1)
	{
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	float height = y3 - y1;
	float dx_left = (x3 - x1) / height;
	float dx_right = (x3 - x2) / height;

	float xs = (float)x1;
	float xe = (float)x2 + 0.5f;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (float)(rect.y - y1);
		xe = xe + dx_right * (float)(rect.y - y1);
		y1 = rect.y;
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
	}

	Uint32* start = GetPixelAddress(surface, 0, y1);
	Uint32 pixel = Color2Uint32(c);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = y1; i <= y3; i++, start += surface->pitch >> 2)
		{
			ASM_MEMSET_DWORD(start + (int)xs, pixel, (int)xe - (int)xs + 1);
			xs += dx_left;
			xe += dx_right;
		}
	}
	else
	{
		int left;
		int right;
		for (int i = y1; i <= y3; i++, start += surface->pitch >> 2)
		{
			left = (int)xs;
			right = (int)xe;
			xs += dx_left;
			xe += dx_right;
			if (left < rect.x)
			{
				left = rect.x;
				if (right < rect.x)
					continue;
			}
			if (right > rect.xmax())
			{
				right = rect.xmax();
				if (left > rect.xmax())
					continue;
			}
			ASM_MEMSET_DWORD(start + left, pixel, right + left - 1);
		}
	}
}

//平底三角形
void DrawClipBottomTriangle(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int x3, int y3, const Color& c)
{
	if (x3 < x2)
	{
		int tmp = x3;
		x3 = x2;
		x2 = tmp;
	}

	float height = y3 - y1;
	float dx_left = (x2 - x1) / height;
	float dx_right = (x3 - x1) / height;

	float xs = (float)x1;
	float xe = (float)x1;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (float)(rect.y - y1);
		xe = xe + dx_right * (float)(rect.y - y1);
		y1 = rect.y;
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
	}

	Uint32* start = GetPixelAddress(surface, 0, y1);
	Uint32 pixel = Color2Uint32(c);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = y1; i <= y3; i++, start += surface->pitch >> 2)
		{
			ASM_MEMSET_DWORD(start + (int)xs, pixel, (int)xe - (int)xs + 1);
			xs += dx_left;
			xe += dx_right;
		}
	}
	else
	{
		int left;
		int right;
		for (int i = y1; i <= y3; i++, start += surface->pitch >> 2)
		{
			left = (int)xs;
			right = (int)xe;
			xs += dx_left;
			xe += dx_right;
			if (left < rect.x)
			{
				left = rect.x;
				if (right < rect.x)
					continue;
			}
			if (right > rect.xmax())
			{
				right = rect.xmax();
				if (left > rect.xmax())
					continue;
			}
			ASM_MEMSET_DWORD(start + left, pixel, right + left - 1);
		}
	}
}

void DrawClipTriangle(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int y2, int x3, int y3, const Color& c)
{
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
	{
		return;
	}

	if (y2 < y1)
	{
		int tmp1 = x2;
		int tmp2 = y2;
		x2 = x1;
		y2 = y1;
		x1 = tmp1;
		y1 = tmp2;
	}

	if (y3 < y1)
	{
		int tmp1 = x3;
		int tmp2 = y3;
		x3 = x1;
		y3 = y1;
		x1 = tmp1;
		y1 = tmp2;
	}

	if (y3 < y2)
	{
		int tmp1 = x3;
		int tmp2 = y3;
		x3 = x2;
		y3 = y2;
		x2 = tmp1;
		y2 = tmp2;

	}

	if (y3<rect.y || y1>rect.ymax() || (x1 < rect.x && x2 < rect.x && x3 < rect.x) || (x1 > rect.xmax() && x2 > rect.xmax() && x3 > rect.xmax()))
	{
		return;
	}

	if (y1 == y2)
	{
		DrawClipTopTriangle(surface, rect, x1, y1, x2, x3, y3, c);
	}
	else if (y2 == y3)
	{
		DrawClipBottomTriangle(surface, rect, x1, y1, x2, x3, y3, c);
	}
	else
	{
		int new_x = x1 + (int)(0.5f + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		DrawClipBottomTriangle(surface, rect, x1, y1, new_x, x2, y2, c);
		DrawClipTopTriangle(surface, rect, x2, y2, new_x, x3, y3, c);
	}
}

void DrawClearColor(SDL_Surface* surface, const Color& c)
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