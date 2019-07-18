#pragma once

#include "color.h"
#include "texture.h"

class Material
{
public:

	Color cAmbient;
	Color cDiffuse;
	Color cSpecular;

	//镜面反射指数
	float power;

	Texture diffuseTex;
	Texture specularTex;
	Texture normalTex;

	Material()
	{
		cAmbient = Color::black;
		cDiffuse = Color::white;
		cSpecular = Color::black;
		power = 0.0f;
	}

	Uint32 GetDiffusePixel(Vector2 uv) const
	{
		int r, g, b;
		diffuseTex.GetRGB(uv, r, g, b);
		return RGB2Uint32(r * cDiffuse.r, g * cDiffuse.g, b * cDiffuse.b);
	}
};