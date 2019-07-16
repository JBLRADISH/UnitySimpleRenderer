#pragma once

#include "color.h"

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

	//环境光反射系数
	float ka;
	//散射光反射系数
	float kd;
	//镜面反射光的反射系数
	float ks;
	//镜面反射指数
	float power;

	Color cDiffuse;
	Color cSpecular;

	static Material CreateWireframe(const Color& c);

	static Material CreateConstant(const Color& c);

	static Material CreateFlat(const Color& c, float ka = 1.0f, float kd = 1.0f);

	static Material CreateGouraud(float ka = 0.0f, float kd = 1.0f, float ks = 0.0f, float power = 128.0f, const Color& cDiffuse = Color::white, const Color& cSpecular = Color::white);
};