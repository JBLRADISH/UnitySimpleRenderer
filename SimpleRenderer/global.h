#pragma once

#include <SDL.h>

const int fps = 30;
const int ping = 1000 / fps;
const int width = 800;
const int height = 600;
const char* title = "Renderer";

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;