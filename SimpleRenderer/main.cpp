#include <iostream>
#include "renderer.h"
#include "draw.h"
#include "obj.h"
#include "gameobject.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "bmp.h"

void Input();
void Render();

renderer render;
bool quit = false;
SDL_Event e;

GameObject* go;
Camera* cam;
vector<Vector3> vertexBuffer;
vector<Color> colorBuffer;
Matrix4x4 m;
Matrix4x4 vp;
Matrix4x4 mvp;
vector<Light> lights;

int main(int argc, char* args[])
{
	if (!render.sdlInit("Renderer", 800, 600, 30))
	{
		return 0;
	}

	GameObject tmp = Obj::Load("lenin.obj");
	go = &tmp;
	go->material.shadingMode = ShadingMode::Gouraud;
	go->material.cDiffuse = Color::white;
	go->transform.scale = Vector3::one * 0.01f;
	vertexBuffer.resize(go->mesh.vertexCount());

	cam = &Camera(60.0f, 0.3f, 1000.0f, Rect(0, 0, 800, 600));
	cam->transform.position = Vector3(236.6051f, 119.8119f, -1.424029f);
	cam->transform.rotation = Quaternion::Euler(Vector3(6.532001f, -90.06901f, 0.0f));

	m = go->transform.localToWorldMatrix();
	vp = cam->projectionMatrix() * cam->worldToCameraMatrix();
	mvp = vp * m;

	Light directional = Light::CreateDirectionalLight(Quaternion::Euler(Vector3(90.0f, 0.0f, 0.0f)));
	lights.push_back(directional);

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
	Draw::DrawClearColor(render.screenSurface, Color::white);
	if (go->material.shadingMode == ShadingMode::Wireframe || go->material.shadingMode == ShadingMode::Constant)
	{
		for (int i = 0; i < go->mesh.vertexCount(); i++)
		{
			vertexBuffer[i] = mvp * go->mesh.vertices[i];
			vertexBuffer[i] = cam->screenPoint(vertexBuffer[i]);
		}
	}
	else
	{
		if (go->material.shadingMode == ShadingMode::Flat)
		{
			for (int i = 0; i < go->mesh.vertexCount(); i++)
			{
				vertexBuffer[i] = m * go->mesh.vertices[i];
			}
			colorBuffer.resize(go->mesh.faces.size());
			for (int i = 0; i < go->mesh.faces.size(); i++)
			{
				Vector3 v1 = vertexBuffer[go->mesh.faces[i].vidx1];
				Vector3 v2 = vertexBuffer[go->mesh.faces[i].vidx2];
				Vector3 v3 = vertexBuffer[go->mesh.faces[i].vidx3];
				Vector3 e0 = v1 - v2;
				Vector3 e1 = v2 - v3;
				Vector3 n = Vector3::Cross(e1, e0);
				Color c = Color::black;
				for (int j = 0; j < lights.size(); j++)
				{
					Light light = lights[j];
					if (light.type == LightType::Ambient)
					{
						c = c + light.GetLightColor(v1, n) * go->material.cAmbient;
					}
					else
					{
						c = c + go->material.cDiffuse * light.GetLightColor(v1, n);
					}
				}
				colorBuffer[i] = c;
			}
		}
		else if (go->material.shadingMode == ShadingMode::Gouraud)
		{
			colorBuffer.resize(go->mesh.vertexCount());
			for (int i = 0; i < go->mesh.vertexCount(); i++)
			{
				vertexBuffer[i] = m * go->mesh.vertices[i];
				//Vector3 v1 = vertexBuffer[i];
				//Vector3 n = m * go->mesh.normals[i];
				//Color c = Color::black;
				//for (int j = 0; j < lights.size(); j++)
				//{
				//	Light light = lights[j];
				//	if (light.type == LightType::Ambient)
				//	{
				//		c = c + light.GetLightColor(v1, n) * go->material.cAmbient;
				//	}
				//	else
				//	{
				//		c = c + go->material.cDiffuse * light.GetLightColor(v1, n);
				//	}
				//}
				//colorBuffer[i] = c;
			}
		}
		for (int i = 0; i < go->mesh.vertexCount(); i++)
		{
			vertexBuffer[i] = vp * vertexBuffer[i];
			vertexBuffer[i] = cam->screenPoint(vertexBuffer[i]);
		}
	}
	for (int i = 0; i < go->mesh.faces.size(); i++)
	{
		int idx1 = go->mesh.faces[i].vidx1;
		int idx2 = go->mesh.faces[i].vidx2;
		int idx3 = go->mesh.faces[i].vidx3;
		int uvidx1 = go->mesh.faces[i].uvidx1;
		int uvidx2 = go->mesh.faces[i].uvidx2;
		int uvidx3 = go->mesh.faces[i].uvidx3;
		Vector3 v1 = vertexBuffer[idx1];
		Vector3 v2 = vertexBuffer[idx2];
		Vector3 v3 = vertexBuffer[idx3];
		if (cam->CullFace_ScreenSpace(v1, v2, v3))
		{
			switch (go->material.shadingMode)
			{
			case ShadingMode::Wireframe:
				Draw::DrawClipLine(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, go->material.cDiffuse);
				Draw::DrawClipLine(render.screenSurface, cam->viewport, v1.x, v1.y, v3.x, v3.y, go->material.cDiffuse);
				Draw::DrawClipLine(render.screenSurface, cam->viewport, v2.x, v2.y, v3.x, v3.y, go->material.cDiffuse);
				break;
			case ShadingMode::Constant:
				Draw::DrawTriangle_Flat(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, go->material.cDiffuse);
				break;
			case ShadingMode::Flat:
				Draw::DrawTriangle_Flat(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, colorBuffer[i]);
				break;
			case ShadingMode::Gouraud:
				//Draw::DrawTriangle_Gouraud(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, colorBuffer[idx1], colorBuffer[idx2], colorBuffer[idx3]);
				Draw::DrawTriangle_Tex_Gouraud(render.screenSurface, cam->viewport, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, go->mesh.uv[uvidx1], go->mesh.uv[uvidx2], go->mesh.uv[uvidx3], go->material.diffuseTex);
				break;
			}
		}
	}
	SDL_UpdateWindowSurface(render.window);
}