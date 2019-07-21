#pragma once

#include "color.h"
#include "transform.h"

enum LightType
{
	Ambient,
	DirectionalLight,
	PointLight,
	SpotLight
};

class Light
{
public:
	Transform transform;
	LightType type;
	Color color;
	float intensity;
	float range;
	//¾Û¹âµÆÄÚ×¶½Ç
	float spotInnerAngle;
	//¾Û¹âµÆÍâ×¶½Ç
	float spotOuterAngle;

	static Light CreateAmbientLight(const Color& c, float intensity = 1.0f);

	static Light CreateDirectionalLight(const Quaternion& q, const Color& c = Color::white, float intensity = 1.0f);

	static Light CreatePointLight(const Vector3& p, const Color& c = Color::white, float intensity = 1.0f, float range = 10.0f);

	static Light CreateSpotLight(const Transform& t, const Color& c = Color::white, float intensity = 1.0f, float range = 10.0f, float spotInnerAngle = 0.0f, float spotOuterAngle = 30.0f);

	Vector3 GetLightDir();

	Color GetLightColor();

	float GetLightAtten(const Vector3& p);

};