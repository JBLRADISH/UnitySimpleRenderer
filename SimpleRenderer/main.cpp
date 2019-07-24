#include <iostream>
#include "renderer.h"
#include "obj.h"
#include "gameobject.h"
#include "camera.h"
#include "pipeline.h"
#include "gouraudshader.h"
#include "skyboxshader.h"

void Input();
void Render();

renderer render;
bool quit = false;
SDL_Event e;

GameObject* go;
Camera* cam;
Pipeline pipeline;

int itemvbo;
int itemibo;
int itemtbo;
int skyboxvbo;
int skyboxibo;
int skyboxtbo;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	GameObject item = Obj::Load("lenin.obj");
	go = &item;
	go->material.cDiffuse = Color::white;
	go->transform.SetRotation(Quaternion::Euler(Vector3(0.0f, 90.0f, 0.0f)));
	go->transform.SetScale(Vector3::one * 0.01f);

	VertexIn* vertexs = (VertexIn*)malloc(sizeof(VertexIn) * go->mesh.vertexCount());
	for (int i = 0; i < go->mesh.vertexCount(); i++)
	{
		vertexs[i].position = go->mesh.vertices[i];
		vertexs[i].normal = go->mesh.normals[i];
		vertexs[i].texcoord = go->mesh.uv[i];
	}
	itemvbo = pipeline.CreateBuffer(Buffer::ARRAY_BUFFER);
	pipeline.BindBuffer(itemvbo);
	pipeline.BufferData(Buffer::ARRAY_BUFFER, sizeof(VertexIn) * go->mesh.vertexCount(), vertexs);
	free(vertexs);

	int* idxs = (int*)malloc(sizeof(int) * go->mesh.faces.size() * 3);
	itemibo = pipeline.CreateBuffer(Buffer::ELEMENT_ARRAY_BUFFER);
	pipeline.BindBuffer(itemibo);
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		idxs[i * 3] = go->mesh.faces[i].vidx1;
		idxs[i * 3 + 1] = go->mesh.faces[i].vidx2;
		idxs[i * 3 + 2] = go->mesh.faces[i].vidx3;
	}
	pipeline.BufferData(Buffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * go->mesh.faces.size() * 3, idxs);
	free(idxs);

	itemtbo = pipeline.GenTexture();
	pipeline.BindTexture(itemtbo);
	pipeline.Tex2DStorage("diffuse.bmp", false);

	GameObject skybox = Obj::Load("cube.obj");

	vertexs = (VertexIn*)malloc(sizeof(VertexIn) * skybox.mesh.vertexCount());
	for (int i = 0; i < skybox.mesh.vertexCount(); i++)
	{
		vertexs[i].position = skybox.mesh.vertices[i];
	}
	skyboxvbo = pipeline.CreateBuffer(Buffer::ARRAY_BUFFER);
	pipeline.BindBuffer(skyboxvbo);
	pipeline.BufferData(Buffer::ARRAY_BUFFER, sizeof(VertexIn) * skybox.mesh.vertexCount(), vertexs);
	free(vertexs);

	idxs = (int*)malloc(sizeof(int) * skybox.mesh.faces.size() * 3);
	skyboxibo = pipeline.CreateBuffer(Buffer::ELEMENT_ARRAY_BUFFER);
	pipeline.BindBuffer(skyboxibo);
	for (int i = 0; i < skybox.mesh.faces.size(); i++)
	{
		idxs[i * 3] = skybox.mesh.faces[i].vidx1;
		idxs[i * 3 + 1] = skybox.mesh.faces[i].vidx2;
		idxs[i * 3 + 2] = skybox.mesh.faces[i].vidx3;
	}
	pipeline.BufferData(Buffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * skybox.mesh.faces.size() * 3, idxs);
	free(idxs);

	skyboxtbo = pipeline.GenTexture();
	pipeline.BindTexture(skyboxtbo);
	pipeline.TexCubeStorage({ "right.bmp", "top.bmp", "front.bmp", "left.bmp", "bottom.bmp", "back.bmp" }, false);

	cam = &Camera(60.0f, 0.3f, 1000.0f, Rect(0, 0, 800, 600));
	cam->transform.SetPosition(Vector3(0.0f, 100.0f, -245.0f));
	pipeline.SetViewPort(cam->viewport, cam->GetZNear());

	pipeline.SetCullFace(true);
	pipeline.SetPolygonMode(PolygonMode::Triangle);
	pipeline.SetScreenSurface(render.screenSurface);

	pipeline.SetMaterial(go->material);

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
			case SDLK_l:
				pipeline.SetPolygonMode(PolygonMode::Line);
				break;
			case SDLK_p:
				pipeline.SetPolygonMode(PolygonMode::Point);
				break;
			case SDLK_t:
				pipeline.SetPolygonMode(PolygonMode::Triangle);
				break;
			}
		}
	}
}

void Render()
{
	pipeline.DrawClearColor(Color::white);
	pipeline.ClearZBuffer(1.0f);

	//天空盒
	SkyBoxShader skyboxShader;
	pipeline.SetShader(&skyboxShader);
	pipeline.SetAlphaBlend(1.0f);
	pipeline.shader->SetViewProjectionMatrix(cam->projectionMatrix() * cam->worldToCameraMatrix().IgnoreTranslate());
	pipeline.BindBuffer(skyboxvbo);
	pipeline.BindBuffer(skyboxibo);
	pipeline.BindTexture(skyboxtbo);
	pipeline.Draw(0, 6);

	//物体剔除
	if (!cam->OutSide(go->GetWorldBounds()))
	{
		GouraudShader gouraudShader;
		pipeline.SetShader(&gouraudShader);
		pipeline.SetAlphaBlend(0.5f);
		pipeline.shader->SetModelMatrix(go->transform.localToWorldMatrix());
		pipeline.shader->SetViewProjectionMatrix(cam->viewProjectionMatrix());
		pipeline.BindBuffer(itemvbo);
		pipeline.BindBuffer(itemibo);
		pipeline.BindTexture(itemtbo);
		pipeline.Draw(0, go->mesh.faces.size() * 3);
	}

	SDL_UpdateWindowSurface(render.window);
}