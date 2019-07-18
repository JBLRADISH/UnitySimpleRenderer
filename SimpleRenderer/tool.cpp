#include "tool.h"

Uint32 Color2Uint32(const Color& c)
{
	return (Clamp(Round(c.r * 255), 0, 255) << 16) + (Clamp(Round(c.g * 255), 0, 255) << 8) + Clamp(Round(c.b * 255), 0, 255);
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