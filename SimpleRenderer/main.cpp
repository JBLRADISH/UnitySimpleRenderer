#include <iostream>
#include "renderer.h"
#include "draw.h"
#include "obj.h"
#include "gameobject.h"
#include "camera.h"

void Input();
void Render();

renderer render;
bool quit = false;
SDL_Event e;

GameObject* go;
Camera* cam;
vector<Vector3> vertexBuffer;
Matrix4x4 mvp;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	go = &GameObject();
	go->mesh = Obj::Load("pig.obj");
	go->material = Material::CreateConstant(Color::red);
	go->transform.position = Vector3(0.0f, 1.5f, 0.0f);
	go->transform.scale = Vector3(2.0f, 2.0f, 2.0f);
	vertexBuffer.resize(go->mesh.vertexCount());
	cam = &Camera(60.0f, 0.3f, 1000.0f, Rect(0, 0, 800, 600));
	cam->transform.position = Vector3(0.0f, 0.0f, -10.0f);

	mvp = cam->projectionMatrix() * cam->worldToCameraMatrix() * go->transform.localToWorldMatrix();

	while (!quit)
	{
		Uint32 startFrame = SDL_GetTicks();
		Input();
		Render();
		Uint32 endFrame = SDL_GetTicks();
		//if (endFrame - startFrame < render.ping)
		//{
		//	SDL_Delay(render.ping - endFrame + startFrame);
		//}
		cout << "FPS: " << 1000 / fmaxf(1, (endFrame - startFrame)) << endl;
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
	DrawClearColor(render.screenSurface, Color::white);
	for (int i = 0; i < go->mesh.vertexCount(); i++)
	{
		vertexBuffer[i] = mvp * go->mesh.vertices[i];
		vertexBuffer[i] = cam->screenPoint(vertexBuffer[i]);
	}
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		Vector3 v1 = vertexBuffer[go->mesh.faces[i].vidx1];
		Vector3 v2 = vertexBuffer[go->mesh.faces[i].vidx2];
		Vector3 v3 = vertexBuffer[go->mesh.faces[i].vidx3];
		if (cam->CullFace_ScreenSpace(v1, v2, v3))
		{
			switch (go->material.shadingMode)
			{
			case ShadingMode::Wireframe:
				DrawClipLine(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, go->material.cDiffuse);
				DrawClipLine(render.screenSurface, cam->viewport, v1.x, v1.y, v3.x, v3.y, go->material.cDiffuse);
				DrawClipLine(render.screenSurface, cam->viewport, v2.x, v2.y, v3.x, v3.y, go->material.cDiffuse);
				break;
			case ShadingMode::Constant:
				DrawClipTriangle(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, go->material.cDiffuse);
			}
		}
	}
	SDL_UpdateWindowSurface(render.window);
}