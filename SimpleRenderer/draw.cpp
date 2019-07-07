#include "draw.h"
#include "tool.h"

void DrawPoint(int x, int y, Color* c)
{
	*GetPixelAddress(x, y) = Color2Uint32(c);
}

void DrawHLine(int x1, int x2, int y, Color* c)
{
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	ASM_MEMSET_DWORD(GetPixelAddress(x1, y), Color2Uint32(c), x2 - x1 + 1);
}