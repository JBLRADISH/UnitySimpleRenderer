#pragma once

#include "matrix4x4.h"
#include "vertex.h"
#include "material.h"
#include "light.h"

class Shader {
public:
	virtual VertexOut VertexShader(const VertexIn& in) { return VertexOut(); }
	virtual Color FragmentShader(const VertexOut& in) { return Color::white; }
	virtual void SetViewProjectionMatrix(const Matrix4x4& vp) {}
	virtual void SetModelMatrix(const Matrix4x4& m) {}
	virtual void SetMaterial(const Material& material) {}
	virtual void SetLight(Light* light) {}
	virtual void SetTexture(Texture* tex) {}
};