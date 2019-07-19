#include "draw.h"
#include "tool.h"

void Draw::DrawPoint(SDL_Surface* surface, int x, int y, const Color& c)
{
	*GetPixelAddress(surface, x, y) = Color2Uint32(c);
}

void Draw::DrawHLine(SDL_Surface* surface, int x1, int x2, int y, const Color& c)
{
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	ASM_MEMSET_DWORD(GetPixelAddress(surface, x1, y), Color2Uint32(c), x2 - x1 + 1);
}

void Draw::DrawVLine(SDL_Surface* surface, int x, int y1, int y2, const Color& c)
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
void Draw::DrawDLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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

//普通的
void Draw::DrawGLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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

void Draw::DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c)
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
		DrawGLine(surface, x1, y1, x2, y2, c);
	}
}

bool Draw::ClipTest(float p, float q, float& u1, float& u2)
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

void Draw::DrawClipLine(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int y2, const Color& c)
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
void Draw::DrawTopTriangle_Flat(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float x3, float y3, const Color& c)
{
	if (x2 < x1)
	{
		float tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	float height = y3 - y1;
	float dx_left = (x3 - x1) / height;
	float dx_right = (x3 - x2) / height;

	float xs = x1;
	float xe = x2;

	int iy1, iy3;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (rect.y - y1);
		xe = xe + dx_right * (rect.y - y1);
		y1 = rect.y;
		iy1 = y1;
	}
	else 
	{
		iy1 = ceilf(y1);
		xs = xs + dx_left * (iy1 - y1);
		xe = xe + dx_right * (iy1 - y1);
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
		iy3 = y3 - 1;
	}
	else
	{
		iy3 = ceilf(y3) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);
	Uint32 pixel = Color2Uint32(c);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			ASM_MEMSET_DWORD(start + (int)xs, pixel, (int)xe - (int)xs + 1);
			xs += dx_left;
			xe += dx_right;
		}
	}
	else
	{
		float left;
		float right;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
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
			ASM_MEMSET_DWORD(start + (int)left, pixel, (int)right - (int)left + 1);
		}
	}
}

//平底三角形
void Draw::DrawBottomTriangle_Flat(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float x3, float y3, const Color& c)
{
	if (x3 < x2)
	{
		float tmp = x3;
		x3 = x2;
		x2 = tmp;
	}

	float height = y3 - y1;
	float dx_left = (x2 - x1) / height;
	float dx_right = (x3 - x1) / height;

	float xs = x1;
	float xe = x1;

	int iy1, iy3;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (rect.y - y1);
		xe = xe + dx_right * (rect.y - y1);
		y1 = rect.y;
		iy1 = y1;
	}
	else
	{
		iy1 = ceilf(y1);
		xs += dx_left * (iy1 - y1);
		xe += dx_right * (iy1 - y1);
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
		iy3 = y3 - 1;
	}
	else
	{
		iy3 = ceilf(y3) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);
	Uint32 pixel = Color2Uint32(c);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			ASM_MEMSET_DWORD(start + (int)xs, pixel, (int)xe - (int)xs + 1);
			xs += dx_left;
			xe += dx_right;
		}
	}
	else
	{
		float left;
		float right;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
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
			ASM_MEMSET_DWORD(start + (int)left, pixel, (int)right - (int)left + 1);
		}
	}
}

