#include <iostream>
#include "renderer.h"
#include "obj.h"
#include "gameobject.h"
#include "camera.h"
#include "pipeline.h"
#include "gouraudshader.h"

void Input();
void Render();

renderer render;
bool quit = false;
SDL_Event e;

GameObject* go;
Camera* cam;
Pipeline pipeline;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	GameObject tmp = Obj::Load("lenin.obj");
	go = &tmp;
	go->material.cDiffuse = Color::white;
	go->transform.scale = Vector3::one * 0.01f;

	GouraudShader gouraudShader;
	pipeline.SetShader(&gouraudShader);
	pipeline.SetMaterial(go->material);

	VertexIn* vertexs = (VertexIn*)malloc(sizeof(VertexIn) * go->mesh.vertexCount());
	for (int i = 0; i < go->mesh.vertexCount(); i++)
	{
		vertexs[i].position = go->mesh.vertices[i];
		vertexs[i].normal = go->mesh.normals[i];
		vertexs[i].texcoord = go->mesh.uv[i];
	}
	int vbo = pipeline.CreateBuffer(Buffer::ARRAY_BUFFER);
	pipeline.BindBuffer(vbo);
	pipeline.BufferData(Buffer::ARRAY_BUFFER, sizeof(VertexIn) * go->mesh.vertexCount(), vertexs);
	free(vertexs);

	int* idxs = (int*)malloc(sizeof(int) * go->mesh.faces.size() * 3);
	int ibo = pipeline.CreateBuffer(Buffer::ELEMENT_ARRAY_BUFFER);
	pipeline.BindBuffer(ibo);
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		idxs[i * 3] = go->mesh.faces[i].vidx1;
		idxs[i * 3 + 1] = go->mesh.faces[i].vidx2;
		idxs[i * 3 + 2] = go->mesh.faces[i].vidx3;
	}
	pipeline.BufferData(Buffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * go->mesh.faces.size() * 3, idxs);
	free(idxs);

	int tbo = pipeline.GenTexture();
	pipeline.BindTexture(tbo);
	pipeline.TexStorage("diffuse.bmp");

	cam = &Camera(60.0f, 0.3f, 1000.0f, Rect(0, 0, 800, 600));
	cam->transform.position = Vector3(236.6051f, 119.8119f, -1.424029f);
	cam->transform.rotation = Quaternion::Euler(Vector3(6.532001f, -90.06901f, 0.0f));
	pipeline.SetViewPort(cam->viewport);

	pipeline.SetCullFace(true);
	pipeline.SetPolygonMode(PolygonMode::Triangle);
	pipeline.SetScreenSurface(render.screenSurface);

	//Light directionalLight = Light::CreateDirectionalLight(Quaternion::Euler(Vector3(90.0f, 0.0f, 0.0f)), Color::red, 2.0f);
	//pipeline.SetLight(&directionalLight);

	pipeline.SetDepthTest(true);

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
			case SDLK_w:
				break;
			case SDLK_s:
				break;
			}
		}
	}
}

void Render()
{
	pipeline.DrawClearColor(Color::white);
	pipeline.ClearZBuffer(1.0f);
	pipeline.shader->SetModelMatrix(go->transform.localToWorldMatrix());
	pipeline.shader->SetViewProjectionMatrix(cam->projectionMatrix() * cam->worldToCameraMatrix());
	pipeline.Draw(0, go->mesh.faces.size() * 3);
	SDL_UpdateWindowSurface(render.window);
}