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
Matrix4x4 m;
Matrix4x4 vp;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	go = &GameObject();
	go->mesh = Obj::Load("pig.obj");
	go->material = Material::CreateWireframe(Color::red);
	go->transform.position = Vector3(0.0f, 1.5f, 0.0f);
	go->transform.scale = Vector3(2.0f, 2.0f, 2.0f);
	vertexBuffer.resize(go->mesh.vertexCount());

	cam = &Camera(60.0f, 0.3f, 1000.0f, Rect(0, 0, 800, 600));
	cam->transform.position = Vector3(0.0f, 0.0f, -10.0f);

	m = go->transform.localToWorldMatrix();
	vp = cam->projectionMatrix() * cam->worldToCameraMatrix();

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
		vertexBuffer[i] = m * go->mesh.vertices[i];
	}
	//背面裁剪
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		int vidx1 = go->mesh.faces[i].vidx1;
		int vidx2 = go->mesh.faces[i].vidx2;
		int vidx3 = go->mesh.faces[i].vidx3;
		if (!cam->CullFace(vertexBuffer[vidx1], vertexBuffer[vidx2], vertexBuffer[vidx3]))
		{
			go->mesh.faces[i].state = 0;
		}
	}
	for (int i = 0; i < go->mesh.vertexCount(); i++)
	{
		vertexBuffer[i] = vp * go->mesh.vertices[i];
		vertexBuffer[i] = cam->screenPoint(vertexBuffer[i]);
	}
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		if (go->mesh.faces[i].state != 0)
		{
			int vidx1 = go->mesh.faces[i].vidx1;
			int vidx2 = go->mesh.faces[i].vidx2;
			int vidx3 = go->mesh.faces[i].vidx3;
			switch (go->material.shadingMode)
			{
			case ShadingMode::Wireframe:
				DrawClipLine(render.screenSurface, cam->viewport, vertexBuffer[vidx1].x, vertexBuffer[vidx1].y, vertexBuffer[vidx2].x, vertexBuffer[vidx2].y, go->material.cDiffuse);
				DrawClipLine(render.screenSurface, cam->viewport, vertexBuffer[vidx1].x, vertexBuffer[vidx1].y, vertexBuffer[vidx3].x, vertexBuffer[vidx3].y, go->material.cDiffuse);
				DrawClipLine(render.screenSurface, cam->viewport, vertexBuffer[vidx2].x, vertexBuffer[vidx2].y, vertexBuffer[vidx3].x, vertexBuffer[vidx3].y, go->material.cDiffuse);
				break;
			case ShadingMode::Constant:
				DrawClipTriangle(render.screenSurface, cam->viewport, vertexBuffer[vidx1].x, vertexBuffer[vidx1].y, vertexBuffer[vidx2].x, vertexBuffer[vidx2].y, vertexBuffer[vidx3].x, vertexBuffer[vidx3].y, go->material.cDiffuse);
			}
		}
	}
	SDL_UpdateWindowSurface(render.window);
}