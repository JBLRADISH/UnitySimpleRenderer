#pragma once

#include "shader.h"

class SkyBoxShader : public Shader
{
public:

	void SetTexture(Texture* tex)
	{
		this->tex = tex;
	}

	void SetViewProjectionMatrix(const Matrix4x4& vp)
	{
		this->vp = vp;
	}

	VertexOut VertexShader(const VertexIn& in)
	{
		VertexOut out;
		out.position = vp * in.position;
		out.position.z = out.position.w;
		out.texcoord = in.position;
		out.texcoord.y = -out.texcoord.y;
		return out;
	}

	Color FragmentShader(const VertexOut& in)
	{
		return tex->GetColor(in.texcoord);
	}

private:
	Matrix4x4 vp;
	Texture* tex;
};