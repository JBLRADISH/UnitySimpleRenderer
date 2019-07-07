#pragma once

#include <stdio.h>
#include "SDL.h"

class renderer
{
public:
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	int ping = 0;

	bool sdlInit(const char* title, int width, int height, int fps)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
			if (window == NULL)
			{
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				screenSurface = SDL_GetWindowSurface(window);
				ping = 1000 / fps;
				return true;
			}
		}
	}
};