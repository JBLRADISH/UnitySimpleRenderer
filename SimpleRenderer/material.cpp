#include "material.h"

Material Material::CreateWireframe(const Color& c)
{
	Material res;
	res.shadingMode = ShadingMode::Wireframe;
	res.cDiffuse = c;
	return res;
}

Material Material::CreateConstant(const Color& c)
{
	Material res;
	res.shadingMode = ShadingMode::Constant;
	res.cDiffuse = c;
	return res;
}

Material Material::CreateGouraud(float ka, float kd, float ks, float power, const Color& cDiffuse, const Color& cSpecular)
{
	Material res;
	res.shadingMode = ShadingMode::Gouraud;
	res.ka = ka;
	res.kd = kd;
	res.ks = ks;
	res.power = power;
	res.cDiffuse = cDiffuse * kd;
	res.cSpecular = cSpecular * ks;
	return res;
}