#include "pipeline.h"
#include "tool.h"
#include "texture2d.h"
#include "texturecube.h"

Pipeline::Pipeline()
{
	id = 0;
	texid = 0;
	bindtexid = 0;
}

int Pipeline::CreateBuffer(Buffer buffer)
{
	id++;
	buffermap[id] = buffer;
	return id;
}

void Pipeline::BindBuffer(int id)
{
	if (buffermap.count(id))
	{
		bindmap[buffermap[id]] = id;
	}
}

void Pipeline::BufferData(Buffer buffer, int size, void* data)
{
	if (bindmap.count(buffer))
	{
		datamap[bindmap[buffer]] = malloc(size);
		memcpy(datamap[bindmap[buffer]], data, size);
	}
}

void* Pipeline::GetBufferData(Buffer buffer)
{
	if (bindmap.count(buffer) && datamap.count(bindmap[buffer]))
	{
		return datamap[bindmap[buffer]];
	}
}

void Pipeline::DeleteBuffer(int id)
{
	buffermap.erase(id);
	if (datamap.count(id))
	{
		free(datamap[id]);
		datamap.erase(id);
	}
}

int Pipeline::GenTexture()
{
	texid++;
	return texid;
}

void Pipeline::BindTexture(int id)
{
	bindtexid = id;
}

void Pipeline::Tex2DStorage(const string& filename, bool mipmap)
{
	if (bindtexid)
	{
		Texture2D* tex = new Texture2D;
		tex->Load(filename, mipmap);
		texmap[bindtexid] = tex;
	}
}

void Pipeline::TexCubeStorage(vector<string> filename, bool mipmap)
{
	if (bindtexid)
	{
		TextureCube* tex = new TextureCube;
		tex->Load(filename, mipmap);
		texmap[bindtexid] = tex;
	}
}

Texture* Pipeline::GetTextureData()
{
	if (texmap.count(bindtexid))
	{
		return texmap[bindtexid];
	}
	return NULL;
}

void Pipeline::DeleteTexture(int id)
{
	if (texmap.count(id))
	{
		texmap[id]->UnLoad();
		delete texmap[id];
		texmap.erase(id);
	}
}

void Pipeline::SetDepthTest(bool depthTest)
{
	this->depthTest = depthTest;
	if (depthTest)
	{
		zBuffer.CreateZBuffer(rect);
	}
}

void Pipeline::Draw(int idx, int count)
{
	int* idxs = (int*)GetBufferData(Buffer::ELEMENT_ARRAY_BUFFER);
	VertexIn* vertices = (VertexIn*)GetBufferData(Buffer::ARRAY_BUFFER);
	for (int i = idx; i < idx + count; i += 3)
	{
		VertexIn in1 = vertices[idxs[i]];
		VertexIn in2 = vertices[idxs[i + 1]];
		VertexIn in3 = vertices[idxs[i + 2]];
		VertexOut out1 = shader->VertexShader(in1);
		VertexOut out2 = shader->VertexShader(in2);
		VertexOut out3 = shader->VertexShader(in3);
		ScreenPoint(out1);
		ScreenPoint(out2);
		ScreenPoint(out3);
		if (cullFace && !CullFace_ScreenSpace(out1, out2, out3))
		{
			continue;
		}
		if (polygonMode == PolygonMode::Point)
		{
			DrawPoint(out1.position.x, out1.position.y, Color::black);
			DrawPoint(out2.position.x, out2.position.y, Color::black);
			DrawPoint(out3.position.x, out3.position.y, Color::black);
		}
		else if (polygonMode == PolygonMode::Line)
		{
			DrawClipLine(out1.position.x, out1.position.y, out2.position.x, out2.position.y, Color::black);
			DrawClipLine(out1.position.x, out1.position.y, out3.position.x, out3.position.y, Color::black);
			DrawClipLine(out2.position.x, out2.position.y, out3.position.x, out3.position.y, Color::black);
		}
		else if (polygonMode == PolygonMode::Triangle)
		{
			DrawTriangle(out1, out2, out3);
		}
	}
}

void Pipeline::PerspectiveCorrection(VertexOut& out1)
{
	out1.invz = 1.0f / out1.position.z;
	out1.color = out1.color * out1.invz;
	out1.texcoord = out1.texcoord * out1.invz;
}