void Draw::DrawTriangle_Flat(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float y2, float x3, float y3, const Color& c)
{
	if ((Equal(x1, x2) && Equal(x2, x3)) || (Equal(y1, y2) && Equal(y2, y3)))
	{
		return;
	}

	if (y2 < y1)
	{
		float tmp1 = x2;
		float tmp2 = y2;
		x2 = x1;
		y2 = y1;
		x1 = tmp1;
		y1 = tmp2;
	}

	if (y3 < y1)
	{
		float tmp1 = x3;
		float tmp2 = y3;
		x3 = x1;
		y3 = y1;
		x1 = tmp1;
		y1 = tmp2;
	}

	if (y3 < y2)
	{
		float tmp1 = x3;
		float tmp2 = y3;
		x3 = x2;
		y3 = y2;
		x2 = tmp1;
		y2 = tmp2;

	}

	if (y3<rect.y || y1>rect.ymax() || (x1 < rect.x && x2 < rect.x && x3 < rect.x) || (x1 > rect.xmax() && x2 > rect.xmax() && x3 > rect.xmax()))
	{
		return;
	}

	if (Equal(y1, y2))
	{
		DrawTopTriangle_Flat(surface, rect, x1, y1, x2, x3, y3, c);
	}
	else if (Equal(y2, y3))
	{
		DrawBottomTriangle_Flat(surface, rect, x1, y1, x2, x3, y3, c);
	}
	else
	{
		float new_x = x1 + (y2 - y1) * (x3 - x1) / (y3 - y1);
		DrawBottomTriangle_Flat(surface, rect, x1, y1, new_x, x2, y2, c);
		DrawTopTriangle_Flat(surface, rect, x2, y2, new_x, x3, y3, c);
	}
}

void Draw::DrawTopTriangle_Gouraud(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float x3, float y3, const Color& c1, const Color& c2, const Color& c3)
{
	Color newC1 = c1;
	Color newC2 = c2;
	if (x2 < x1)
	{
		float tmp = x2;
		x2 = x1;
		x1 = tmp;
		Color tmp2 = newC2;
		newC2 = newC1;
		newC1 = tmp2;
	}

	float height = y3 - y1;
	float invHeight = 1 / (y3 - y1);
	float dx_left = (x3 - x1) * invHeight;
	float dx_right = (x3 - x2) * invHeight;
	Color dc_left = (c3 - newC1) * invHeight;
	Color dc_right = (c3 - newC2) * invHeight;

	float xs = x1;
	float xe = x2;
	Color cs = newC1;
	Color ce = newC2;

	int iy1, iy3;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (rect.y - y1);
		xe = xe + dx_right * (rect.y - y1);
		cs = cs + dc_left * (rect.y - y1);
		ce = ce + dc_right * (rect.y - y1);
		y1 = rect.y;
		iy1 = y1;
	}
	else
	{
		iy1 = ceilf(y1);
		xs = xs + dx_left * (iy1 - y1);
		xe = xe + dx_right * (iy1 - y1);
		cs = cs + dc_left * (iy1 - y1);
		ce = ce + dc_right * (iy1 - y1);
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
		iy3 = y3 - 1;
	}
	else
	{
		iy3 = ceilf(y3) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			Color curC = cs;
			Uint32* curP = start + (int)xs;
			Color dc = (ce - cs) / ((int)xe - (int)xs);
			for (int j = xs; j <= xe; j++)
			{
				*curP = Color2Uint32(curC);
				curC = curC + dc;
				curP++;
			}
			xs += dx_left;
			xe += dx_right;
			cs = cs + dc_left;
			ce = ce + dc_right;
		}
	}
	else
	{
		float left;
		float right;
		Color tmpCS;
		Color tmpCe;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
			tmpCS = cs;
			tmpCe = ce;
			xs += dx_left;
			xe += dx_right;
			cs = cs + dc_left;
			ce = ce + dc_right;
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
			Color curC = tmpCS;
			Uint32* curP = start + (int)left;
			Color dc = (tmpCe - tmpCS) / ((int)right - (int)left);
			for (int j = left; j <= right; j++)
			{
				*curP = Color2Uint32(curC);
				curC = curC + dc;
				curP++;
			}
		}
	}
}

