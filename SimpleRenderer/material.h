#pragma once

#include "color.h"
#include "texture.h"

enum ShadingMode
{
	Wireframe,//线框模式
	Constant, //固定着色
	Flat, //恒定着色
	Gouraud //逐顶点着色
};

class Material
{
public:
	ShadingMode shadingMode;

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
		shadingMode = ShadingMode::Wireframe;
		cAmbient = Color::black;
		cDiffuse = Color::white;
		cSpecular = Color::black;
		power = 0.0f;
	}
};