void Pipeline::ScreenPoint(VertexOut& out)
{
	float invW = 1.0f / out.position.w;
	float ndcX = out.position.x * invW;
	float ndcY = out.position.y * invW;
	float ndcZ = (out.position.z * invW + 1.0f) / 2;
	out.position.x = rect.x + (ndcX + 1.0f) * 0.5f * (rect.xmax());
	out.position.y = rect.y + (1.0f - (ndcY + 1.0f) * 0.5f) * (rect.ymax());
	out.position.z = ndcZ;
}

//基于屏幕空间背面裁剪
bool Pipeline::CullFace_ScreenSpace(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3)
{
	//计算平面三角形有向面积 d >= 0 则被裁剪
	float d = out1.position.x * (out2.position.y - out3.position.y) + out1.position.y * (out3.position.x - out2.position.x) + out2.position.x * out3.position.y - out3.position.x * out2.position.y;
	//标准公式应该再d * 0.5f, 但现在只需得到正负
	if (d >= 0)
	{
		return false;
	}
	return true;
}

void Pipeline::DrawPoint(int x, int y, const Color& c)
{
	if (x >= rect.x && x <= rect.xmax() && y >= rect.y && y <= rect.ymax())
	{
		*GetPixelAddress(surface, x, y) = Color2Uint32(c);
	}
}

