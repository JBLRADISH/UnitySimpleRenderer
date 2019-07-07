#pragma once

#include "color.h"
#include "SDL.h"

void DrawPoint(SDL_Surface* surface, int x, int y, Color* c);
void DrawHLine(SDL_Surface* surface, int x1, int x2, int y, Color* c);
void DrawVLine(SDL_Surface* surface, int x, int y1, int y2, Color* c);
void DrawDLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c);
void DrawLine_Bresenham(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c);
void DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Color* c);
void DrawClearColor(SDL_Surface* surface, Color* c);