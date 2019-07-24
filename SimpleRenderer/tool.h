#pragma once

#include "color.h"
#include "SDL.h"
#include "mathf.h"

Uint32 Color2Uint32(const Color& c);

Uint32 Blend(Uint32 p1, Uint32 p2, float k);

Uint32 RGB2Uint32(int r, int g, int b);

void ASM_MEMSET_DWORD(void* dest, Uint32 data, int count);

void ASM_MEMSET_DWORD(void* dest, float data, int count);

Uint32* GetPixelAddress(SDL_Surface* surface, int x, int y);