void Pipeline::DrawHLine(int x1, int x2, int y, const Color& c)
{
	if (x1 > x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	ASM_MEMSET_DWORD(GetPixelAddress(surface, x1, y), Color2Uint32(c), x2 - x1 + 1);
}

void Pipeline::DrawVLine(int x, int y1, int y2, const Color& c)
{
	if (y1 > y2)
	{
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	Uint32* start = GetPixelAddress(surface, x, y1);
	Uint32 pixel = Color2Uint32(c);
	for (int i = 0; i <= y2 - y1; i++)
	{
		*start = pixel;
		start += surface->pitch >> 2;
	}
}

//对角线 k=1
void Pipeline::DrawDLine(int x1, int y1, int x2, int y2, const Color& c)
{
	int xInc = x2 > x1 ? 1 : -1;
	int yInc = y2 > y1 ? (surface->pitch >> 2) : -(surface->pitch >> 2);
	Uint32* start = GetPixelAddress(surface, x1, y1);
	Uint32 pixel = Color2Uint32(c);
	for (int i = 0; i <= x2 - x1; i++)
	{
		*start = pixel;
		start += xInc + yInc;
	}
}

//普通的
void Pipeline::DrawGLine(int x1, int y1, int x2, int y2, const Color& c)
{
	Uint32 pixel = Color2Uint32(c);
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx > dy)
	{
		if (x1 > x2)
		{
			int tmp = x1;
			x1 = x2;
			x2 = tmp;

			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		int yInc = y2 > y1 ? (surface->pitch >> 2) : -(surface->pitch >> 2);
		int dydouble = dy << 1;
		int dydoubleminusdxdouble = (dy - dx) << 1;
		int p = dydouble - dx;
		Uint32* start = GetPixelAddress(surface, x1, y1);
		*start = pixel;
		for (int i = 0; i < dx; i++)
		{
			start++;
			if (p < 0)
				p += dydouble;
			else
			{
				start += yInc;
				p += dydoubleminusdxdouble;
			}
			*start = pixel;
		}
	}
	else
	{
		if (y1 > y2)
		{
			int tmp = y1;
			y1 = y2;
			y2 = tmp;

			tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		int xInc = x2 > x1 ? 1 : -1;
		int dxdouble = dx << 1;
		int dxdoubleminusdydouble = (dx - dy) << 1;
		int p = dxdouble - dy;
		Uint32* start = GetPixelAddress(surface, x1, y1);
		*start = pixel;
		for (int i = 0; i < dy; i++)
		{
			start += surface->pitch >> 2;
			if (p < 0)
				p += dxdouble;
			else
			{
				start += xInc;
				p += dxdoubleminusdydouble;
			}
			*start = pixel;
		}
	}
}

void Pipeline::DrawLine(int x1, int y1, int x2, int y2, const Color& c)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dy == 0)
	{
		DrawHLine(x1, x2, y1, c);
	}
	else if (dx == 0)
	{
		DrawVLine(x1, y1, y2, c);
	}
	else if (dx == dy)
	{
		DrawDLine(x1, y1, x2, y2, c);
	}
	else
	{
		DrawGLine(x1, y1, x2, y2, c);
	}
}

bool Pipeline::ClipTest(float p, float q, float& u1, float& u2)
{
	float r;
	if (p < 0.0f)
	{
		r = q / p;
		if (r > u2)
		{
			return false;
		}
		else if (r > u1)
		{
			u1 = r;
		}
	}
	else if (p > 0.0f)
	{
		r = q / p;
		if (r < u1)
		{
			return false;
		}
		else if (r < u2)
		{
			u2 = r;
		}
	}
	else if (q < 0.0f)
	{
		return false;
	}
	return true;
}

void Pipeline::DrawClipLine(int x1, int y1, int x2, int y2, const Color& c)
{
	float u1 = 0.0f, u2 = 1.0f;
	float dx = x2 - x1, dy;
	int clipx1 = x1, clipy1 = y1, clipx2 = x2, clipy2 = y2;
	if (ClipTest(-dx, x1 - rect.x, u1, u2))
	{
		if (ClipTest(dx, rect.xmax() - x1, u1, u2))
		{
			dy = y2 - y1;
			if (ClipTest(-dy, y1 - rect.y, u1, u2))
			{
				if (ClipTest(dy, rect.ymax() - y1, u1, u2))
				{
					if (u2 < 1.0f)
					{
						clipx2 = x1 + u2 * dx;
						clipy2 = y1 + u2 * dy;
					}
					if (u1 > 0.0f)
					{
						clipx1 = x1 + u1 * dx;
						clipy1 = y1 + u1 * dy;
					}
					DrawLine(clipx1, clipy1, clipx2, clipy2, c);
				}
			}
		}
	}
}

void Pipeline::DrawTopTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3)
{
	VertexOut newOut1 = out1;
	VertexOut newOut2 = out2;
	VertexOut newOut3 = out3;
	PerspectiveCorrection(newOut1);
	PerspectiveCorrection(newOut2);
	PerspectiveCorrection(newOut3);
	if (newOut2.position.x < newOut1.position.x)
	{
		VertexOut tmp = newOut2;
		newOut2 = newOut1;
		newOut1 = tmp;
	}

	float invHeight = 1.0f / (newOut3.position.y - newOut1.position.y);
	VertexOut dleft = (newOut3 - newOut1) * invHeight;
	VertexOut dright = (newOut3 - newOut2) * invHeight;

	VertexOut s = newOut1;
	VertexOut e = newOut2;


	int iy1, iy3;

	if (newOut1.position.y < rect.y)
	{
		float dy = rect.y - newOut1.position.y;
		s += dleft * dy;
		e += dright * dy;
		newOut1.position.y = rect.y;
		iy1 = Floor(newOut1.position.y);
	}
	else
	{
		iy1 = Ceil(newOut1.position.y);
		float dy = iy1 - newOut1.position.y;
		s += dleft * dy;
		e += dright * dy;
	}

	if (newOut3.position.y > rect.ymax())
	{
		newOut3.position.y = rect.ymax();
		iy3 = Floor(newOut3.position.y - 1.0f);
	}
	else
	{
		iy3 = Ceil(newOut3.position.y) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (newOut1.position.x >= rect.x && newOut1.position.x <= rect.xmax() && newOut2.position.x >= rect.x && newOut2.position.x <= rect.xmax() && newOut3.position.x >= rect.x && newOut3.position.x <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			VertexOut cur = s;
			int ixs = Floor(s.position.x);
			int ixe = Floor(e.position.x);
			float invdx = 1.0f / (ixe - ixs);
			Uint32* curP = start + ixs;
			VertexOut d = (e - s) * invdx;
			for (int j = ixs; j <= ixe; j++)
			{
				if (!depthTest || zBuffer.DepthTest(j, i, cur.position.z))
				{
					float invz = 1.0f / cur.invz;
					VertexOut tmp = cur;
					tmp.color = cur.color * invz;
					tmp.texcoord = cur.texcoord * invz;
					Color c = shader->FragmentShader(tmp);
					*curP = Color2Uint32(c);
				}
				cur += d;
				curP++;
			}
			s += dleft;
			e += dright;
		}
	}
	else
	{
		VertexOut tmps;
		VertexOut tmpe;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			tmps = s;
			tmpe = e;
			s += dleft;
			e += dright;
			if (tmps.position.x < rect.x)
			{
				tmps.position.x = rect.x;
				if (tmpe.position.x < rect.x)
					continue;
			}
			if (tmpe.position.x > rect.xmax())
			{
				tmpe.position.x = rect.xmax();
				if (tmps.position.x > rect.xmax())
					continue;
			}
			VertexOut cur = tmps;
			int ileft = Floor(tmps.position.x);
			int iright = Floor(tmpe.position.x);
			float invdx = 1.0f / (iright - ileft);
			Uint32* curP = start + ileft;
			VertexOut d = (tmpe - tmps) * invdx;
			for (int j = ileft; j <= iright; j++)
			{
				if (!depthTest || zBuffer.DepthTest(j, i, cur.position.z))
				{
					float invz = 1.0f / cur.invz;
					VertexOut tmp = cur;
					tmp.color = cur.color * invz;
					tmp.texcoord = cur.texcoord * invz;
					Color c = shader->FragmentShader(tmp);
					*curP = Color2Uint32(c);
				}
				cur += d;
				curP++;
			}
		}
	}
}

