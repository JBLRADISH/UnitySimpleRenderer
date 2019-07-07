#pragma once

#include "color.h"
#include "global_extern.h"

Uint32 Color2Uint32(Color* c)
{
	return (c->r << 16) + (c->g << 8) + c->b;
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

Uint32* GetPixelAddress(int x, int y)
{
	Uint32* pixels = (Uint32*)screenSurface->pixels;
	return pixels + y * screenSurface->w + x;
}