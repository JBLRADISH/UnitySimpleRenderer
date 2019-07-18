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
		Color c;
		diffuseTex.GetRGB(uv, c);
		return RGB2Uint32(c.r * cDiffuse.r, c.g * cDiffuse.g, c.b * cDiffuse.b);
	}
};