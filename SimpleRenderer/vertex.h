#pragma once

#include "vector3.h"
#include "vector2.h"
#include "color.h"

class VertexIn {
public:
	Vector3 position;
	Vector3 normal;
	Vector2 texcoord;
	Color color;
};

class VertexOut {
public:
	Vector3 position;
	Vector2 texcoord;
	Color color;
	float invz;

	VertexOut operator+(const VertexOut& out)
	{
		VertexOut res;
		res.position = position + out.position;
		res.texcoord = texcoord + out.texcoord;
		res.color = color + out.color;
		res.invz = invz + out.invz;
		return res;
	}

	VertexOut operator-(const VertexOut& out) const
	{
		VertexOut res;
		res.position = position - out.position;
		res.texcoord = texcoord - out.texcoord;
		res.color = color - out.color;
		res.invz = invz - out.invz;
		return res;
	}

	VertexOut operator*(float f) const
	{
		VertexOut res;
		res.position = position * f;
		res.texcoord = texcoord * f;
		res.color = color * f;
		res.invz = invz * f;
		return res;
	}

	static VertexOut Lerp(const VertexOut& out1, const VertexOut& out2, float t)
	{
		VertexOut res;
		res.position = Vector3::Lerp(out1.position, out2.position, t);
		res.color = Color::Lerp(out1.color, out2.color, t);
		res.texcoord = Vector2::Lerp(out1.texcoord, out2.texcoord, t);
		res.invz = out1.invz + (out2.invz - out1.invz) * t;
		return res;
	}
};