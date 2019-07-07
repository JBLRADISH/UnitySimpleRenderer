#include "app.h"
#include "draw.h"

void Input();
void Render();

bool quit = false;
SDL_Event e;

int main(int argc, char* args[])
{
	if (!sdlInit())
	{
		return 0;
	}

	while (!quit)
	{
		Uint32 startFrame = SDL_GetTicks();
		Input();
		Render();
		Uint32 endFrame = SDL_GetTicks();
		if (endFrame - startFrame < ping)
		{
			SDL_Delay(ping - endFrame + startFrame);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void Input()
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
	}
}

void Render()
{
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	for (int i = 400; i <= 500; i++)
	{
		DrawPoint(i, 400, &Color(255, 0, 0));
	}
	DrawHLine(400, 500, 500, &Color(255, 255, 0));
	SDL_UpdateWindowSurface(window);
}