void Pipeline::DrawBottomTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3)
{
	VertexOut newOut1 = out1;
	VertexOut newOut2 = out2;
	VertexOut newOut3 = out3;
	PerspectiveCorrection(newOut1);
	PerspectiveCorrection(newOut2);
	PerspectiveCorrection(newOut3);
	if (newOut3.position.x < newOut2.position.x)
	{
		VertexOut tmp = newOut3;
		newOut3 = newOut2;
		newOut2 = tmp;
	}

	float invHeight = 1.0f / (newOut3.position.y - newOut1.position.y);
	VertexOut dleft = (newOut2 - newOut1) * invHeight;
	VertexOut dright = (newOut3 - newOut1) * invHeight;

	VertexOut s = newOut1;
	VertexOut e = newOut1;

	int iy1, iy3;

	if (newOut1.position.y < rect.y)
	{
		float dy = rect.y - newOut1.position.y;
		s += dleft * dy;
		e += dright * dy;
		newOut1.position.y = rect.y;
		iy1 = Floor(newOut1.position.y);
	}
	else
	{
		iy1 = Ceil(newOut1.position.y);
		float dy = iy1 - newOut1.position.y;
		s += dleft * dy;
		e += dright * dy;
	}

	if (newOut3.position.y > rect.ymax())
	{
		newOut3.position.y = rect.ymax();
		iy3 = Floor(newOut3.position.y - 1.0f);
	}
	else
	{
		iy3 = Ceil(newOut3.position.y) - 1;
	}

	Uint32* start = GetPixelAddress(surface, 0, iy1);

	if (newOut1.position.x >= rect.x && newOut1.position.x <= rect.xmax() && newOut2.position.x >= rect.x && newOut2.position.x <= rect.xmax() && newOut3.position.x >= rect.x && newOut3.position.x <= rect.xmax())
	{
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			VertexOut cur = s;
			int ixs = Floor(s.position.x);
			int ixe = Floor(e.position.x);
			float invdx = 1.0f / (ixe - ixs);
			Uint32* curP = start + ixs;
			VertexOut d = (e - s) * invdx;
			for (int j = ixs; j <= ixe; j++)
			{
				if (!depthTest || zBuffer.DepthTest(j, i, cur.position.z))
				{
					float invz = 1.0f / cur.invz;
					VertexOut tmp = cur;
					tmp.color = cur.color * invz;
					tmp.texcoord = cur.texcoord * invz;
					Color c = shader->FragmentShader(tmp);
					*curP = Color2Uint32(c);
				}
				cur += d;
				curP++;
			}
			s += dleft;
			e += dright;
		}
	}
	else
	{
		VertexOut tmps;
		VertexOut tmpe;
		for (int i = iy1; i <= iy3; i++, start += surface->pitch >> 2)
		{
			tmps = s;
			tmpe = e;
			s += dleft;
			e += dright;
			if (tmps.position.x < rect.x)
			{
				tmps.position.x = rect.x;
				if (tmpe.position.x < rect.x)
					continue;
			}
			if (tmpe.position.x > rect.xmax())
			{
				tmpe.position.x = rect.xmax();
				if (tmps.position.x > rect.xmax())
					continue;
			}
			VertexOut cur = tmps;
			int ileft = Floor(tmps.position.x);
			int iright = Floor(tmpe.position.x);
			float invdx = 1.0f / (iright - ileft);
			Uint32* curP = start + ileft;
			VertexOut d = (tmpe - tmps) * invdx;
			for (int j = ileft; j <= iright; j++)
			{
				if (!depthTest || zBuffer.DepthTest(j, i, cur.position.z))
				{
					float invz = 1.0f / cur.invz;
					VertexOut tmp = cur;
					tmp.color = cur.color * invz;
					tmp.texcoord = cur.texcoord * invz;
					Color c = shader->FragmentShader(tmp);
					*curP = Color2Uint32(c);
				}
				cur += d;
				curP++;
			}
		}
	}
}

