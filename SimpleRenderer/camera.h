#pragma once

#include "rect.h"
#include "plane.h"
#include "transform.h"
#include "mesh.h"

class Camera
{
public:
	Transform transform;
	Rect viewport;
	float fov;
	float near;
	float far;

	Camera() = default;

	Camera(float fov, float near, float far, const Rect& viewport);

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
	Matrix4x4 projectionMatrix();
	Vector3 screenPoint(const Vector3& v);
	bool CullFace_WorldSpace(Vector3& v1, Vector3& v2, Vector3& v3);
	bool CullFace_ScreenSpace(Vector3& v1, Vector3& v2, Vector3& v3);

private:
	float aspect;
};