void Draw::DrawBottomTriangle_Gouraud(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float x3, float y3, const Color& c1, const Color& c2, const Color& c3)
{
	Color newC2 = c2;
	Color newC3 = c3;
	if (x3 < x2)
	{
		float tmp = x3;
		x3 = x2;
		x2 = tmp;
		Color tmp2 = newC3;
		newC3 = newC2;
		newC2 = tmp2;
	}

	float height = y3 - y1;
	float invHeight = 1 / (y3 - y1);
	float dx_left = (x2 - x1) * invHeight;
	float dx_right = (x3 - x1) * invHeight;
	Color dc_left = (newC2 - c1) * invHeight;
	Color dc_right = (newC3 - c1) * invHeight;

	float xs = x1;
	float xe = x1;
	Color cs = c1;
	Color ce = c1;

	int iy1, iy3;

	if (y1 < rect.y)
	{
		xs = xs + dx_left * (rect.y - y1);
		xe = xe + dx_right * (rect.y - y1);
		cs = cs + dc_left * (rect.y - y1);
		ce = ce + dc_right * (rect.y - y1);
		y1 = rect.y;
		iy1 = y1;
	}
	else
	{
		iy1 = ceilf(y1);
		xs = xs + dx_left * (iy1 - y1);
		xe = xe + dx_right * (iy1 - y1);
		cs = cs + dc_left * (iy1 - y1);
		ce = ce + dc_right * (iy1 - y1);
	}
	if (y3 > rect.ymax())
	{
		y3 = rect.ymax();
		iy3 = y3 - 1;
	}
	else
	{
		iy3 = ceilf(y3) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (x1 >= rect.x && x1 <= rect.xmax() && x2 >= rect.x && x2 <= rect.xmax() && x3 >= rect.x && x3 <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			Color curC = cs;
			Uint32* curP = start + (int)xs;
			Color dc = (ce - cs) / ((int)xe - (int)xs);
			for (int j = xs; j <= xe; j++)
			{
				*curP = Color2Uint32(curC);
				curC = curC + dc;
				curP++;
			}
			xs += dx_left;
			xe += dx_right;
			cs = cs + dc_left;
			ce = ce + dc_right;
		}
	}
	else
	{
		float left;
		float right;
		Color tmpCS;
		Color tmpCe;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
			tmpCS = cs;
			tmpCe = ce;
			xs += dx_left;
			xe += dx_right;
			cs = cs + dc_left;
			ce = ce + dc_right;
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
			Color curC = tmpCS;
			Uint32* curP = start + (int)left;
			Color dc = (tmpCe - tmpCS) / ((int)right - (int)left);
			for (int j = left; j <= right; j++)
			{
				*curP = Color2Uint32(curC);
				curC = curC + dc;
				curP++;
			}
		}
	}
}

void Draw::DrawTriangle_Gouraud(SDL_Surface* surface, Rect& rect, float x1, float y1, float x2, float y2, float x3, float y3, const Color& c1, const Color& c2, const Color& c3)
{
	if ((Equal(x1, x2) && Equal(x2, x3)) || (Equal(y1, y2) && Equal(y2, y3)))
	{
		return;
	}

	Color newC1 = c1;
	Color newC2 = c2;
	Color newC3 = c3;

	if (y2 < y1)
	{
		float tmp1 = x2;
		float tmp2 = y2;
		x2 = x1;
		y2 = y1;
		x1 = tmp1;
		y1 = tmp2;
		Color tmp3 = newC2;
		newC2 = newC1;
		newC1 = tmp3;
	}

	if (y3 < y1)
	{
		float tmp1 = x3;
		float tmp2 = y3;
		x3 = x1;
		y3 = y1;
		x1 = tmp1;
		y1 = tmp2;
		Color tmp3 = newC3;
		newC3 = newC1;
		newC1 = tmp3;
	}

	if (y3 < y2)
	{
		float tmp1 = x3;
		float tmp2 = y3;
		x3 = x2;
		y3 = y2;
		x2 = tmp1;
		y2 = tmp2;
		Color tmp3 = newC3;
		newC3 = newC2;
		newC2 = tmp3;
	}

	if (y3<rect.y || y1>rect.ymax() || (x1 < rect.x && x2 < rect.x && x3 < rect.x) || (x1 > rect.xmax() && x2 > rect.xmax() && x3 > rect.xmax()))
	{
		return;
	}

	if (Equal(y1, y2))
	{
		DrawTopTriangle_Gouraud(surface, rect, x1, y1, x2, x3, y3, newC1, newC2, newC3);
	}
	else if (Equal(y2, y3))
	{
		DrawBottomTriangle_Gouraud(surface, rect, x1, y1, x2, x3, y3, newC1, newC2, newC3);
	}
	else
	{
		float inv = (y2 - y1) / (y3 - y1);
		float new_x = x1 + (x3 - x1) * inv;
		Color newC4 = newC1 + (newC3 - newC1) * inv;
		DrawBottomTriangle_Gouraud(surface, rect, x1, y1, new_x, x2, y2, newC1, newC4, newC2);
		DrawTopTriangle_Gouraud(surface, rect, x2, y2, new_x, x3, y3, newC2, newC4, newC3);
	}
}

