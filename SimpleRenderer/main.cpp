#include "renderer.h"
#include "draw.h"

void Input();
void Render();

renderer render;
bool quit = false;
SDL_Event e;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	while (!quit)
	{
		Uint32 startFrame = SDL_GetTicks();
		Input();
		Render();
		Uint32 endFrame = SDL_GetTicks();
		if (endFrame - startFrame < render.ping)
		{
			SDL_Delay(render.ping - endFrame + startFrame);
		}
	}

	SDL_DestroyWindow(render.window);
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
	DrawClearColor(render.screenSurface, &Color(255, 0, 0));
	DrawLine(render.screenSurface, 100, 200, 400, 500, &Color(0, 0, 0));
	SDL_UpdateWindowSurface(render.window);
}