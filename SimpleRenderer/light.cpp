#include "light.h"
#include "mathf.h"

Light Light::CreateAmbientLight(const Color& c, float intensity)
{
	Light res;
	res.type = LightType::Ambient;
	res.color = c;
	res.intensity = intensity;
	return res;
}

Light Light::CreateDirectionalLight(const Quaternion& q, const Color& c, float intensity)
{
	Light res;
	res.transform.rotation = q;
	res.type = LightType::Directional;
	res.color = c;
	res.intensity = intensity;
	return res;
}

Light Light::CreatePointLight(const Vector3& p, const Color& c, float intensity, float range)
{
	Light res;
	res.transform.position = p;
	res.type = LightType::Point;
	res.color = c;
	res.intensity = intensity;
	res.range = range;
	return res;
}

Light Light::CreateSpotLight(const Transform& t, const Color& c, float intensity, float range, float spotInnerAngle, float spotOuterAngle)
{
	Light res;
	res.transform = t;
	res.type = LightType::Spot;
	res.color = c;
	res.intensity = intensity;
	res.range = range;
	res.spotInnerAngle = spotInnerAngle * 0.5f * Deg2Rad;
	res.spotOuterAngle = spotOuterAngle * 0.5f * Deg2Rad;
	return res;
}

Vector3 Light::GetLightDir()
{
	Quaternion q = transform.rotation;
	float num1 = q.x * 2.0f;
	float num2 = q.y * 2.0f;
	float num3 = q.z * 2.0f;
	float num4 = q.x * num1;
	float num5 = q.y * num2;
	float num8 = q.x * num3;
	float num9 = q.y * num3;
	float num10 = q.w * num1;
	float num11 = q.w * num2;
	return Vector3(num8 + num11, num9 - num10, (float)(1.0 - ((double)num4 + (double)num5)));
}

Color Light::GetLightColor(const Vector3& p, const Vector3& n)
{
	switch (type)
	{
	case Ambient:
		return color * intensity;
		break;
	case Directional:
	{
		Vector3 forward = GetLightDir();
		float costheta = Vector3::Dot(n, forward * (-1.0f));
		if (costheta <= 0.0f)
		{
			return Color::black;
		}
		costheta /= Vector3::Magnitude(n);
		return color * intensity * costheta;
	}
	break;
	case Point:
	{
		Vector3 d = transform.position - p;
		float costheta = Vector3::Dot(n, d);
		if (costheta <= 0.0f)
		{
			return Color::black;
		}
		float dist2 = d.SqrMagnitude();
		float range2 = range * range;
		if (dist2 >= range2)
		{
			return Color::black;
		}
		Color c = color * intensity * (1.0f / (dist2 + 0.01f));
		float ratio = dist2 / range2;
		ratio = ratio * ratio;
		float win = 1.0f - ratio;
		win = win * win;
		return c * win;
	}
	break;
	case Spot:
	{
		Vector3 forward = GetLightDir();
		float costheta = Vector3::Dot(n, forward * (-1.0f));
		if (costheta <= 0.0f)
		{
			return Color::black;
		}
		Vector3 d = p - transform.position;
		float costhetas = Vector3::Dot(forward, d) / Vector3::Magnitude(d);
		float costhetau = cosf(spotOuterAngle);
		if (costhetas <= costhetau)
		{
			return Color::black;
		}
		float dist2 = d.SqrMagnitude();
		float range2 = range * range;
		if (dist2 >= range2)
		{
			return Color::black;
		}
		Color c = color * intensity;
		float ratio = dist2 / range2;
		float dist = 1.0f - ratio;
		dist = dist * dist;
		c = c * dist;
		float t = fmin(1.0f, (costhetas - costhetau) / (cosf(spotInnerAngle) - costhetau));
		float dir = SmoothStep(t);
		return c * dir;
	}
	break;
	}
}