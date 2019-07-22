#pragma once

#include "shader.h"

class GouraudShader : public Shader
{
public:
	void SetLight(Light* light)
	{
		this->light = light;
	}

	void SetMaterial(const Material& material)
	{
		mat = material;
	}

	void SetTexture(Texture* tex) 
	{
		this->tex = tex;
	}

	void SetModelMatrix(const Matrix4x4& m)
	{
		this->m = m;
	}

	void SetViewProjectionMatrix(const Matrix4x4& vp)
	{
		this->vp = vp;
	}

	VertexOut VertexShader(const VertexIn& in)
	{
		VertexOut out;
		out.position = m * in.position;
		if (light)
		{
			Vector3 forward = light->GetLightDir();
			float costheta = Vector3::Dot(in.normal, forward * (-1.0f));
			if (costheta <= 0.0f)
			{
				out.color = Color::black;
			}
			else
			{
				costheta /= Vector3::Magnitude(in.normal);
				out.color = mat.cDiffuse * light->GetLightColor() * light->GetLightAtten(out.position) * costheta;
			}
		}
		else
		{
			out.color = mat.cDiffuse;
		}
		out.position = vp * out.position;
		out.texcoord = Vector3(in.texcoord.x, in.texcoord.y);
		return out;
	}

	Color FragmentShader(const VertexOut& in)
	{
		if (tex)
		{
			return in.color * tex->GetColor(in.texcoord);
		}
		return in.color;
	}

private:
	Matrix4x4 m;
	Matrix4x4 vp;
	Material mat;
	Light* light;
	Texture* tex;
};