void Draw::DrawTopTriangle_Tex_Gouraud(SDL_Surface* surface, Rect& rect, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3, const Material& mat, const ZBuffer& zBuffer)
{
	Vector3 newV1 = v1;
	Vector3 newV2 = v2;
	Vector3 newV3 = v3;
	Vector2 newUV1 = uv1;
	Vector2 newUV2 = uv2;
	if (newV2.x < newV1.x)
	{
		Vector3 tmp = newV2;
		newV2 = newV1;
		newV1 = tmp;
		Vector2 tmp2 = newUV2;
		newUV2 = newUV1;
		newUV1 = tmp2;
	}

	float invHeight = 1.0f / (newV3.y - newV1.y);
	float dx_left = (newV3.x - newV1.x) * invHeight;
	float dx_right = (newV3.x - newV2.x) * invHeight;
	float invv1z = 1.0f / newV1.z;
	float invv2z = 1.0f / newV2.z;
	float invv3z = 1.0f / newV3.z;
	float dz_left = (invv3z - invv1z) * invHeight;
	float dz_right = (invv3z - invv2z) * invHeight;
	Vector2 duv_left = (uv3 * invv3z - newUV1 * invv1z) * invHeight;
	Vector2 duv_right = (uv3 * invv3z - newUV2 * invv2z) * invHeight;

	float xs = newV1.x;
	float xe = newV2.x;
	float zs = invv1z;
	float ze = invv2z;
	Vector2 uvs = newUV1 * invv1z;
	Vector2 uve = newUV2 * invv2z;

	int iy1, iy3;

	if (newV1.y < rect.y)
	{
		float dy = rect.y - newV1.y;
		xs = xs + dx_left * dy;
		xe = xe + dx_right * dy;
		zs = zs + dz_left * dy;
		ze = ze + dz_right * dy;
		uvs = uvs + duv_left * dy;
		uve = uve + duv_right * dy;
		newV1.y = rect.y;
		iy1 = Floor(newV1.y);
	}
	else
	{
		iy1 = Ceil(newV1.y);
		float dy = iy1 - newV1.y;
		xs = xs + dx_left * dy;
		xe = xe + dx_right * dy;
		zs = zs + dz_left * dy;
		ze = ze + dz_right * dy;
		uvs = uvs + duv_left * dy;
		uve = uve + duv_right * dy;
	}
	if (newV3.y > rect.ymax())
	{
		newV3.y = rect.ymax();
		iy3 = Floor(newV3.y - 1.0f);
	}
	else
	{
		iy3 = Ceil(newV3.y) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (newV1.x >= rect.x && newV1.x <= rect.xmax() && newV2.x >= rect.x && newV2.x <= rect.xmax() && newV3.x >= rect.x && newV3.x <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			Vector2 curUV = uvs;
			float curZ = zs;
			int ixs = Floor(xs);
			float invdx = 1.0f / (Floor(xe) - ixs);
			Uint32* curP = start + ixs;
			Vector2 duv = (uve - uvs) * invdx;
			float dz = (ze - zs) * invdx;
			for (int j = xs; j <= xe; j++)
			{
				if (zBuffer.DepthTest(j, i, curZ))
				{
					*curP = mat.GetDiffusePixel(curUV / curZ);
				}
				curUV = curUV + duv;
				curZ += dz;
				curP++;
			}
			xs += dx_left;
			xe += dx_right;
			zs += dz_left;
			ze += dz_right;
			uvs = uvs + duv_left;
			uve = uve + duv_right;
		}
	}
	else
	{
		float left;
		float right;
		Vector2 tmpUVS;
		Vector2 tmpUVE;
		float tmpZS;
		float tmpZE;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
			tmpUVS = uvs;
			tmpUVE = uve;
			tmpZS = zs;
			tmpZE = ze;
			xs += dx_left;
			xe += dx_right;
			zs += dz_left;
			ze += dz_right;
			uvs = uvs + duv_left;
			uve = uve + duv_right;
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
			Vector2 curUV = tmpUVS;
			float curZ = tmpZS;
			int ileft = Floor(left);
			float invdx = 1.0f / (Floor(right) - ileft);
			Uint32* curP = start + ileft;
			Vector2 duv = (tmpUVE - tmpUVS) * invdx;
			float dz = (tmpZE - tmpZS) * invdx;
			for (int j = left; j <= right; j++)
			{
				if (zBuffer.DepthTest(j, i, curZ))
				{
					*curP = mat.GetDiffusePixel(curUV / curZ);
				}
				curUV = curUV + duv;
				curZ += dz;
				curP++;
			}
		}
	}
}

