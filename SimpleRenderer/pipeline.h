#pragma once

#include "SDL.h"
#include "shader.h"
#include "rect.h"
#include "light.h"
#include "zbuffer.h"
#include <vector>
#include <map>
using namespace std;

enum Buffer
{
	ARRAY_BUFFER,
	ELEMENT_ARRAY_BUFFER,
};

enum PolygonMode
{
	Point,
	Line,
	Triangle
};

class Pipeline {
public:
	Pipeline();
	int CreateBuffer(Buffer buffer);
	void BindBuffer(int id);
	void BufferData(Buffer buffer, int size, void* data);
	void* GetBufferData(Buffer buffer);
	void DeleteBuffer(int id);

	int GenTexture();
	void BindTexture(int id);
	void Tex2DStorage(const string& filename, bool mipmap);
	void TexCubeStorage(vector<string> filename, bool mipmap);
	Texture* GetTextureData();
	void DeleteTexture(int id);

	void SetPolygonMode(PolygonMode polygonMode) { this->polygonMode = polygonMode; }
	void SetShader(Shader* shader) { this->shader = shader; }
	void SetMaterial(const Material& material) { this->material = material; }
	void Draw(int idx, int count);

	void SetLight(Light* light) { this->light = light; }

	void SetCullFace(bool cullFace) { this->cullFace = cullFace; }

	void SetScreenSurface(SDL_Surface* surface) { this->surface = surface; }
	void SetViewPort(const Rect& rect) { this->rect = rect; }

	void SetDepthTest(bool depthTest);
	void ClearZBuffer(float data) { zBuffer.ClearZBuffer(data); }

	void DrawPoint(int x, int y, const Color& c);
	void DrawHLine(int x1, int x2, int y, const Color& c);
	void DrawVLine(int x, int y1, int y2, const Color& c);
	void DrawDLine(int x1, int y1, int x2, int y2, const Color& c);
	void DrawGLine(int x1, int y1, int x2, int y2, const Color& c);
	void DrawLine(int x1, int y1, int x2, int y2, const Color& c);
	bool ClipTest(float p, float q, float& u1, float& u2);
	void DrawClipLine(int x1, int y1, int x2, int y2, const Color& c);
	void DrawTopTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3);
	void DrawBottomTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3);
	void DrawTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3);
	void DrawClearColor(const Color& c);

	Shader* shader;
private:
	int id;
	int texid;
	int bindtexid;
	map<int, Buffer> buffermap;
	map<int, void*> datamap;
	map<Buffer, int> bindmap;
	map<int, Texture*> texmap;
	PolygonMode polygonMode;
	SDL_Surface* surface;
	Material material;
	bool cullFace;
	Rect rect;
	Light* light;
	ZBuffer zBuffer;
	bool depthTest;

	bool CullFace_ScreenSpace(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3);
	void ScreenPoint(VertexOut& out);
	void PerspectiveCorrection(VertexOut& out);
};