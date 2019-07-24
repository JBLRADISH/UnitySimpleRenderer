#include "tool.h"

Uint32 Color2Uint32(const Color& c)
{
	return (Clamp(Round(c.r * 255), 0, 255) << 16) + (Clamp(Round(c.g * 255), 0, 255) << 8) + Clamp(Round(c.b * 255), 0, 255);
}

Uint32 Blend(Uint32 p1, Uint32 p2, float k)
{
	int r1 = (p1 >> 16) & 0xff;
	int g1 = (p1 >> 8) & 0xff;
	int b1 = p1 & 0xff;
	int r2 = (p2 >> 16) & 0xff;
	int g2 = (p2 >> 8) & 0xff;
	int b2 = p2 & 0xff;
	return (Round(r1 * k + r2 * (1 - k)) << 16) + (Round(g1 * k + g2 * (1 - k)) << 8) + Round(b1 * k + b2 * (1 - k));
}

Uint32 RGB2Uint32(int r, int g, int b)
{
	return (r << 16) + (g << 8) + b;
}

void ASM_MEMSET_DWORD(void* dest, Uint32 data, int count)
{
	_asm
	{
		mov edi, dest;
		mov ecx, count;
		mov eax, data;
		rep stosd;
	}
}

void ASM_MEMSET_DWORD(void* dest, float data, int count)
{
	_asm
	{
		mov edi, dest;
		mov ecx, count;
		mov eax, data;
		rep stosd;
	}
}

Uint32* GetPixelAddress(SDL_Surface* surface, int x, int y)
{
	Uint32* pixels = (Uint32*)surface->pixels;
	return pixels + y * (surface->pitch >> 2) + x;
}