void Pipeline::DrawTriangle(const VertexOut& out1, const VertexOut& out2, const VertexOut& out3)
{
	if ((Equal(out1.position.x, out2.position.x) && Equal(out2.position.x, out3.position.x)) || (Equal(out1.position.y, out2.position.y) && Equal(out2.position.y, out3.position.y)))
	{
		return;
	}

	VertexOut newOut1 = out1;
	VertexOut newOut2 = out2;
	VertexOut newOut3 = out3;

	if (newOut2.position.y < newOut1.position.y)
	{
		VertexOut tmp1 = newOut2;
		newOut2 = newOut1;
		newOut1 = tmp1;
	}

	if (newOut3.position.y < newOut1.position.y)
	{
		VertexOut tmp1 = newOut3;
		newOut3 = newOut1;
		newOut1 = tmp1;
	}

	if (newOut3.position.y < newOut2.position.y)
	{
		VertexOut tmp1 = newOut3;
		newOut3 = newOut2;
		newOut2 = tmp1;
	}

	if (newOut3.position.y < rect.y || newOut1.position.y > rect.ymax() || (newOut1.position.x < rect.x && newOut2.position.x < rect.x && newOut3.position.x < rect.x) || (newOut1.position.x > rect.xmax() && newOut2.position.x > rect.xmax() && newOut3.position.x > rect.xmax()))
	{
		return;
	}

	shader->SetMaterial(material);
	shader->SetLight(light);
	shader->SetTexture(GetTextureData());

	if (Equal(newOut1.position.y, newOut2.position.y))
	{
		DrawTopTriangle(newOut1, newOut2, newOut3);
	}
	else if (Equal(newOut2.position.y, newOut3.position.y))
	{
		DrawBottomTriangle(newOut1, newOut2, newOut3);
	}
	else
	{
		float t = (newOut2.position.y - newOut1.position.y) / (newOut3.position.y - newOut1.position.y);
		VertexOut newOut4 = VertexOut::Lerp(newOut1, newOut3, t);
		newOut4.position.y = newOut2.position.y;
		DrawBottomTriangle(newOut1, newOut4, newOut2);
		DrawTopTriangle(newOut2, newOut4, newOut3);
	}
}

void Pipeline::DrawClearColor(const Color& c)
{
	//内存对齐的考虑 有时会不相等
	if (surface->pitch == surface->w << 2)
	{
		ASM_MEMSET_DWORD(GetPixelAddress(surface, 0, 0), Color2Uint32(c), surface->w * surface->h);
	}
	else
	{
		Uint32* start = GetPixelAddress(surface, 0, 0);
		Uint32 pixel = Color2Uint32(c);
		for (int i = 0; i < surface->h; i++)
		{
			ASM_MEMSET_DWORD(start, pixel, surface->w);
			start += (surface->pitch >> 2);
		}
	}
}