void Draw::DrawBottomTriangle_Tex_Gouraud(SDL_Surface* surface, Rect& rect, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3, const Material& mat, const ZBuffer& zBuffer)
{
	Vector3 newV1 = v1;
	Vector3 newV2 = v2;
	Vector3 newV3 = v3;
	Vector2 newUV2 = uv2;
	Vector2 newUV3 = uv3;
	if (newV3.x < newV2.x)
	{
		Vector3 tmp = newV3;
		newV3 = newV2;
		newV2 = tmp;
		Vector2 tmp2 = newUV3;
		newUV3 = newUV2;
		newUV2 = tmp2;
	}

	float invHeight = 1.0f / (newV3.y - newV1.y);
	float dx_left = (newV2.x - newV1.x) * invHeight;
	float dx_right = (newV3.x - newV1.x) * invHeight;
	float invv1z = 1.0f / newV1.z;
	float invv2z = 1.0f / newV2.z;
	float invv3z = 1.0f / newV3.z;
	float dz_left = (invv2z - invv1z) * invHeight;
	float dz_right = (invv3z - invv1z) * invHeight;
	Vector2 duv_left = (newUV2 * invv2z - uv1 * invv1z) * invHeight;
	Vector2 duv_right = (newUV3 * invv3z - uv1 * invv1z) * invHeight;

	float xs = newV1.x;
	float xe = newV1.x;
	float zs = invv1z;
	float ze = invv1z;
	Vector2 uvs = uv1 * invv1z;
	Vector2 uve = uv1 * invv1z;

	int iy1, iy3;

	if (newV1.y < rect.y)
	{
		float dy = rect.y - newV1.y;
		xs = xs + dx_left * dy;
		xe = xe + dx_right * dy;
		zs = zs + dz_left * dy;
		ze = ze + dz_right * dy;
		uvs = uvs + duv_left * dy;
		uve = uve + duv_right * dy;
		newV1.y = rect.y;
		iy1 = Floor(newV1.y);
	}
	else
	{
		iy1 = Ceil(newV1.y);
		float dy = iy1 - newV1.y;
		xs = xs + dx_left * dy;
		xe = xe + dx_right * dy;
		zs = zs + dz_left * dy;
		ze = ze + dz_right * dy;
		uvs = uvs + duv_left * dy;
		uve = uve + duv_right * dy;
	}
	if (newV3.y > rect.ymax())
	{
		newV3.y = rect.ymax();
		iy3 = Floor(newV3.y - 1.0f);
	}
	else
	{
		iy3 = Ceil(newV3.y) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (newV1.x >= rect.x && newV1.x <= rect.xmax() && newV2.x >= rect.x && newV2.x <= rect.xmax() && newV3.x >= rect.x && newV3.x <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			Vector2 curUV = uvs;
			float curZ = zs;
			int ixs = Floor(xs);
			float invdx = 1.0f / (Floor(xe) - ixs);
			Uint32* curP = start + ixs;
			Vector2 duv = (uve - uvs) * invdx;
			float dz = (ze - zs) * invdx;
			for (int j = xs; j <= xe; j++)
			{
				if (zBuffer.DepthTest(j, i, curZ))
				{
					*curP = mat.GetDiffusePixel(curUV / curZ);
				}
				curUV = curUV + duv;
				curZ += dz;
				curP++;
			}
			xs += dx_left;
			xe += dx_right;
			zs += dz_left;
			ze += dz_right;
			uvs = uvs + duv_left;
			uve = uve + duv_right;
		}
	}
	else
	{
		float left;
		float right;
		Vector2 tmpUVS;
		Vector2 tmpUVE;
		float tmpZS;
		float tmpZE;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			left = xs;
			right = xe;
			tmpUVS = uvs;
			tmpUVE = uve;
			tmpZS = zs;
			tmpZE = ze;
			xs += dx_left;
			xe += dx_right;
			zs += dz_left;
			ze += dz_right;
			uvs = uvs + duv_left;
			uve = uve + duv_right;
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
			Vector2 curUV = tmpUVS;
			float curZ = tmpZS;
			int ileft = Floor(left);
			float invdx = 1.0f / (Floor(right) - ileft);
			Uint32* curP = start + ileft;
			Vector2 duv = (tmpUVE - tmpUVS) * invdx;
			float dz = (tmpZE - tmpZS) * invdx;
			for (int j = left; j <= right; j++)
			{
				if (zBuffer.DepthTest(j, i, curZ))
				{
					*curP = mat.GetDiffusePixel(curUV / curZ);
				}
				curUV = curUV + duv;
				curZ += dz;
				curP++;
			}
		}
	}
}

