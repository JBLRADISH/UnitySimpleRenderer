#pragma once

#include "color.h"
#include "SDL.h"
#include "rect.h"

void DrawPoint(SDL_Surface* surface, int x, int y, const Color& c);
void DrawHLine(SDL_Surface* surface, int x1, int x2, int y, const Color& c);
void DrawVLine(SDL_Surface* surface, int x, int y1, int y2, const Color& c);
void DrawDLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c);
void DrawLine_Bresenham(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c);
void DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, const Color& c);
void DrawClipLine(SDL_Surface* surface, Rect& rect, int x1, int y1, int x2, int y2, const Color& c);
void DrawClearColor(SDL_Surface* surface, const Color& c);