#pragma once

#include "color.h"
#include "SDL.h"

Uint32 Color2Uint32(const Color& c)
{
	return ((c.r & 255) << 16) + ((c.g & 255) << 8) + (c.b & 255);
}

inline void ASM_MEMSET_DWORD(void* dest, Uint32 data, int count)
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