void Draw::DrawTriangle_Tex_Gouraud(SDL_Surface* surface, Rect& rect, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3, const Material& mat, const ZBuffer& zBuffer)
{
	if ((Equal(v1.x, v2.x) && Equal(v2.x, v3.x)) || (Equal(v1.y, v2.y) && Equal(v2.y, v3.y)))
	{
		return;
	}

	Vector3 newV1 = v1;
	Vector3 newV2 = v2;
	Vector3 newV3 = v3;
	Vector2 newUV1 = uv1;
	Vector2 newUV2 = uv2;
	Vector2 newUV3 = uv3;

	if (newV2.y < newV1.y)
	{
		Vector3 tmp1 = newV2;
		newV2 = newV1;
		newV1 = tmp1;
		Vector2 tmp2 = newUV2;
		newUV2 = newUV1;
		newUV1 = tmp2;
	}

	if (newV3.y < newV1.y)
	{
		Vector3 tmp1 = newV3;
		newV3 = newV1;
		newV1 = tmp1;
		Vector2 tmp2 = newUV3;
		newUV3 = newUV1;
		newUV1 = tmp2;
	}

	if (newV3.y < newV2.y)
	{
		Vector3 tmp1 = newV3;
		newV3 = newV2;
		newV2 = tmp1;
		Vector2 tmp2 = newUV3;
		newUV3 = newUV2;
		newUV2 = tmp2;
	}

	if (newV3.y < rect.y || newV1.y > rect.ymax() || (newV1.x < rect.x && newV2.x < rect.x && newV3.x < rect.x) || (newV1.x > rect.xmax() && newV2.x > rect.xmax() && newV3.x > rect.xmax()))
	{
		return;
	}

	if (Equal(newV1.y, newV2.y))
	{
		DrawTopTriangle_Tex_Gouraud(surface, rect, newV1, newV2, newV3, newUV1, newUV2, newUV3, mat, zBuffer);
	}
	else if (Equal(newV2.y, newV3.y))
	{
		DrawBottomTriangle_Tex_Gouraud(surface, rect, newV1, newV2, newV3, newUV1, newUV2, newUV3, mat, zBuffer);
	}
	else
	{
		float inv = (newV2.y - newV1.y) / (newV3.y - newV1.y);
		Vector3 newV4(newV1.x + (newV3.x - newV1.x) * inv, newV2.y, newV1.z + (newV3.z - newV1.z) * inv);
		Vector2 newUV4 = newUV1 + (newUV3 - newUV1) * inv;
		DrawBottomTriangle_Tex_Gouraud(surface, rect, newV1, newV4, newV2, newUV1, newUV4, newUV2, mat, zBuffer);
		DrawTopTriangle_Tex_Gouraud(surface, rect, newV2, newV4, newV3, newUV2, newUV4, newUV3, mat, zBuffer);
	}
}

void Draw::DrawClearColor(SDL_Surface* surface, const Color& c)
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